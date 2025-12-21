import os
import json
import re
import httpx
from rapidfuzz import fuzz
from fastapi import FastAPI, UploadFile, File, HTTPException, Form, APIRouter
from pydantic import BaseModel
from deepgram import DeepgramClient
from fastapi.middleware.cors import CORSMiddleware
from openai import OpenAI
from dotenv import load_dotenv

load_dotenv()

def check_and_init_db():
    print("Database initialization mocked.")
    pass

class MockRouter:
    def __init__(self, name):
        self.router = APIRouter()
        print(f"Mocking router: {name}")

patients = MockRouter("patients")
documents = MockRouter("documents")
new_patient_forms = MockRouter("new_patient_forms")
medical_reports = MockRouter("medical_reports")
consultation_forms = MockRouter("consultation_forms")
prescription_forms = MockRouter("prescription_forms")

HF_TOKEN = os.getenv("HF_TOKEN", "")

# Verificare token HF_TOKEN
if not HF_TOKEN or HF_TOKEN == "":
    print("WARNING: HF_TOKEN is not set. Please set it in .env file or environment variables.")
MODEL_ID = "google/gemma-2-2b-it"
SYSTEM_PROMPT = (
    "Sunteți un asistent expert în extragerea informațiilor din text medical transcrit. "
    "Analizați cu atenție textul și extrageți informațiile solicitate de utilizator. "
    "Răspunsul trebuie să fie STRICT un JSON valid care respectă schema furnizată. "
    "Nu includeți niciun text suplimentar, explicații sau caractere înainte sau după JSON. "
    "IMPORTANT: Toate valorile trebuie să fie STRING-uri simple, NU liste, NU array-uri, NU obiecte. "
    "Dacă există multiple informații pentru un câmp, combinați-le într-un singur string, separând cu virgulă sau punct. "
    "CRITICAL: Când extrageți valoarea pentru un câmp, NU includeți numele câmpului în valoarea extrasă. "
    "Extrageți DOAR textul care apare DUPĂ numele câmpului, fără a include numele câmpului însuși. "
    "De exemplu, dacă textul spune 'Diagnostic, are probleme cu inima', extrageți DOAR 'are probleme cu inima', NU 'Diagnostic, are probleme cu inima'. "
    "Când căutați informații pentru un câmp, căutați variații ale numelui câmpului în text (ignorați diferențe de majuscule/minuscule și variații de formulare). "
    "Exemple de variații acceptate: 'istoricul prezent' = 'istoric bolii prezente' = 'istoricul bolii prezente' = 'istoric prezent'. "
    "Exemple de variații acceptate: 'examinare fizica' = 'examinare fizică' = 'examinare fizica,'. "
    "Dacă un câmp apare în text urmat de virgulă sau două puncte, extrageți tot textul care urmează până la următorul câmp sau până la sfârșitul propoziției. "
    "Extrageți EXACT textul care apare după numele câmpului (fără numele câmpului), chiar dacă pare neclar sau conține erori. "
    "Valorile extrase trebuie să includă întotdeauna unitatea de măsură când este relevant (de exemplu, '10 milimetri', '40kg', '9cm', '2 bete', '3 paduri'). "
    "Dacă nu se găsesc informații pentru o cheie, utilizați exact valoarea 'null'. "
    "Adauga unitatile de masura utilizate in Romania pentru fiecare dimensiune extrasa. Exemplu: '10 milimetri', '20 centimetri', '30 metri', '40 mm', '50 cm', '60 m', '40kg'. "
    "Exemplu corect: {\"simptome\": \"tuse seacă, durere în gât, febră ușoară\"} - NU {\"simptome\": [\"tuse seacă\", \"durere în gât\"]}"
)

