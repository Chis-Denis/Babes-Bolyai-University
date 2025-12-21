<template>
  <div class="medical-report">
    <div class="document-header">
      <h2>Raport Medical</h2>
      <div class="report-info">
        <div class="info-item">
          <label>Data:</label>
          <span>{{ patientData.date }}</span>
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
        <strong>💡 Sfat:</strong> Spuneți câmpurile și valorile în română, de exemplu: "plângere principală durere de cap, istoricul prezent febră de 3 zile, examinare fizică temperatura 38 grade"
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

    <div class="form-sections">
      <!-- Chief Complaint -->
      <div class="form-section">
        <h3>Plângere Principală</h3>
        <div class="field-container">
          <label>Motivul principal al vizitei:</label>
          <div class="input-group">
            <textarea 
              v-model="localData.chiefComplaint"
              placeholder="Introduceți sau înregistrați plângerea principală..."
              rows="3"
              @input="updateField('chiefComplaint', $event.target.value)"
              class="form-input"
            ></textarea>
            <span v-if="parsedData && parsedData['plangere principala'] && parsedData['plangere principala'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
              ✓
            </span>
          </div>
        </div>
      </div>

      <!-- History of Present Illness -->
      <div class="form-section">
        <h3>Istoricul Bolii Prezente</h3>
        <div class="field-container">
          <label>Istoric detaliat și simptome:</label>
          <div class="input-group">
            <textarea 
              v-model="localData.historyOfPresentIllness"
              placeholder="Introduceți sau înregistrați istoricul bolii prezente..."
              rows="4"
              @input="updateField('historyOfPresentIllness', $event.target.value)"
              class="form-input"
            ></textarea>
            <span v-if="parsedData && parsedData['istoricul prezent'] && parsedData['istoricul prezent'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
              ✓
            </span>
          </div>
        </div>
      </div>

      <!-- Physical Examination -->
      <div class="form-section">
        <h3>Examinare Fizică</h3>
        <div class="field-container">
          <label>Găsiri la examinare:</label>
          <div class="input-group">
            <textarea 
              v-model="localData.physicalExamination"
              placeholder="Introduceți sau înregistrați găsirile examinării fizice..."
              rows="4"
              @input="updateField('physicalExamination', $event.target.value)"
              class="form-input"
            ></textarea>
            <span v-if="parsedData && parsedData['examinare fizica'] && parsedData['examinare fizica'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
              ✓
            </span>
          </div>
        </div>
      </div>

      <!-- Diagnosis -->
      <div class="form-section">
        <h3>Diagnostic</h3>
        <div class="field-container">
          <label>Diagnostic medical:</label>
          <div class="input-group">
            <textarea 
              v-model="localData.diagnosis"
              placeholder="Introduceți sau înregistrați diagnosticul..."
              rows="3"
              @input="updateField('diagnosis', $event.target.value)"
              class="form-input"
            ></textarea>
            <span v-if="parsedData && parsedData['diagnostic'] && parsedData['diagnostic'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
              ✓
            </span>
          </div>
        </div>
      </div>

      <!-- Treatment -->
      <div class="form-section">
        <h3>Plan de Tratament</h3>
        <div class="field-container">
          <label>Tratament recomandat:</label>
          <div class="input-group">
            <textarea 
              v-model="localData.treatment"
              placeholder="Introduceți sau înregistrați planul de tratament..."
              rows="4"
              @input="updateField('treatment', $event.target.value)"
              class="form-input"
            ></textarea>
            <span v-if="parsedData && parsedData['tratament'] && parsedData['tratament'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
              ✓
            </span>
          </div>
        </div>
      </div>

      <!-- Recommendations -->
      <div class="form-section">
        <h3>Recomandări</h3>
        <div class="field-container">
          <label>Recomandări pentru urmărire:</label>
          <div class="input-group">
            <textarea 
              v-model="localData.recommendations"
              placeholder="Introduceți sau înregistrați recomandările..."
              rows="3"
              @input="updateField('recommendations', $event.target.value)"
              class="form-input"
            ></textarea>
            <span v-if="parsedData && parsedData['recomandari'] && parsedData['recomandari'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
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
    required: true
  }
})

