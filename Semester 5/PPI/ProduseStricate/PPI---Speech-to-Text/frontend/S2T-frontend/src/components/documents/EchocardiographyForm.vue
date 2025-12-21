<template>
  <div class="echocardiography-form">
    <div class="document-header">
      <h2>Ecografie Cardiacă / Echocardiography</h2>
      <div class="report-info">
        <div class="info-item">
          <label>Data:</label>
          <span>{{ patientData.date || new Date().toISOString().split('T')[0] }}</span>
        </div>
        <div class="info-item">
          <label>Pacient:</label>
          <span>{{ patientData.name || 'Selectați Pacient' }}</span>
        </div>
      </div>
    </div>

    <!-- Audio Recording Controls -->
    <div class="recording-section">
      <div class="recording-controls">
        <button
          @click="handleToggleRecording"
          class="record-btn"
          :class="isRecording ? 'stop-btn' : 'start-btn'"
          :disabled="isProcessing"
        >
          <svg v-if="!isRecording" viewBox="0 0 24 24" fill="currentColor" class="mic-icon">
            <path d="M12 14c1.66 0 3-1.34 3-3V5c0-1.66-1.34-3-3-3S9 3.34 9 5v6c0 1.66 1.34 3 3 3z"/>
            <path d="M17 11c0 2.76-2.24 5-5 5s-5-2.24-5-5H5c0 3.53 2.61 6.43 6 6.92V21h2v-3.08c3.39-.49 6-3.39 6-6.92h-2z"/>
          </svg>
          <svg v-else viewBox="0 0 24 24" fill="currentColor" class="stop-icon">
            <rect x="6" y="6" width="12" height="12" rx="2"/>
          </svg>
          {{ isRecording ? 'Oprește Înregistrarea' : 'Începe Înregistrarea' }}
        </button>
        <button
          @click="handleTestWithMockData"
          class="test-btn"
          :disabled="isProcessing || isRecording"
          title="Testează cu date mock pentru a verifica funcționalitatea"
        >
          🧪 Test cu Date Mock
        </button>
      </div>

      <!-- Recording Status -->
      <div v-if="isRecording" class="recording-status">
        <div class="recording-indicator">
          <span class="recording-dot"></span>
          <span>Înregistrare în curs...</span>
        </div>
        <div v-if="recordingDuration > 0" class="recording-duration">
          Durată: {{ formatDuration(recordingDuration) }}
        </div>
      </div>

      <!-- Audio Status -->
      <div v-if="audioBlob && !isRecording" class="audio-status">
        <div class="audio-info">
          <span class="audio-icon">🎵</span>
          <span>Audio înregistrat: {{ formatFileSize(audioBlob.size) }}</span>
        </div>
        <button @click="clearRecording" class="clear-btn" title="Șterge înregistrarea">
          ✕
        </button>
      </div>

      <!-- Recording Tips -->
      <div v-if="!isRecording && !audioBlob" class="recording-tips">
        <strong>💡 Sfat:</strong> Spuneți câmpurile și valorile în română, de exemplu: "aorta la inel 10 milimetri, ventricul drept 10 milimetri"
      </div>

      <!-- Status Messages -->
      <div v-if="error" class="error-message">
        <strong>Eroare:</strong> {{ error }}
        <div v-if="error.includes('microphone')" class="error-help">
          Asigurați-vă că ați acordat permisiuni pentru microfon în browser.
        </div>
        <div v-if="error.includes('Processing failed')" class="error-help">
          Verificați că serverul backend rulează și că Deepgram API key este configurat corect.
        </div>
      </div>
      <div v-if="isProcessing" class="processing-message">
        <div class="spinner"></div>
        <span>Se procesează audio-ul... Vă rugăm așteptați.</span>
      </div>
      <div v-if="rawTranscript" class="transcript-preview">
        <strong>Transcriere:</strong>
        <p>{{ rawTranscript }}</p>
      </div>
    </div>

    <!-- Form Fields -->
    <div class="form-sections">
      <div class="form-section">
        <h3>Măsurători Ecocardiografice</h3>
        
        <div class="field-container" v-for="field in formFields" :key="field.key">
          <label :for="field.key">{{ field.label }}</label>
          <div class="input-group">
            <input
              :id="field.key"
              type="text"
              v-model="formData[field.key]"
              :placeholder="field.placeholder"
              class="form-input"
            />
            <span v-if="parsedData && parsedData[field.romanianName] && parsedData[field.romanianName] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
              ✓
            </span>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, reactive, watch, onUnmounted } from 'vue'