FORM_SCHEMAS = {
    "echocardiography": {
        "aorta la inel": "dimensiunea in mm a aortei la inel",
        "aorta la sinusur levart sagva": "dimensiunea in mm a aortei la sinusur levart sagva",
        "aorta ascendenta": "dimensiunea in mm a aortei ascendente",
        "ventricul drept": "dimensiunea in mm a ventriculului drept",
        "atriu stang": "dimensiunea in mm a atriului stang",
        "as": "dimensiunea in mm a atriului stang (prescurtat)",
        "vd": "dimensiunea in mm a ventriculului drept (prescurtat)"
    },
    "medical-report": {
        "plangere principala": "plângerea principală a pacientului, motivul principal pentru consultație",
        "istoricul prezent": "istoricul bolii prezente, descrierea detaliată a simptomelor și evoluției",
        "examinare fizica": "rezultatele examinării fizice, observațiile clinice",
        "diagnostic": "diagnosticul medical stabilit",
        "tratament": "planul de tratament recomandat",
        "recomandari": "recomandări pentru urmărire și monitorizare"
    },
    "consultation-form": {
        "simptome": "simptomele prezente și plângerile pacientului",
        "semne vitale": "semnele vitale măsurate (tensiune arterială, temperatură, puls, etc.)",
        "evaluare": "evaluarea clinică și concluziile medicale",
        "plan": "planul de tratament și urmărire"
    },
    "prescription-form": {
        "medicamente": "numele medicamentelor prescrise",
        "dozaj": "dozajul și frecvența de administrare",
        "instructiuni": "instrucțiuni speciale și avertismente",
        "urmarire": "instrucțiuni pentru urmărire și control"
    },
    "first-time-new-patient": {
        "nume pacient": "numele complet al pacientului",
        "data nasterii": "data nașterii pacientului",
        "gen": "genul pacientului",
        "informatii contact": "informații de contact (telefon, email, contact de urgență)",
        "plangere principala": "plângerea principală, motivul consultației",
        "istoricul prezent": "istoricul bolii prezente",
        "istoric medical trecut": "istoricul medical anterior (boli, intervenții chirurgicale, spitalizări)",
        "medicamente": "medicamentele curente și dozajele",
        "alergii": "alergiile cunoscute (medicamente, alimente, mediu)",
        "istoric familial": "istoricul medical familial relevant",
        "istoric social": "istoricul social (fumat, alcool, ocupație, factori de stil de viață)",
        "semne vitale": "semnele vitale măsurate",
        "examinare fizica": "găsirile examinării fizice",
        "evaluare": "impresia clinică și diagnosticul de lucru",
        "plan": "teste diagnostice, medicamente, instrucțiuni de urmărire",
        "urmarire": "când să revină, semne de alarmă de urmărit, modificări de stil de viață"
    }
}

class TranscriptionResponse(BaseModel):
    text: str

class ParsedRecordingResponse(BaseModel):
    raw_transcript: str
    parsed_json: dict

def safe_encode_str(s):
    """Safely encode a string to ASCII, replacing problematic characters"""
    try:
        if isinstance(s, bytes):
            s = s.decode('utf-8', errors='replace')
        return str(s).encode('ascii', 'replace').decode('ascii')
    except:
        return repr(s)

def safe_print(msg):
    """Safely print a message, handling Unicode encoding"""
    try:
        safe_msg = safe_encode_str(msg)
        print(safe_msg)
    except:
        print(repr(msg))

