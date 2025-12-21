<template>
  <div class="document-template first-time-patient">
    <div class="document-header">
      <h3>Evaluare Pacient Nou</h3>
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
        <strong>💡 Sfat:</strong> Spuneți câmpurile și valorile în română, de exemplu: "informatii contact telefon 0712345678, plangere principala durere de cap, semne vitale tensiune 120 pe 80"
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

    <div class="form-section">
      <!-- Patient Information -->
      <div class="form-group">
        <label for="patientName">Numele Complet al Pacientului</label>
        <div class="input-group">
          <input 
            type="text" 
            id="patientName" 
            :value="formData.patientName"
            @input="updateField('patientName', $event.target.value)"
            :class="{ 'field-locked': isFieldLocked }"
            :disabled="isFieldLocked"
            placeholder="Introduceți numele complet al pacientului"
          />
          <span v-if="parsedData && parsedData['nume pacient'] && parsedData['nume pacient'] !== '' && !isFieldLocked" class="parsed-indicator" title="Valoare extrasă din audio">
            ✓
          </span>
        </div>
      </div>

      <div class="form-group">
        <label for="dateOfBirth">Data Nașterii *</label>
        <div class="input-group">
          <input 
            type="date" 
            id="dateOfBirth" 
            :value="formData.dateOfBirth"
            @input="updateField('dateOfBirth', $event.target.value)"
            :class="{ 'field-locked': isFieldLocked }"
            :disabled="isFieldLocked"
            required
          />
          <span v-if="parsedData && parsedData['data nasterii'] && parsedData['data nasterii'] !== '' && !isFieldLocked" class="parsed-indicator" title="Valoare extrasă din audio">
            ✓
          </span>
        </div>
      </div>

      <div class="form-group">
        <label for="gender">Gen *</label>
        <div class="input-group">
          <select 
            id="gender" 
            :value="formData.gender"
            @change="updateField('gender', $event.target.value)"
            :class="{ 'field-locked': isFieldLocked }"
            :disabled="isFieldLocked"
            required
          >
            <option value="">Selectați Genul</option>
            <option value="Masculin">Masculin</option>
            <option value="Feminin">Feminin</option>
            <option value="Altul">Altul</option>
            <option value="Prefer să nu răspund">Prefer să nu răspund</option>
          </select>
          <span v-if="parsedData && parsedData['gen'] && parsedData['gen'] !== '' && !isFieldLocked" class="parsed-indicator" title="Valoare extrasă din audio">
            ✓
          </span>
        </div>
      </div>

      <div class="form-group">
        <label for="contactInfo">Informații de Contact</label>
        <div class="input-group">
          <textarea 
            id="contactInfo" 
            :value="formData.contactInfo"
            @input="updateField('contactInfo', $event.target.value)"
            placeholder="Număr de telefon, adresă email, contact de urgență"
          ></textarea>
          <span v-if="parsedData && parsedData['informatii contact'] && parsedData['informatii contact'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
            ✓
          </span>
        </div>
      </div>

      <!-- Medical History -->
      <div class="form-group">
        <label for="chiefComplaint">Plângere Principală</label>
        <div class="input-group">
          <textarea 
            id="chiefComplaint" 
            :value="formData.chiefComplaint"
            @input="updateField('chiefComplaint', $event.target.value)"
            placeholder="Motivul principal al vizitei"
          ></textarea>
          <span v-if="parsedData && parsedData['plangere principala'] && parsedData['plangere principala'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
            ✓
          </span>
        </div>
      </div>

      <div class="form-group">
        <label for="presentIllness">Istoricul Bolii Prezente</label>
        <div class="input-group">
          <textarea 
            id="presentIllness" 
            :value="formData.presentIllness"
            @input="updateField('presentIllness', $event.target.value)"
            placeholder="Descriere detaliată a simptomelor actuale și evoluția lor"
          ></textarea>
          <span v-if="parsedData && parsedData['istoricul prezent'] && parsedData['istoricul prezent'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
            ✓
          </span>
        </div>
      </div>

      <div class="form-group">
        <label for="pastMedicalHistory">Istoric Medical Trecut</label>
        <div class="input-group">
          <textarea 
            id="pastMedicalHistory" 
            :value="formData.pastMedicalHistory"
            @input="updateField('pastMedicalHistory', $event.target.value)"
            placeholder="Boli anterioare, intervenții chirurgicale, spitalizări"
          ></textarea>
          <span v-if="parsedData && parsedData['istoric medical trecut'] && parsedData['istoric medical trecut'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
            ✓
          </span>
        </div>
      </div>

      <div class="form-group">
        <label for="medications">Medicație Curentă</label>
        <div class="input-group">
          <textarea 
            id="medications" 
            :value="formData.medications"
            @input="updateField('medications', $event.target.value)"
            placeholder="Listați toate medicamentele curente, dozajele și frecvența"
          ></textarea>
          <span v-if="parsedData && parsedData['medicamente'] && parsedData['medicamente'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
            ✓
          </span>
        </div>
      </div>

      <div class="form-group">
        <label for="allergies">Alergii</label>
        <div class="input-group">
          <textarea 
            id="allergies" 
            :value="formData.allergies"
            @input="updateField('allergies', $event.target.value)"
            placeholder="Alergii la medicamente, alergii alimentare, alergii de mediu"
          ></textarea>
          <span v-if="parsedData && parsedData['alergii'] && parsedData['alergii'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
            ✓
          </span>
        </div>
      </div>

      <div class="form-group">
        <label for="familyHistory">Istoric Familial</label>
        <div class="input-group">
          <textarea 
            id="familyHistory" 
            :value="formData.familyHistory"
            @input="updateField('familyHistory', $event.target.value)"
            placeholder="Istoric medical familial relevant"
          ></textarea>
          <span v-if="parsedData && parsedData['istoric familial'] && parsedData['istoric familial'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
            ✓
          </span>
        </div>
      </div>

      <div class="form-group">
        <label for="socialHistory">Istoric Social</label>
        <div class="input-group">
          <textarea 
            id="socialHistory" 
            :value="formData.socialHistory"
            @input="updateField('socialHistory', $event.target.value)"
            placeholder="Fumat, alcool, ocupație, factori de stil de viață"
          ></textarea>
          <span v-if="parsedData && parsedData['istoric social'] && parsedData['istoric social'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
            ✓
          </span>
        </div>
      </div>

      <!-- Physical Examination -->
      <div class="form-group">
        <label for="vitalSigns">Semne Vitale</label>
        <div class="input-group">
          <textarea 
            id="vitalSigns" 
            :value="formData.vitalSigns"
            @input="updateField('vitalSigns', $event.target.value)"
            placeholder="Tensiune arterială, frecvență cardiacă, temperatură, frecvență respiratorie, saturație oxigen"
          ></textarea>
          <span v-if="parsedData && parsedData['semne vitale'] && parsedData['semne vitale'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
            ✓
          </span>
        </div>
      </div>

      <div class="form-group">
        <label for="physicalExam">Găsiri la Examinare Fizică</label>
        <div class="input-group">
          <textarea 
            id="physicalExam" 
            :value="formData.physicalExam"
            @input="updateField('physicalExam', $event.target.value)"
            placeholder="Aspect general, cap/gât, cardiovascular, respirator, abdominal, găsiri neurologice"
          ></textarea>
          <span v-if="parsedData && parsedData['examinare fizica'] && parsedData['examinare fizica'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
            ✓
          </span>
        </div>
      </div>

      <!-- Assessment and Plan -->
      <div class="form-group">
        <label for="assessment">Evaluare</label>
        <div class="input-group">
          <textarea 
            id="assessment" 
            :value="formData.assessment"
            @input="updateField('assessment', $event.target.value)"
            placeholder="Impresie clinică și diagnostic de lucru"
          ></textarea>
          <span v-if="parsedData && parsedData['evaluare'] && parsedData['evaluare'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
            ✓
          </span>
        </div>
      </div>

      <div class="form-group">
        <label for="plan">Plan de Tratament</label>
        <div class="input-group">
          <textarea 
            id="plan" 
            :value="formData.plan"
            @input="updateField('plan', $event.target.value)"
            placeholder="Teste diagnostice, medicamente, instrucțiuni de urmărire"
          ></textarea>
          <span v-if="parsedData && parsedData['plan'] && parsedData['plan'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
            ✓
          </span>
        </div>
      </div>

      <div class="form-group">
        <label for="followUp">Instrucțiuni de Urmărire</label>
        <div class="input-group">
          <textarea 
            id="followUp" 
            :value="formData.followUp"
            @input="updateField('followUp', $event.target.value)"
            placeholder="Când să revină, semne de alarmă de urmărit, modificări de stil de viață"
          ></textarea>
          <span v-if="parsedData && parsedData['urmarire'] && parsedData['urmarire'] !== ''" class="parsed-indicator" title="Valoare extrasă din audio">
            ✓
          </span>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { reactive, watch, ref, computed, onUnmounted } from 'vue'
