<template>
  <div class="medical-report">
    <div class="document-header">
      <h2>Medical Report</h2>
      <div class="report-info">
        <div class="info-item">
          <label>Date:</label>
          <span>{{ patientData.date }}</span>
        </div>
        <div class="info-item">
          <label>Patient:</label>
          <span>{{ patientData.name || 'Select Patient' }}</span>
        </div>
      </div>
    </div>

    <div class="form-sections">
      <!-- Chief Complaint -->
      <div class="form-section">
        <h3>Chief Complaint</h3>
        <div class="field-container">
          <label>Primary reason for visit:</label>
          <div class="input-with-button">
            <textarea 
              v-model="localData.chiefComplaint"
              placeholder="Click to record or type the chief complaint..."
              rows="3"
              @input="updateField('chiefComplaint', $event.target.value)"
              @click="handleFieldClick('chiefComplaint')"
              :class="{ 'recording-active': isRecording && currentField === 'chiefComplaint' }"
            ></textarea>
            <button 
              type="button"
              class="record-button"
              :class="{ 'recording': isRecording && currentField === 'chiefComplaint' }"
              @click="startRecording('chiefComplaint')"
              :disabled="isRecording"
            >
              <svg v-if="isRecording && currentField === 'chiefComplaint'" viewBox="0 0 24 24" fill="currentColor" class="stop-icon">
                <rect x="6" y="6" width="12" height="12" rx="2"/>
              </svg>
              <svg v-else viewBox="0 0 24 24" fill="currentColor" class="mic-icon">
                <path d="M12 14c1.66 0 3-1.34 3-3V5c0-1.66-1.34-3-3-3S9 3.34 9 5v6c0 1.66 1.34 3 3 3z"/>
                <path d="M17 11c0 2.76-2.24 5-5 5s-5-2.24-5-5H5c0 3.53 2.61 6.43 6 6.92V21h2v-3.08c3.39-.49 6-3.39 6-6.92h-2z"/>
              </svg>
            </button>
          </div>
        </div>
      </div>

      <!-- History of Present Illness -->
      <div class="form-section">
        <h3>History of Present Illness</h3>
        <div class="field-container">
          <label>Detailed history and symptoms:</label>
          <div class="input-with-button">
            <textarea 
              v-model="localData.historyOfPresentIllness"
              placeholder="Click to record or type the history of present illness..."
              rows="4"
              @input="updateField('historyOfPresentIllness', $event.target.value)"
              @click="handleFieldClick('historyOfPresentIllness')"
              :class="{ 'recording-active': isRecording && currentField === 'historyOfPresentIllness' }"
            ></textarea>
            <button 
              type="button"
              class="record-button"
              :class="{ 'recording': isRecording && currentField === 'historyOfPresentIllness' }"
              @click="startRecording('historyOfPresentIllness')"
              :disabled="isRecording"
            >
              <svg v-if="isRecording && currentField === 'historyOfPresentIllness'" viewBox="0 0 24 24" fill="currentColor" class="stop-icon">
                <rect x="6" y="6" width="12" height="12" rx="2"/>
              </svg>
              <svg v-else viewBox="0 0 24 24" fill="currentColor" class="mic-icon">
                <path d="M12 14c1.66 0 3-1.34 3-3V5c0-1.66-1.34-3-3-3S9 3.34 9 5v6c0 1.66 1.34 3 3 3z"/>
                <path d="M17 11c0 2.76-2.24 5-5 5s-5-2.24-5-5H5c0 3.53 2.61 6.43 6 6.92V21h2v-3.08c3.39-.49 6-3.39 6-6.92h-2z"/>
              </svg>
            </button>
          </div>
        </div>
      </div>

      <!-- Physical Examination -->
      <div class="form-section">
        <h3>Physical Examination</h3>
        <div class="field-container">
          <label>Examination findings:</label>
          <div class="input-with-button">
            <textarea 
              v-model="localData.physicalExamination"
              placeholder="Click to record or type physical examination findings..."
              rows="4"
              @input="updateField('physicalExamination', $event.target.value)"
              @click="handleFieldClick('physicalExamination')"
              :class="{ 'recording-active': isRecording && currentField === 'physicalExamination' }"
            ></textarea>
            <button 
              type="button"
              class="record-button"
              :class="{ 'recording': isRecording && currentField === 'physicalExamination' }"
              @click="startRecording('physicalExamination')"
              :disabled="isRecording"
            >
              <svg v-if="isRecording && currentField === 'physicalExamination'" viewBox="0 0 24 24" fill="currentColor" class="stop-icon">
                <rect x="6" y="6" width="12" height="12" rx="2"/>
              </svg>
              <svg v-else viewBox="0 0 24 24" fill="currentColor" class="mic-icon">
                <path d="M12 14c1.66 0 3-1.34 3-3V5c0-1.66-1.34-3-3-3S9 3.34 9 5v6c0 1.66 1.34 3 3 3z"/>
                <path d="M17 11c0 2.76-2.24 5-5 5s-5-2.24-5-5H5c0 3.53 2.61 6.43 6 6.92V21h2v-3.08c3.39-.49 6-3.39 6-6.92h-2z"/>
              </svg>
            </button>
          </div>
        </div>
      </div>

      <!-- Diagnosis -->
      <div class="form-section">
        <h3>Diagnosis</h3>
        <div class="field-container">
          <label>Medical diagnosis:</label>
          <div class="input-with-button">
            <textarea 
              v-model="localData.diagnosis"
              placeholder="Click to record or type the diagnosis..."
              rows="3"
              @input="updateField('diagnosis', $event.target.value)"
              @click="handleFieldClick('diagnosis')"
              :class="{ 'recording-active': isRecording && currentField === 'diagnosis' }"
            ></textarea>
            <button 
              type="button"
              class="record-button"
              :class="{ 'recording': isRecording && currentField === 'diagnosis' }"
              @click="startRecording('diagnosis')"
              :disabled="isRecording"
            >
              <svg v-if="isRecording && currentField === 'diagnosis'" viewBox="0 0 24 24" fill="currentColor" class="stop-icon">
                <rect x="6" y="6" width="12" height="12" rx="2"/>
              </svg>
              <svg v-else viewBox="0 0 24 24" fill="currentColor" class="mic-icon">
                <path d="M12 14c1.66 0 3-1.34 3-3V5c0-1.66-1.34-3-3-3S9 3.34 9 5v6c0 1.66 1.34 3 3 3z"/>
                <path d="M17 11c0 2.76-2.24 5-5 5s-5-2.24-5-5H5c0 3.53 2.61 6.43 6 6.92V21h2v-3.08c3.39-.49 6-3.39 6-6.92h-2z"/>
              </svg>
            </button>
          </div>
        </div>
      </div>

      <!-- Treatment -->
      <div class="form-section">
        <h3>Treatment Plan</h3>
        <div class="field-container">
          <label>Recommended treatment:</label>
          <div class="input-with-button">
            <textarea 
              v-model="localData.treatment"
              placeholder="Click to record or type the treatment plan..."
              rows="4"
              @input="updateField('treatment', $event.target.value)"
              @click="handleFieldClick('treatment')"
              :class="{ 'recording-active': isRecording && currentField === 'treatment' }"
            ></textarea>
            <button 
              type="button"
              class="record-button"
              :class="{ 'recording': isRecording && currentField === 'treatment' }"
              @click="startRecording('treatment')"
              :disabled="isRecording"
            >
              <svg v-if="isRecording && currentField === 'treatment'" viewBox="0 0 24 24" fill="currentColor" class="stop-icon">
                <rect x="6" y="6" width="12" height="12" rx="2"/>
              </svg>
              <svg v-else viewBox="0 0 24 24" fill="currentColor" class="mic-icon">
                <path d="M12 14c1.66 0 3-1.34 3-3V5c0-1.66-1.34-3-3-3S9 3.34 9 5v6c0 1.66 1.34 3 3 3z"/>
                <path d="M17 11c0 2.76-2.24 5-5 5s-5-2.24-5-5H5c0 3.53 2.61 6.43 6 6.92V21h2v-3.08c3.39-.49 6-3.39 6-6.92h-2z"/>
              </svg>
            </button>
          </div>
        </div>
      </div>

      <!-- Recommendations -->
      <div class="form-section">
        <h3>Recommendations</h3>
        <div class="field-container">
          <label>Follow-up recommendations:</label>
          <div class="input-with-button">
            <textarea 
              v-model="localData.recommendations"
              placeholder="Click to record or type recommendations..."
              rows="3"
              @input="updateField('recommendations', $event.target.value)"
              @click="handleFieldClick('recommendations')"
              :class="{ 'recording-active': isRecording && currentField === 'recommendations' }"
            ></textarea>
            <button 
              type="button"
              class="record-button"
              :class="{ 'recording': isRecording && currentField === 'recommendations' }"
              @click="startRecording('recommendations')"
              :disabled="isRecording"
            >
              <svg v-if="isRecording && currentField === 'recommendations'" viewBox="0 0 24 24" fill="currentColor" class="stop-icon">
                <rect x="6" y="6" width="12" height="12" rx="2"/>
              </svg>
              <svg v-else viewBox="0 0 24 24" fill="currentColor" class="mic-icon">
                <path d="M12 14c1.66 0 3-1.34 3-3V5c0-1.66-1.34-3-3-3S9 3.34 9 5v6c0 1.66 1.34 3 3 3z"/>
                <path d="M17 11c0 2.76-2.24 5-5 5s-5-2.24-5-5H5c0 3.53 2.61 6.43 6 6.92V21h2v-3.08c3.39-.49 6-3.39 6-6.92h-2z"/>
              </svg>
            </button>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, reactive, watch } from 'vue'