# Helper to safely extract exception info without calling str() or repr() on exception object
def extract_exception_info(exc):
    """Extract exception information without calling str() or repr() on exception to avoid encoding issues"""
    exc_type = type(exc).__name__
    exc_args = []
    
    try:
        # Try to get args safely - NEVER call str() or repr() on the exception object itself
        if hasattr(exc, 'args') and exc.args:
            for arg in exc.args:
                try:
                    # Try to encode each arg
                    if isinstance(arg, str):
                        exc_args.append(safe_encode_str(arg))
                    elif isinstance(arg, bytes):
                        exc_args.append(safe_encode_str(arg.decode('utf-8', errors='replace')))
                    else:
                        # For non-string args, try to convert safely
                        # But wrap in try-except to catch encoding errors
                        try:
                            # Try str() but catch encoding errors
                            arg_str = str(arg)
                            exc_args.append(safe_encode_str(arg_str))
                        except UnicodeEncodeError:
                            # If str() fails due to encoding, try repr() on the arg (not the exception)
                            try:
                                arg_repr = repr(arg)
                                exc_args.append(safe_encode_str(arg_repr))
                            except:
                                exc_args.append("(unable to encode arg)")
                        except Exception:
                            # Any other error converting arg
                            exc_args.append("(unable to convert arg)")
                except Exception:
                    # If anything fails, just skip this arg
                    exc_args.append("(error encoding arg)")
    except Exception:
        # If we can't even access args, that's okay
        pass
    
    # Build message from parts - NEVER use repr(exc) as it might call str(exc) internally
    if exc_args:
        msg = ' '.join(exc_args)
    else:
        msg = exc_type
    
    return exc_type, msg

def transcribe_audio_bytes_ro(
    audio_content: bytes, 
    content_type: str
) -> TranscriptionResponse:
    
    DEEPGRAM_API_KEY = os.getenv("DEEPGRAM_API_KEY", "4700b39b6fffa69b829bc2510b31859a60081fe3")
    
    # Validare API key
    if not DEEPGRAM_API_KEY or DEEPGRAM_API_KEY.strip() == "":
        raise RuntimeError("Deepgram API key nu este configurat. Vă rugăm să setați DEEPGRAM_API_KEY în fișierul .env sau în variabilele de mediu.")
    
    # Initialize DeepgramClient
    safe_print(f"Initializing DeepgramClient with API key (length: {len(DEEPGRAM_API_KEY)})")
    safe_print(f"API key starts with: {DEEPGRAM_API_KEY[:10]}...")
    
    deepgram_client = None
    try:
        # Initialize with explicit keyword argument
        deepgram_client = DeepgramClient(api_key=DEEPGRAM_API_KEY)
        safe_print("[SUCCESS] DeepgramClient initialized successfully")
    except Exception as init_error:
        error_type, error_msg = extract_exception_info(init_error)
        safe_print(f"[ERROR] Failed to initialize DeepgramClient: {error_type} - {error_msg}")
        raise RuntimeError(f"Eroare la inițializarea Deepgram client: {error_msg}. Verificați că DEEPGRAM_API_KEY este valid și că aveți conexiune la internet.")

    # Call Deepgram API
    try:
        safe_print("Calling Deepgram API...")
        response = deepgram_client.listen.v1.media.transcribe_file(
            request=audio_content,
            smart_format=True,
            model="whisper",
            language="ro"
        )
        
        safe_print("Deepgram API call successful, extracting transcript...")
        
        # Safely extract transcript, handling potential None values
        try:
            transcript_text = response.results.channels[0].alternatives[0].transcript
        except (AttributeError, IndexError, KeyError) as e:
            safe_print("Warning: Could not extract transcript from Deepgram response")
            raise RuntimeError("Nu s-a putut extrage transcrierea din răspunsul API. Vă rugăm să încercați din nou.")
        
        # Handle None or missing transcript
        if transcript_text is None:
            safe_print("Warning: None transcript received from Deepgram")
            raise RuntimeError("Nu s-a detectat niciun vorbire în înregistrare. Vă rugăm să înregistrați din nou.")
        
        # Validate transcript - check if it's empty or contains generic placeholder text
        transcript_text = str(transcript_text).strip()
        if not transcript_text:
            safe_print("Warning: Empty transcript received from Deepgram")
            raise RuntimeError("Nu s-a detectat niciun vorbire în înregistrare. Vă rugăm să înregistrați din nou.")
        
        # Check for common generic/placeholder phrases that indicate unclear or no speech
        transcript_lower = transcript_text.lower()
        generic_phrases = [
            "vă mulțumesc",
            "mulțumesc pentru",
            "thank you",
            "thanks for",
            "pentru vizionare",
            "for watching",
            "pentru atenție",
            "for your attention",
            "mulțumesc frumos"
        ]
        
        # If transcript only contains generic phrases and is short, it's likely invalid
        if any(phrase in transcript_lower for phrase in generic_phrases) and len(transcript_text) < 50:
            safe_print(f"Warning: Generic placeholder transcript detected: {transcript_text[:100]}")
            raise RuntimeError("Vorbirea nu a putut fi recunoscută clar sau înregistrarea conține doar zgomot. Vă rugăm să vorbiți clar despre măsurătorile ecocardiografice și să înregistrați din nou.")
        
        # Check if transcript is too short (likely noise or unclear)
        if len(transcript_text) < 10:
            safe_print(f"Warning: Very short transcript: {transcript_text}")
            raise RuntimeError("Înregistrarea este prea scurtă sau neclară. Vă rugăm să înregistrați din nou.")
        
        safe_print(f"Transcript validated successfully: {len(transcript_text)} characters")
        return TranscriptionResponse(text=transcript_text)
        
    except RuntimeError as e:
        # If it's already a RuntimeError (like our validation errors), preserve it with UTF-8
        # Only encode to ASCII for console printing
        if hasattr(e, 'args') and e.args and isinstance(e.args[0], str):
            error_msg_utf8 = e.args[0]
            error_msg_ascii = safe_encode_str(error_msg_utf8)
            safe_print(f"RuntimeError: {error_msg_ascii}")
            raise  # Re-raise the original RuntimeError with UTF-8 message
        else:
            # Fallback if structure is different
            error_type, error_msg = extract_exception_info(e)
            safe_print(f"RuntimeError: {error_msg}")
            raise RuntimeError(error_msg)
    except Exception as e:
        # Extract exception info safely without calling str()
        error_type, error_msg = extract_exception_info(e)
        
        safe_print(f"Deepgram API Error - Type: {error_type}")
        
        # Check if it's a Deepgram-related error
        if "deepgram" in error_msg.lower() or "api" in error_msg.lower() or "deepgram" in error_type.lower():
            raise RuntimeError(f"Deepgram API Error during transcription: {error_msg}")
        else:
            raise RuntimeError(f"An unexpected error occurred during transcription: {error_msg}")