const emit = defineEmits(['field-update'])

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

const recordingDuration = ref(0)
let recordingInterval = null

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

const formFields = [
  {
    key: 'chiefComplaint',
    romanianName: 'plangere principala'
  },
  {
    key: 'historyOfPresentIllness',
    romanianName: 'istoricul prezent'
  },
  {
    key: 'physicalExamination',
    romanianName: 'examinare fizica'
  },
  {
    key: 'diagnosis',
    romanianName: 'diagnostic'
  },
  {
    key: 'treatment',
    romanianName: 'tratament'
  },
  {
    key: 'recommendations',
    romanianName: 'recomandari'
  }
]

const localData = reactive({
  chiefComplaint: props.patientData.chiefComplaint || '',
  historyOfPresentIllness: props.patientData.historyOfPresentIllness || '',
  physicalExamination: props.patientData.physicalExamination || '',
  diagnosis: props.patientData.diagnosis || '',
  treatment: props.patientData.treatment || '',
  recommendations: props.patientData.recommendations || ''
})

watch(() => props.patientData, (newData) => {
  Object.assign(localData, {
    chiefComplaint: newData.chiefComplaint || '',
    historyOfPresentIllness: newData.historyOfPresentIllness || '',
    physicalExamination: newData.physicalExamination || '',
    diagnosis: newData.diagnosis || '',
    treatment: newData.treatment || '',
    recommendations: newData.recommendations || ''
  })
}, { deep: true })

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
      await processAudio(fieldList, 'medical-report')
    } catch (err) {
      console.error('Error auto-processing audio:', err)
    }
  }
})

const clearRecording = () => {
  audioBlob.value = null
  rawTranscript.value = null
  parsedData.value = null
  error.value = null
  recordingDuration.value = 0
}

const formatDuration = (seconds) => {
  const mins = Math.floor(seconds / 60)
  const secs = seconds % 60
  return `${mins.toString().padStart(2, '0')}:${secs.toString().padStart(2, '0')}`
}

const formatFileSize = (bytes) => {
  if (!bytes) return '0 B'
  if (bytes < 1024) return bytes + ' B'
  if (bytes < 1024 * 1024) return (bytes / 1024).toFixed(2) + ' KB'
  return (bytes / (1024 * 1024)).toFixed(2) + ' MB'
}

watch(parsedData, (newParsedData) => {
  if (!newParsedData) return

  formFields.forEach(field => {
    const romanianName = field.romanianName
    const value = newParsedData[romanianName]
    
    if (value !== undefined && value !== null && value !== '') {
      localData[field.key] = value
      updateField(field.key, value)
    }
  })
}, { deep: true, immediate: true })

const updateField = (fieldName, value) => {
  localData[fieldName] = value
  emit('field-update', fieldName, value)
}

onUnmounted(() => {
  if (recordingInterval) {
    clearInterval(recordingInterval)
  }
  cleanup()
})
</script>

<style scoped>
.medical-report {
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
  align-items: flex-start;
  gap: 0.5rem;
}

.form-input {
  flex: 1;
  border: 1px solid #ced4da;
  border-radius: 6px;
  padding: 0.75rem;
  font-size: 0.95rem;
  line-height: 1.5;
  resize: vertical;
  transition: all 0.3s ease;
  font-family: inherit;
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
  margin-top: 0.5rem;
}

@media (max-width: 768px) {
  .medical-report {
    padding: 1rem;
  }
  
  .recording-controls {
    flex-direction: column;
  }
  
  .record-btn {
    width: 100%;
    justify-content: center;
  }
}
</style>