const props = defineProps({
  patientData: {
    type: Object,
    required: true
  },
  isRecording: {
    type: Boolean,
    default: false
  },
  currentField: {
    type: String,
    default: ''
  }
})

const emit = defineEmits(['field-click', 'field-update'])

// Recording state
const isRecording = ref(false)
const currentField = ref('')

// Local reactive data
const localData = reactive({
  chiefComplaint: props.patientData.chiefComplaint || '',
  historyOfPresentIllness: props.patientData.historyOfPresentIllness || '',
  physicalExamination: props.patientData.physicalExamination || '',
  diagnosis: props.patientData.diagnosis || '',
  treatment: props.patientData.treatment || '',
  recommendations: props.patientData.recommendations || ''
})

// Watch for changes in patientData
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

const handleFieldClick = (fieldName) => {
  emit('field-click', fieldName)
}

const startRecording = async (fieldName) => {
  if (isRecording.value) {
    console.log('Already recording, please wait...')
    return
  }

  try {
    console.log(`Starting recording for field: ${fieldName}`)
    isRecording.value = true
    currentField.value = fieldName
    
    // Request microphone access
    const stream = await navigator.mediaDevices.getUserMedia({ audio: true })
    console.log('Microphone access granted')
    
    // Simulate 5-second recording
    setTimeout(() => {
      stopRecording(fieldName)
    }, 5000)
    
  } catch (error) {
    console.error('Failed to start recording:', error)
    isRecording.value = false
    currentField.value = ''
    alert('Microphone access denied. Please allow microphone access to use speech-to-text.')
  }
}