def extract_value_after_field(transcript: str, field_end_pos: int, all_field_names: list) -> str:
    """
    Extract the value (number + unit) immediately after a field name.
    Stops when it finds another field name or extracts a complete value.
    """
    number_words = {
        'zero': '0', 'unu': '1', 'una': '1', 'doi': '2', 'două': '2',
        'trei': '3', 'patru': '4', 'cinci': '5', 'șase': '6', 'sase': '6',
        'șapte': '7', 'sapte': '7', 'opt': '8', 'nouă': '9', 'noua': '9',
        'zece': '10', 'unsprezece': '11', 'doisprezece': '12', 'treisprezece': '13',
        'paisprezece': '14', 'cincisprezece': '15', 'șaisprezece': '16',
        'șaptesprezece': '17', 'optsprezece': '18', 'nouăsprezece': '19', 'nouasprezece': '19',
        'douăzeci': '20', 'douazeci': '20'
    }
    
    units = ['milimetri', 'centimetri', 'metri', 'mm', 'cm', 'm']
    
    remaining_text = transcript[field_end_pos:].strip()
    remaining_lower = remaining_text.lower()
    
    next_field_pos = len(remaining_text)
    normalized_all_fields = [f.lower() for f in all_field_names]
    
    for field_name in normalized_all_fields:
        field_pos = remaining_lower.find(field_name)
        if field_pos != -1 and field_pos < next_field_pos:
            next_field_pos = field_pos
    
    search_text = remaining_text[:next_field_pos].strip()
    search_lower = search_text.lower()
    
    pattern1 = r'(\d+(?:[.,]\d+)?)\s*(milimetri|centimetri|metri|mm|cm|m)\b'
    match1 = re.search(pattern1, search_lower, re.IGNORECASE)
    if match1:
        original_match = re.search(pattern1, search_text, re.IGNORECASE)
        if original_match:
            value = original_match.group(0).strip()
            parts = value.split()
            if len(parts) >= 2:
                if parts[1].lower() in units or parts[1].lower().replace('mm', '').replace('cm', '').replace('m', '') == '':
                    return f"{parts[0]} {parts[1]}"
            return value
    
    for word, digit in number_words.items():
        pattern2 = rf'\b{re.escape(word)}\s+(milimetri|centimetri|metri|mm|cm|m)\b'
        match2 = re.search(pattern2, search_lower)
        if match2:
            original_match = re.search(pattern2, search_text, re.IGNORECASE)
            if original_match:
                value = original_match.group(0).strip()
                parts = value.split()
                if len(parts) >= 2 and parts[0].lower() in number_words:
                    return f"{number_words[parts[0].lower()]} {parts[1]}"
                return value
    
    pattern3 = r'\b(\d+(?:[.,]\d+)?)\b'
    match3 = re.search(pattern3, search_lower)
    if match3:
        original_match = re.search(pattern3, search_text, re.IGNORECASE)
        if original_match:
            num = original_match.group(1)
            num_start = original_match.start()
            
            after_num_text = search_text[original_match.end():original_match.end()+10].strip()
            unit_match = re.search(r'\b(milimetri|centimetri|metri|mm|cm|m)\b', after_num_text, re.IGNORECASE)
            
            if unit_match:
                return f"{num} {unit_match.group(0)}"
            else:
                if num_start < 30:
                    return num
    
    first_part = search_lower[:30] if len(search_lower) > 30 else search_lower
    for word, digit in number_words.items():
        pattern4 = rf'\b{re.escape(word)}\b'
        match4 = re.search(pattern4, first_part)
        if match4:
            word_end_in_full = match4.end()
            after_word = search_lower[word_end_in_full:word_end_in_full+15].strip()
            unit_match = re.search(r'\b(milimetri|centimetri|metri|mm|cm|m)\b', after_word)
            if unit_match:
                return f"{digit} {unit_match.group(0)}"
            else:
                return digit
    
    return ""