import { useAudioProcessor } from '../../composables/useAudioProcessor.js'

const props = defineProps({
  patientData: {
    type: Object,
    default: () => ({})
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

const isFieldLocked = computed(() => {
  const hasSelectedPatient = props.patientData?.hasPatientSelected === true
  const isEditingSavedDocument = localStorage.getItem('editingDocumentId') !== null
  return hasSelectedPatient || isEditingSavedDocument
})

const formFields = [
  {
    key: 'patientName',
    romanianName: 'nume pacient'
  },
  {
    key: 'dateOfBirth',
    romanianName: 'data nasterii'
  },
  {
    key: 'gender',
    romanianName: 'gen'
  },
  {
    key: 'contactInfo',
    romanianName: 'informatii contact'
  },
  {
    key: 'chiefComplaint',
    romanianName: 'plangere principala'
  },
  {
    key: 'presentIllness',
    romanianName: 'istoricul prezent'
  },
  {
    key: 'pastMedicalHistory',
    romanianName: 'istoric medical trecut'
  },
  {
    key: 'medications',
    romanianName: 'medicamente'
  },
  {
    key: 'allergies',
    romanianName: 'alergii'
  },
  {
    key: 'familyHistory',
    romanianName: 'istoric familial'
  },
  {
    key: 'socialHistory',
    romanianName: 'istoric social'
  },
  {
    key: 'vitalSigns',
    romanianName: 'semne vitale'
  },
  {
    key: 'physicalExam',
    romanianName: 'examinare fizica'
  },
  {
    key: 'assessment',
    romanianName: 'evaluare'
  },
  {
    key: 'plan',
    romanianName: 'plan'
  },
  {
    key: 'followUp',
    romanianName: 'urmarire'
  }
]

const formData = reactive({
  patientName: '',
  dateOfBirth: '',
  gender: '',
  contactInfo: '',
  chiefComplaint: '',
  presentIllness: '',
  pastMedicalHistory: '',
  medications: '',
  allergies: '',
  familyHistory: '',
  socialHistory: '',
  vitalSigns: '',
  physicalExam: '',
  assessment: '',
  plan: '',
  followUp: ''
})

watch(() => props.patientData, (newData) => {
  if (newData) {
    const newPatientName = newData.patientName || newData.name || ''
    const newDateOfBirth = newData.dateOfBirth || ''
    const newGender = newData.gender || ''
    
    if (newPatientName && newPatientName !== formData.patientName) {
      formData.patientName = newPatientName
    }
    if (newDateOfBirth && newDateOfBirth !== formData.dateOfBirth) {
      formData.dateOfBirth = newDateOfBirth
    }
    if (newGender && newGender !== formData.gender) {
      formData.gender = newGender
    }
    
    Object.assign(formData, {
      contactInfo: newData.contactInfo || '',
      chiefComplaint: newData.chiefComplaint || '',
      presentIllness: newData.presentIllness || '',
      pastMedicalHistory: newData.pastMedicalHistory || '',
      medications: newData.medications || '',
      allergies: newData.allergies || '',
      familyHistory: newData.familyHistory || '',
      socialHistory: newData.socialHistory || '',
      vitalSigns: newData.vitalSigns || '',
      physicalExam: newData.physicalExam || '',
      assessment: newData.assessment || '',
      plan: newData.plan || '',
      followUp: newData.followUp || ''
    })
  }
}, { deep: true, immediate: true })

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
      await processAudio(fieldList, 'first-time-new-patient')
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
      if (field.key === 'patientName' || field.key === 'dateOfBirth' || field.key === 'gender') {
        if (!isFieldLocked.value) {
          formData[field.key] = value
          updateField(field.key, value)
        }
      } else {
        formData[field.key] = value
        updateField(field.key, value)
      }
    }
  })
}, { deep: true, immediate: true })

