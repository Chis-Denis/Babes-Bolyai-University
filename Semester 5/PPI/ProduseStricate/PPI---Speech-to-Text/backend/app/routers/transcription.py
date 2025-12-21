"""
OpenAI Speech-to-Text transcription endpoints
"""
from fastapi import APIRouter, UploadFile, File, HTTPException, Form
from openai import OpenAI
import os
import json
import re
from dotenv import load_dotenv
from app.models import TranscriptionResponse, ProcessRecordingResponse

load_dotenv()

router = APIRouter()

def get_openai_client():
    """Get OpenAI client, initializing it only when needed"""
    api_key = os.getenv('OPENAI_API_KEY')
    if not api_key:
        return None
    return OpenAI(api_key=api_key)


@router.post("/transcribe", response_model=TranscriptionResponse)
async def transcribe_audio(audio_file: UploadFile = File(...)):
    """
    Transcribe audio file using OpenAI Whisper
    """
    openai_client = get_openai_client()
    if not openai_client:
        raise HTTPException(
            status_code=503,
            detail="OpenAI API key not configured. Please set OPENAI_API_KEY environment variable."
        )
    
    import tempfile
    temp_file_path = None
    
    try:
        audio_content = await audio_file.read()
        
        with tempfile.NamedTemporaryFile(delete=False, suffix=".webm") as temp_file_obj:
            temp_file_obj.write(audio_content)
            temp_file_path = temp_file_obj.name
        
        filename = audio_file.filename or "recording.webm"
        
        with open(temp_file_path, "rb") as audio_f:
            transcript = openai_client.audio.transcriptions.create(
                model="whisper-1",
                file=audio_f,
                response_format="json"
            )
        
        if os.path.exists(temp_file_path):
            os.remove(temp_file_path)
        
        return TranscriptionResponse(text=transcript.text)
    
    except Exception as e:
        import traceback
        error_trace = traceback.format_exc()
        print(f"Transcription error: {str(e)}")
        print(f"Traceback: {error_trace}")
        
        if temp_file_path and os.path.exists(temp_file_path):
            os.remove(temp_file_path)
        
        raise HTTPException(
            status_code=500,
            detail=f"Transcription failed: {str(e)}"
        )


@router.post("/process-recording", response_model=ProcessRecordingResponse)
async def process_recording(
    audio_file: UploadFile = File(...),
    fields_json: str = Form(...)
):
    """
    Process audio recording with field list and return parsed data.
    Accepts audio file and a JSON string with field names to extract.
    """
    openai_client = get_openai_client()
    if not openai_client:
        raise HTTPException(
            status_code=503,
            detail="OpenAI API key not configured. Please set OPENAI_API_KEY environment variable."
        )
    
    import tempfile
    temp_file_path = None
    
    try:
        try:
            fields_data = json.loads(fields_json)
            field_list = fields_data.get('fields', [])
        except json.JSONDecodeError:
            raise HTTPException(
                status_code=400,
                detail="Invalid fields_json format. Expected JSON with 'fields' array."
            )
        
        if not field_list:
            raise HTTPException(
                status_code=400,
                detail="Field list cannot be empty."
            )
        
        audio_content = await audio_file.read()
        
        with tempfile.NamedTemporaryFile(delete=False, suffix=".webm") as temp_file_obj:
            temp_file_obj.write(audio_content)
            temp_file_path = temp_file_obj.name
        
        with open(temp_file_path, "rb") as audio_f:
            transcript = openai_client.audio.transcriptions.create(
                model="whisper-1",
                file=audio_f,
                response_format="json",
                language="ro"
            )
        
        raw_transcript = transcript.text
        
        parsed_data = parse_transcript_with_openai(openai_client, raw_transcript, field_list)
        
        if os.path.exists(temp_file_path):
            os.remove(temp_file_path)
        
        return ProcessRecordingResponse(
            raw_transcript=raw_transcript,
            parsed_json=parsed_data
        )
    
    except HTTPException:
        raise
    except Exception as e:
        import traceback
        error_trace = traceback.format_exc()
        print(f"Processing error: {str(e)}")
        print(f"Traceback: {error_trace}")
        
        if temp_file_path and os.path.exists(temp_file_path):
            os.remove(temp_file_path)
        
        raise HTTPException(
            status_code=500,
            detail=f"Processing failed: {str(e)}"
        )