def extract_strict_json(response_text):
    """Extract JSON from response text, handling cases where JSON is wrapped in text"""
    if not response_text:
        return None
    try:
        return json.loads(response_text)
    except json.JSONDecodeError:
        match = re.search(r"\{.*\}", response_text, re.DOTALL)
        if match:
            try:
                return json.loads(match.group())
            except json.JSONDecodeError:
                return None
        else:
            return None

def extract_data_with_api(model_id, text_to_analyze, form_schema, hf_token):
    api_key_to_use = HF_TOKEN
    
    if not api_key_to_use:
        print("ERROR: Hugging Face Token (hf_token) is missing or not set in environment variables.")
        return "Extraction failed: Missing API Key."
    
    user_content = (
        f"""
Vă rugăm să extrageți informațiile din textul următor pe baza schemei JSON furnizate.

TEXT TRANSCRIS:
"{text_to_analyze}"

SCHEMA JSON (cheile sunt numele câmpurilor, valorile sunt descrieri):
{form_schema}

INSTRUCȚIUNI CRITICE:
1. Căutați în text variații ale numelor câmpurilor (ignorați diferențe de majuscule/minuscule și variații de formulare)
   - Exemple: "istoricul prezent" = "istoric bolii prezente" = "istoricul bolii prezente" = "istoric prezent"
   - Exemple: "examinare fizica" = "examinare fizică" = "examinare fizica,"
   - Exemple: "plangere principala" = "plângerea principală" = "plangerea principala"
2. Extrageți DOAR textul care apare DUPĂ numele câmpului, FĂRĂ a include numele câmpului în valoarea extrasă
   - Dacă textul spune "Diagnostic, are probleme cu inima", extrageți DOAR "are probleme cu inima"
   - Dacă textul spune "Plan de tratament, 7 pastile", extrageți DOAR "7 pastile"
   - Dacă textul spune "Examinare fizica, 40kg, 2 bete", extrageți DOAR "40kg, 2 bete"
3. Extrageți tot textul care apare după numele câmpului până la următorul câmp sau sfârșitul propoziției
4. Păstrați textul exact așa cum apare, chiar dacă conține erori sau pare neclar
5. Dacă un câmp nu este găsit în text, folosiți "null"

JSON OUTPUT (doar JSON, fără text suplimentar):
"""
    )
    
    messages = [
        {"role": "system", "content": SYSTEM_PROMPT},
        {"role": "user", "content": user_content}
    ]
    
    payload = {
        "model": model_id,
        "messages": messages,
        "max_tokens": 512,
        "temperature": 0.0
    }
    
    try:
        with httpx.Client() as client:
            response = client.post(
                "https://router.huggingface.co/v1/chat/completions",
                headers={
                    "Authorization": f"Bearer {api_key_to_use}",
                    "Content-Type": "application/json"
                },
                json=payload,
                timeout=30.0
            )
            response.raise_for_status()
            result = response.json()
            response_text = result["choices"][0]["message"]["content"]
            return response_text
    
    except Exception as e:
        return f"Extraction failed due to API error: {e}"



