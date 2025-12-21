import { ref } from 'vue';
import { appConfig } from '../config/app.config.js';

const API_BASE_URL = 'http://127.0.0.1:8000';
const API_ENDPOINT = `${API_BASE_URL}/api/process-recording`;

export function useAudioProcessor() {
    // State
    const isRecording = ref(false);
    const isProcessing = ref(false);
    const audioBlob = ref(null);
    const rawTranscript = ref(null);
    const parsedData = ref(null);
    const error = ref(null);

    // Internal Refs
    const mediaRecorderRef = ref(null);
    const audioChunks = ref([]);
    let streamRef = null;

    // Recording Logic
    const startRecording = async () => {
        error.value = null;
        rawTranscript.value = null;
        parsedData.value = null;
        audioBlob.value = null;
        audioChunks.value = [];

        try {
            streamRef = await navigator.mediaDevices.getUserMedia({ audio: true });
            mediaRecorderRef.value = new MediaRecorder(streamRef);
            
            mediaRecorderRef.value.ondataavailable = (event) => {
                audioChunks.value.push(event.data);
            };

            mediaRecorderRef.value.onstop = () => {
                const mimeType = mediaRecorderRef.value.mimeType;
                const newAudioBlob = new Blob(audioChunks.value, { type: mimeType });
                audioBlob.value = newAudioBlob;
                
                if (streamRef) {
                    streamRef.getTracks().forEach(track => track.stop());
                }
            };

            mediaRecorderRef.value.start();
            isRecording.value = true;
        } catch (err) {
            console.error("Error accessing microphone:", err);
            error.value = "Could not access microphone. Check permissions.";
            isRecording.value = false;
        }
    };

    const stopRecording = () => {
        if (mediaRecorderRef.value && mediaRecorderRef.value.state === 'recording') {
            mediaRecorderRef.value.stop();
            isRecording.value = false;
        }
    };

    // API Submission Handler (receives fields array and form type)
    const processAudio = async (fieldList, formType = null) => {
        if (!audioBlob.value) {
            error.value = "Please record audio before processing.";
            return;
        }
        if (!fieldList || fieldList.length === 0) {
            error.value = "Field list cannot be empty.";
            return;
        }

        isProcessing.value = true;
        error.value = null;
        rawTranscript.value = null;
        parsedData.value = null;

        try {
            const formData = new FormData();
            formData.append('audio_file', audioBlob.value, 'recording.webm');
            
            const fieldsDataObject = { fields: fieldList };
            formData.append('fields_json', JSON.stringify(fieldsDataObject));
            
            if (formType) {
                formData.append('form_type', formType);
            }

            const response = await fetch(API_ENDPOINT, {
                method: 'POST',
                body: formData,
            });

            if (!response.ok) {
                const errorData = await response.json();
                throw new Error(errorData.detail || `Server error: ${response.status}`);
            }

            const result = await response.json();
            
            rawTranscript.value = result.raw_transcript;
            parsedData.value = result.parsed_json;

        } catch (err) {
            console.error("Processing failed:", err);
            error.value = `Processing failed: ${err.message}`;
        } finally {
            isProcessing.value = false;
        }
    };
    
    // Cleanup function for unmount hook
    const cleanup = () => {
        if (streamRef) {
            streamRef.getTracks().forEach(track => track.stop());
        }
    };

    return {
        // State
        isRecording,
        isProcessing,
        audioBlob,
        rawTranscript,
        parsedData,
        error,

        // Actions
        startRecording,
        stopRecording,
        processAudio,
        cleanup,
    };
}