import { useAudioProcessor } from '../../composables/useAudioProcessor.js'

const props = defineProps({
  patientData: {
    type: Object,
    default: () => ({})
  }
})

const emit = defineEmits(['field-update'])

// Use the composable
const {
  isRecording,
  isProcessing,
  audioBlob,
  rawTranscript,
  parsedData,
  error,
  startRecording,
  stopRecording,
  processAudio,
  cleanup
} = useAudioProcessor()

// Recording duration tracking
const recordingDuration = ref(0)
let recordingInterval = null

// Watch isRecording to start/stop duration timer
watch(isRecording, (recording) => {
  if (recording) {
    recordingDuration.value = 0
    recordingInterval = setInterval(() => {
      recordingDuration.value++
    }, 1000)
  } else {
    if (recordingInterval) {
      clearInterval(recordingInterval)
      recordingInterval = null
    }
  }
})

// Form fields based on mock data structure
const formFields = [
  {
    key: 'aorta_la_inel',
    label: 'Aorta la Inel',
    placeholder: 'ex: 10 milimetri',
    romanianName: 'aorta la inel'
  },
  {
    key: 'aorta_la_sinusur_levart_sagva',
    label: 'Aorta la Sinusur Levart Sagva',
    placeholder: 'ex: 15 milimetri',
    romanianName: 'aorta la sinusur levart sagva'
  },
  {
    key: 'aorta_ascendenta',
    label: 'Aorta Ascendentă',
    placeholder: 'ex: 14 milimetri',
    romanianName: 'aorta ascendenta'
  },
  {
    key: 'as',
    label: 'AS (Aorta Ascendentă)',
    placeholder: 'ex: 14 milimetri',
    romanianName: 'as'
  },
  {
    key: 'ventricul_drept',
    label: 'Ventricul Drept',
    placeholder: 'ex: 10 milimetri',
    romanianName: 'ventricul drept'
  },
  {
    key: 'atriu_stang',
    label: 'Atriu Stâng',
    placeholder: 'ex: 12 milimetri',
    romanianName: 'atriu stang'
  },
  {
    key: 'vd',
    label: 'VD (Ventricul Drept)',
    placeholder: 'ex: 10 milimetri',
    romanianName: 'vd'
  }
]

// Form data
const formData = reactive({
  aorta_la_inel: '',
  aorta_la_sinusur_levart_sagva: '',
  aorta_ascendenta: '',
  as: '',
  ventricul_drept: '',
  atriu_stang: '',
  vd: ''
})

const getRomanianFieldNames = () => {
  return formFields.map(field => field.romanianName)
}

const shouldAutoProcess = ref(false)

const handleToggleRecording = async () => {
  if (isRecording.value) {
    shouldAutoProcess.value = true
    stopRecording()
  } else {
    shouldAutoProcess.value = false
    try {
      await startRecording()
    } catch (err) {
      console.error('Failed to start recording:', err)
    }
  }
}

watch([audioBlob, isRecording], async ([newBlob, recording]) => {
  if (!recording && newBlob && shouldAutoProcess.value) {
    shouldAutoProcess.value = false
    const fieldList = getRomanianFieldNames()
    try {
      await processAudio(fieldList, 'echocardiography')
    } catch (err) {
      console.error('Error auto-processing audio:', err)
    }
  }
})

// Clear recording
const clearRecording = () => {
  audioBlob.value = null
  rawTranscript.value = null
  parsedData.value = null
  error.value = null
  recordingDuration.value = 0
}

// Format duration (seconds to MM:SS)
const formatDuration = (seconds) => {
  const mins = Math.floor(seconds / 60)
  const secs = seconds % 60
  return `${mins.toString().padStart(2, '0')}:${secs.toString().padStart(2, '0')}`
}

// Format file size
const formatFileSize = (bytes) => {
  if (!bytes) return '0 B'
  if (bytes < 1024) return bytes + ' B'
  if (bytes < 1024 * 1024) return (bytes / 1024).toFixed(2) + ' KB'
  return (bytes / (1024 * 1024)).toFixed(2) + ' MB'
}

// Handle audio processing
const handleProcessAudio = async () => {
  const fieldList = getRomanianFieldNames()
  try {
    await processAudio(fieldList, 'echocardiography')
  } catch (err) {
    console.error('Error processing audio:', err)
  }
}