check_and_init_db()

app = FastAPI(
    title="Speech-to-Text Medical System API",
    description="Backend API for managing patients and medical documents",
    version="1.0.0"
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=[
        "http://localhost:5173",  
        "http://localhost:5174",
        "http://127.0.0.1:5173",  
        "http://127.0.0.1:5174"
    ],
    allow_credentials=False,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.include_router(patients.router, prefix="/api/patients", tags=["patients"])
app.include_router(new_patient_forms.router, prefix="/api/new-patient-forms", tags=["new-patient-forms"])
app.include_router(medical_reports.router, prefix="/api/medical-reports", tags=["medical-reports"])
app.include_router(consultation_forms.router, prefix="/api/consultation-forms", tags=["consultation-forms"])
app.include_router(prescription_forms.router, prefix="/api/prescription-forms", tags=["prescription-forms"])

@app.post("/api/process-recording", response_model=ParsedRecordingResponse, tags=["transcription"])
async def process_recording_endpoint(
    audio_file: UploadFile = File(...),
    fields_json: str = Form(...),
    form_type: str = Form(None)
):
    
    try:
        fields_data = json.loads(fields_json)
        target_fields = fields_data.get("fields", [])
        
        if not target_fields or not isinstance(target_fields, list):
            raise HTTPException(
                status_code=400, 
                detail="Invalid 'fields' array provided in JSON string."
            )
            
    except json.JSONDecodeError:
        raise HTTPException(
            status_code=400,
            detail="The 'fields_json' parameter must be a valid JSON string."
        )

    try:
        audio_content = await audio_file.read()
        
        safe_print(f"Processing audio file: {audio_file.filename}, size: {len(audio_content)} bytes")
        safe_print(f"Content type: {audio_file.content_type}")
        safe_print(f"Form type: {form_type}")
        
        transcript_response = transcribe_audio_bytes_ro(
            audio_content,
            audio_file.content_type
        )
        raw_transcript = transcript_response.text
        safe_print(f"Transcription successful, length: {len(raw_transcript)} chars")

        if form_type and form_type in FORM_SCHEMAS:
            predefined_schema = FORM_SCHEMAS[form_type]
            form_schema_dict = {}
            for field in target_fields:
                if field in predefined_schema:
                    form_schema_dict[field] = predefined_schema[field]
                else:
                    form_schema_dict[field] = f"valoarea pentru {field}"
            safe_print(f"Using predefined schema for form type: {form_type}")
        else:
            form_schema_dict = {}
            for field in target_fields:
                form_schema_dict[field] = f"dimensiunea in mm pentru {field}"
            safe_print("Using dynamically generated schema")
        
        form_schema = json.dumps(form_schema_dict, indent=2, ensure_ascii=False)
        
        response_text = extract_data_with_api(MODEL_ID, raw_transcript, form_schema, HF_TOKEN)
        
        parsed_json = extract_strict_json(response_text)
        
        if parsed_json is None:
            parsed_json = {field: "" for field in target_fields}
        else:
            result_dict = {}
            for field in target_fields:
                value = parsed_json.get(field)
                if value is None or value == "null":
                    result_dict[field] = ""
                elif isinstance(value, list):
                    result_dict[field] = ", ".join(str(item) for item in value)
                elif isinstance(value, dict):
                    result_dict[field] = str(value)
                else:
                    value_str = str(value)
                    field_lower = field.lower()
                    value_lower = value_str.lower()
                    
                    if value_lower.startswith(field_lower):
                        remaining = value_str[len(field):].strip()
                        if remaining.startswith(','):
                            remaining = remaining[1:].strip()
                        if remaining.startswith(':'):
                            remaining = remaining[1:].strip()
                        result_dict[field] = remaining
                    else:
                        variations = [
                            field.replace("ul ", " ").replace("ului ", " "),
                            field.replace("ul ", "").replace("ului ", ""),
                            field.split()[-1] if len(field.split()) > 1 else field
                        ]
                        for variation in variations:
                            var_lower = variation.lower()
                            if value_lower.startswith(var_lower):
                                remaining = value_str[len(variation):].strip()
                                if remaining.startswith(','):
                                    remaining = remaining[1:].strip()
                                if remaining.startswith(':'):
                                    remaining = remaining[1:].strip()
                                result_dict[field] = remaining
                                break
                        else:
                            result_dict[field] = value_str
            parsed_json = result_dict
        
        return ParsedRecordingResponse(
            raw_transcript=raw_transcript,
            parsed_json=parsed_json
        )
    
    except RuntimeError as e:
        # For RuntimeError, extract the message directly (it's already in Romanian and safe)
        # RuntimeError messages are user-facing, so preserve UTF-8 encoding for HTTP responses
        # Only encode to ASCII for console printing
        if hasattr(e, 'args') and e.args and isinstance(e.args[0], str):
            # Preserve original UTF-8 string for HTTP response
            error_msg_utf8 = e.args[0]
            # Use ASCII-encoded version only for console
            error_msg_ascii = safe_encode_str(error_msg_utf8)
            safe_print(f"Processing failed: {error_msg_ascii}")
            raise HTTPException(
                status_code=500,
                detail=error_msg_utf8  # Use UTF-8 version for HTTP response
            )
        else:
            # Fallback to extract_exception_info if structure is different
            error_type, error_msg = extract_exception_info(e)
            safe_print(f"Processing failed: {error_type}")
            raise HTTPException(
                status_code=500,
                detail=error_msg
            )
    except Exception as e:
        # Extract exception info safely without calling str()
        error_type, error_msg = extract_exception_info(e)
        safe_print(f"An unexpected error occurred: {error_type}")
        raise HTTPException(
            status_code=500,
            detail=f"An unexpected error occurred: {error_msg}"  # Use message directly
        )


@app.get("/")
def read_root():
    return {
        "message": "Speech-to-Text Medical System API",
        "version": "1.0.0",
        "docs": "/docs"
    }


@app.get("/health")
def health_check():
    return {"status": "healthy"}