def parse_transcript_with_openai(openai_client: OpenAI, transcript: str, field_list: list[str]) -> dict:
    """
    Parse transcript using OpenAI to extract structured field values.
    Falls back to regex-based parsing if OpenAI fails.
    """
    try:
        field_names_str = ', '.join([f'"{field}"' for field in field_list])
        prompt = f"""Extrage valorile pentru următoarele câmpuri din acest text transcrit în română.
Returnează un JSON cu cheile exacte ca în lista de câmpuri și valorile extrase.

Câmpuri: {field_names_str}

Text transcrit:
{transcript}

Returnează doar un JSON valid cu cheile exacte din lista de câmpuri. Dacă un câmp nu este găsit, folosește string gol "".
Format JSON: {{"câmp1": "valoare1", "câmp2": "valoare2", ...}}
"""
        
        response = openai_client.chat.completions.create(
            model="gpt-3.5-turbo",
            messages=[
                {"role": "system", "content": "Ești un asistent care extrage date structurate din text medical în română. Returnezi doar JSON valid."},
                {"role": "user", "content": prompt}
            ],
            temperature=0.1,
            max_tokens=1000
        )
        
        content = response.choices[0].message.content.strip()
        if content.startswith("```"):
            content = content.split("```")[1]
            if content.startswith("json"):
                content = content[4:]
            content = content.strip()
        
        parsed = json.loads(content)
        
        result = {}
        for field in field_list:
            result[field] = parsed.get(field, "")
        
        return result
        
    except Exception as e:
        print(f"OpenAI parsing failed, using fallback: {str(e)}")
        return parse_transcript_for_fields_fallback(transcript, field_list)


def parse_transcript_for_fields_fallback(transcript: str, field_list: list[str]) -> dict:
    """
    Fallback parser using regex patterns.
    """
    parsed = {}
    transcript_lower = transcript.lower()
    
    def normalize_romanian(text: str) -> str:
        """Normalize Romanian text for matching"""
        replacements = {
            'ă': 'a', 'â': 'a', 'î': 'i', 'ș': 's', 'ț': 't',
            'Ă': 'A', 'Â': 'A', 'Î': 'I', 'Ș': 'S', 'Ț': 'T'
        }
        for old, new in replacements.items():
            text = text.replace(old, new)
        return text.lower()
    
    for field in field_list:
        field_normalized = normalize_romanian(field)
        field_lower = field.lower()
        
        patterns = [
            rf"{re.escape(field_lower)}\s*[:]\s*(.+?)(?:\n|$|,|\.|;|și|si)",
            rf"{re.escape(field_lower)}\s+(.+?)(?:\n|$|,|\.|;|și|si)",
            rf"{re.escape(field_normalized)}\s*[:]\s*(.+?)(?:\n|$|,|\.|;|și|si)",
            rf"{re.escape(field_normalized)}\s+(.+?)(?:\n|$|,|\.|;|și|si)",
        ]
        
        value = None
        for pattern in patterns:
            match = re.search(pattern, transcript_lower, re.IGNORECASE | re.MULTILINE)
            if match:
                value = match.group(1).strip()
                value = re.sub(r'\s+', ' ', value)
                if value and value[-1] in [',', '.', ';']:
                    value = value[:-1]
                break
        
        if not value:
            field_index = transcript_lower.find(field_lower)
            if field_index == -1:
                field_index = transcript_lower.find(field_normalized)
            
            if field_index != -1:
                remaining_text = transcript[field_index + len(field):].strip()
                sentences = re.split(r'[.!?;]', remaining_text)
                if sentences:
                    value = sentences[0].strip()
                else:
                    value = remaining_text[:200].strip()
        
        parsed[field] = value if value else ""
    
    return parsed


@router.get("/health")
async def transcription_health():
    """Check if transcription service is available"""
    openai_client = get_openai_client()
    if not openai_client:
        return {
            "status": "unavailable",
            "message": "OpenAI API key not configured"
        }
    return {
        "status": "available",
        "message": "Transcription service is ready"
    }