// Handle test with mock data
const handleTestWithMockData = () => {
  // Mock data as provided by the user
  const mockParsedData = {
    "aorta la inel": "10 milimetri",
    "aorta la sinusur levart sagva": "15 milimetri",
    "aorta ascendenta": "",
    "as": "cendenta 14 milimetri",
    "ventricul drept": "10 milimetri",
    "atriu stang": "12 milimetri pot să le dictești și prescurtat adică mă rog aorta la inel și asta cu aorta nu dar după aia as 14 milimetri",
    "vd": "10 milimetri no cam asa ne auzim multumesc"
  }
  
  const mockTranscript = "aorta la inel 10 milimetri, aorta la sinusur levart sagva 15 milimetri, as cendenta 14 milimetri, ventricul drept 10 milimetri, atriu stang 12 milimetri pot să le dictești și prescurtat adică mă rog aorta la inel și asta cu aorta nu dar după aia as 14 milimetri, vd 10 milimetri no cam asa ne auzim multumesc"
  
  rawTranscript.value = mockTranscript
  parsedData.value = mockParsedData
  error.value = null
}

watch(parsedData, (newParsedData) => {
  if (!newParsedData) return

  formFields.forEach(field => {
    const romanianName = field.romanianName
    const value = newParsedData[romanianName]
    
    if (value !== undefined && value !== null && value !== '') {
      formData[field.key] = value
      emit('field-update', field.key, value)
    }
  })
}, { deep: true, immediate: true })

// Watch formData changes and emit updates
watch(formData, (newData) => {
  Object.keys(newData).forEach(key => {
    emit('field-update', key, newData[key])
  })
}, { deep: true })

// Cleanup on unmount
onUnmounted(() => {
  if (recordingInterval) {
    clearInterval(recordingInterval)
  }
  cleanup()
})
</script>

<style scoped>
.echocardiography-form {
  max-width: 900px;
  margin: 0 auto;
  padding: 2rem;
}

.document-header {
  border-bottom: 2px solid #e9ecef;
  padding-bottom: 1.5rem;
  margin-bottom: 2rem;
}

.document-header h2 {
  color: #2c3e50;
  margin-bottom: 1rem;
  font-size: 2rem;
}

.report-info {
  display: flex;
  gap: 2rem;
  flex-wrap: wrap;
}

.info-item {
  display: flex;
  align-items: center;
  gap: 0.5rem;
}

.info-item label {
  font-weight: 600;
  color: #495057;
}

.info-item span {
  color: #6c757d;
  padding: 0.25rem 0.75rem;
  background: #f8f9fa;
  border-radius: 4px;
}

.recording-section {
  background: #f8f9fa;
  border-radius: 8px;
  padding: 1.5rem;
  margin-bottom: 2rem;
}

.recording-controls {
  display: flex;
  gap: 1rem;
  align-items: center;
  flex-wrap: wrap;
}

.record-btn {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  padding: 0.75rem 1.5rem;
  border: none;
  border-radius: 8px;
  font-size: 1rem;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.3s ease;
}

.start-btn {
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  color: white;
}

.start-btn:hover:not(:disabled) {
  transform: translateY(-2px);
  box-shadow: 0 4px 12px rgba(102, 126, 234, 0.3);
}