const stopRecording = (fieldName) => {
  console.log(`Stopping recording for field: ${fieldName}`)
  isRecording.value = false
  currentField.value = ''
  
  // Simulate transcription result
  const mockTranscription = `Transcribed text for ${fieldName} field. This is a placeholder transcription.`
  updateField(fieldName, mockTranscription)
}

const updateField = (fieldName, value) => {
  console.log(`MedicalReport: Updating field ${fieldName} with value:`, value)
  localData[fieldName] = value
  emit('field-update', fieldName, value)
  console.log(`MedicalReport: Emitted field-update event for ${fieldName}`)
}
</script>

<style scoped>
.medical-report {
  max-width: 800px;
  margin: 0 auto;
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
  margin-bottom: 1rem;
  font-size: 1.3rem;
  border-bottom: 1px solid #dee2e6;
  padding-bottom: 0.5rem;
}

.field-container {
  position: relative;
  cursor: pointer;
  transition: all 0.3s ease;
  border-radius: 6px;
  padding: 0.5rem;
}

.field-container:hover {
  background: #f8f9fa;
}

.field-container--recording {
  background: #fff3cd;
  border: 2px solid #ffc107;
  animation: pulse 2s infinite;
}

@keyframes pulse {
  0% { box-shadow: 0 0 0 0 rgba(255, 193, 7, 0.4); }
  70% { box-shadow: 0 0 0 10px rgba(255, 193, 7, 0); }
  100% { box-shadow: 0 0 0 0 rgba(255, 193, 7, 0); }
}

.field-container label {
  display: block;
  font-weight: 600;
  color: #495057;
  margin-bottom: 0.5rem;
}

.input-with-button {
  display: flex;
  gap: 0.5rem;
  align-items: flex-start;
}

.input-with-button textarea {
  flex: 1;
  border: 1px solid #ced4da;
  border-radius: 6px;
  padding: 0.75rem;
  font-size: 0.95rem;
  line-height: 1.5;
  resize: vertical;
  transition: all 0.3s ease;
}

.input-with-button textarea:focus {
  outline: none;
  border-color: #667eea;
  box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.1);
}

.record-button {
  width: 40px;
  height: 40px;
  border: none;
  border-radius: 8px;
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  color: white;
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 1.2rem;
  transition: all 0.3s ease;
  flex-shrink: 0;
}

.record-button:hover:not(:disabled) {
  transform: translateY(-2px);
  box-shadow: 0 4px 12px rgba(102, 126, 234, 0.3);
}

.record-button:disabled {
  opacity: 0.6;
  cursor: not-allowed;
}

.record-button.recording {
  background: linear-gradient(135deg, #dc3545 0%, #c82333 100%);
  animation: pulse 2s infinite;
}

.recording-active {
  border-color: #ffc107 !important;
  box-shadow: 0 0 0 3px rgba(255, 193, 7, 0.1) !important;
}

.mic-icon,
.stop-icon {
  width: 20px;
  height: 20px;
}

.recording-indicator {
  position: absolute;
  top: 0.5rem;
  right: 0.5rem;
  display: flex;
  align-items: center;
  gap: 0.5rem;
  background: #dc3545;
  color: white;
  padding: 0.25rem 0.75rem;
  border-radius: 4px;
  font-size: 0.8rem;
  font-weight: 600;
}

.recording-dot {
  width: 8px;
  height: 8px;
  background: white;
  border-radius: 50%;
  animation: blink 1s infinite;
}

@keyframes blink {
  0%, 50% { opacity: 1; }
  51%, 100% { opacity: 0; }
}

@media (max-width: 768px) {
  .report-info {
    flex-direction: column;
    gap: 1rem;
  }
  
  .form-section {
    padding: 1rem;
  }
}
</style>