const updateField = (fieldName, value) => {
  formData[fieldName] = value
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
.document-template {
  padding: 1.5rem;
  border: 1px solid #e0e0e0;
  border-radius: 8px;
  background-color: #fdfdfd;
  max-width: 900px;
  margin: 0 auto;
}

.document-header {
  border-bottom: 2px solid #e9ecef;
  padding-bottom: 1.5rem;
  margin-bottom: 2rem;
}

.document-header h3 {
  color: #2c3e50;
  margin-bottom: 1rem;
  text-align: center;
  font-size: 1.8rem;
}

.report-info {
  display: flex;
  gap: 2rem;
  flex-wrap: wrap;
  justify-content: center;
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

.form-section {
  display: grid;
  gap: 1.5rem;
}

.form-group label {
  display: block;
  margin-bottom: 0.5rem;
  color: #34495e;
  font-weight: 600;
}

.input-group {
  display: flex;
  gap: 0.5rem;
  align-items: flex-start;
}

.input-group input[type="text"],
.input-group input[type="date"],
.input-group select,
.input-group textarea {
  flex: 1;
  padding: 0.8rem 1rem;
  border: 1px solid #ccc;
  border-radius: 6px;
  font-size: 1rem;
  transition: all 0.2s ease-in-out;
  box-sizing: border-box;
  font-family: inherit;
}

.input-group textarea {
  min-height: 100px;
  resize: vertical;
}

.input-group select {
  cursor: pointer;
}

.input-group input:focus,
.input-group select:focus,
.input-group textarea:focus {
  border-color: #667eea;
  box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.2);
  outline: none;
}

.parsed-indicator {
  color: #28a745;
  font-size: 1.2rem;
  font-weight: bold;
  margin-top: 0.5rem;
}

.field-locked {
  background-color: #f5f5f5 !important;
  color: #666 !important;
  cursor: not-allowed !important;
  border-color: #ccc !important;
  opacity: 0.7;
}

.field-locked:focus {
  border-color: #ccc !important;
  box-shadow: none !important;
}

@media (max-width: 768px) {
  .document-template {
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