.stop-btn {
  background: linear-gradient(135deg, #dc3545 0%, #c82333 100%);
  color: white;
  animation: pulse 2s infinite;
}

.stop-btn:hover {
  transform: translateY(-2px);
  box-shadow: 0 4px 12px rgba(220, 53, 69, 0.3);
}

.process-btn {
  padding: 0.75rem 1.5rem;
  background: linear-gradient(135deg, #28a745 0%, #20c997 100%);
  color: white;
  border: none;
  border-radius: 8px;
  font-size: 1rem;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.3s ease;
}

.process-btn:hover:not(:disabled) {
  transform: translateY(-2px);
  box-shadow: 0 4px 12px rgba(40, 167, 69, 0.3);
}

.process-btn:disabled {
  opacity: 0.6;
  cursor: not-allowed;
}

.test-btn {
  padding: 0.75rem 1.5rem;
  background: linear-gradient(135deg, #ffc107 0%, #ff9800 100%);
  color: white;
  border: none;
  border-radius: 8px;
  font-size: 1rem;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.3s ease;
}

.test-btn:hover:not(:disabled) {
  transform: translateY(-2px);
  box-shadow: 0 4px 12px rgba(255, 193, 7, 0.3);
}

.test-btn:disabled {
  opacity: 0.6;
  cursor: not-allowed;
}

.mic-icon,
.stop-icon {
  width: 20px;
  height: 20px;
}

@keyframes pulse {
  0% { box-shadow: 0 0 0 0 rgba(220, 53, 69, 0.4); }
  70% { box-shadow: 0 0 0 10px rgba(220, 53, 69, 0); }
  100% { box-shadow: 0 0 0 0 rgba(220, 53, 69, 0); }
}

.error-message {
  margin-top: 1rem;
  padding: 1rem;
  background: #f8d7da;
  color: #721c24;
  border-radius: 4px;
  border: 1px solid #f5c6cb;
}

.processing-message {
  margin-top: 1rem;
  padding: 1rem;
  background: #d1ecf1;
  color: #0c5460;
  border-radius: 4px;
  border: 1px solid #bee5eb;
  display: flex;
  align-items: center;
  gap: 0.75rem;
}

.spinner {
  width: 20px;
  height: 20px;
  border: 3px solid #bee5eb;
  border-top-color: #0c5460;
  border-radius: 50%;
  animation: spin 1s linear infinite;
}

@keyframes spin {
  to { transform: rotate(360deg); }
}

.recording-status {
  margin-top: 1rem;
  padding: 1rem;
  background: #fff3cd;
  border-radius: 4px;
  border: 1px solid #ffc107;
}

.recording-indicator {
  display: flex;
  align-items: center;
  gap: 0.75rem;
  font-weight: 600;
  color: #856404;
}

.recording-dot {
  width: 12px;
  height: 12px;
  background: #dc3545;
  border-radius: 50%;
  animation: blink 1s infinite;
}

@keyframes blink {
  0%, 50% { opacity: 1; }
  51%, 100% { opacity: 0.3; }
}

.recording-duration {
  margin-top: 0.5rem;
  font-size: 0.9rem;
  color: #856404;
}

.audio-status {
  margin-top: 1rem;
  padding: 0.75rem 1rem;
  background: #d4edda;
  border-radius: 4px;
  border: 1px solid #c3e6cb;
  display: flex;
  align-items: center;
  justify-content: space-between;
}

.audio-info {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  color: #155724;
  font-weight: 500;
}

.audio-icon {
  font-size: 1.2rem;
}

.clear-btn {
  background: #dc3545;
  color: white;
  border: none;
  border-radius: 4px;
  width: 28px;
  height: 28px;
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 1rem;
  transition: all 0.2s ease;
}

.clear-btn:hover {
  background: #c82333;
  transform: scale(1.1);
}

.recording-tips {
  margin-top: 1rem;
  padding: 0.75rem 1rem;
  background: #e7f3ff;
  border-radius: 4px;
  border: 1px solid #b3d9ff;
  color: #004085;
  font-size: 0.9rem;
}

.error-help {
  margin-top: 0.5rem;
  padding-top: 0.5rem;
  border-top: 1px solid rgba(114, 28, 36, 0.2);
  font-size: 0.9rem;
  font-style: italic;
}

.transcript-preview {
  margin-top: 1rem;
  padding: 1rem;
  background: white;
  border-radius: 4px;
  border: 1px solid #dee2e6;
}

.transcript-preview strong {
  display: block;
  margin-bottom: 0.5rem;
  color: #495057;
}

.transcript-preview p {
  margin: 0;
  color: #6c757d;
  line-height: 1.6;
}

.form-sections {
  display: flex;
  flex-direction: column;
  gap: 2rem;
}

.form-section {
  border: 1px solid #e9ecef;
  border-radius: 8px;
  padding: 1.5rem;
  background: #fafbfc;
}

.form-section h3 {
  color: #2c3e50;
  margin-bottom: 1.5rem;
  font-size: 1.3rem;
  border-bottom: 1px solid #dee2e6;
  padding-bottom: 0.5rem;
}

.field-container {
  margin-bottom: 1.5rem;
}

.field-container label {
  display: block;
  font-weight: 600;
  color: #495057;
  margin-bottom: 0.5rem;
}

.input-group {
  display: flex;
  align-items: center;
  gap: 0.5rem;
}

.form-input {
  flex: 1;
  border: 1px solid #ced4da;
  border-radius: 6px;
  padding: 0.75rem;
  font-size: 0.95rem;
  transition: all 0.3s ease;
}

.form-input:focus {
  outline: none;
  border-color: #667eea;
  box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.1);
}

.parsed-indicator {
  color: #28a745;
  font-size: 1.2rem;
  font-weight: bold;
}

@media (max-width: 768px) {
  .echocardiography-form {
    padding: 1rem;
  }
  
  .recording-controls {
    flex-direction: column;
  }
  
  .record-btn,
  .process-btn {
    width: 100%;
    justify-content: center;
  }
}
</style>

