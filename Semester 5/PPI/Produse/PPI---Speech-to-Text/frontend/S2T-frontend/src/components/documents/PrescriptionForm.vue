<template>
  <div class="prescription-form">
    <div class="document-header">
      <h2>Prescription Form</h2>
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
      <!-- Medications -->
      <div class="form-section">
        <h3>Prescribed Medications</h3>
        <div class="field-container">
          <label>Medication names and types:</label>
          <div class="input-with-button">
            <textarea 
              v-model="localData.medications"
              placeholder="Click to record or type prescribed medications..."
              rows="4"
              @input="updateField('medications', $event.target.value)"
              @click="handleFieldClick('medications')"
              :class="{ 'recording-active': isRecording && currentField === 'medications' }"
            ></textarea>
            <button 
              type="button"
              class="record-button"
              :class="{ 'recording': isRecording && currentField === 'medications' }"
              @click="startRecording('medications')"
              :disabled="isRecording"
            >
              <svg v-if="isRecording && currentField === 'medications'" viewBox="0 0 24 24" fill="currentColor" class="stop-icon">
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

      <!-- Dosage -->
      <div class="form-section">
        <h3>Dosage Instructions</h3>
        <div class="field-container">
          <label>Dosage amounts and frequency:</label>
          <div class="input-with-button">
            <textarea 
              v-model="localData.dosage"
              placeholder="Click to record or type dosage instructions..."
              rows="3"
              @input="updateField('dosage', $event.target.value)"
              @click="handleFieldClick('dosage')"
              :class="{ 'recording-active': isRecording && currentField === 'dosage' }"
            ></textarea>
            <button 
              type="button"
              class="record-button"
              :class="{ 'recording': isRecording && currentField === 'dosage' }"
              @click="startRecording('dosage')"
              :disabled="isRecording"
            >
              <svg v-if="isRecording && currentField === 'dosage'" viewBox="0 0 24 24" fill="currentColor" class="stop-icon">
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

      <!-- Instructions -->
      <div class="form-section">
        <h3>Special Instructions</h3>
        <div class="field-container">
          <label>Special instructions and warnings:</label>
          <div class="input-with-button">
            <textarea 
              v-model="localData.instructions"
              placeholder="Click to record or type special instructions..."
              rows="4"
              @input="updateField('instructions', $event.target.value)"
              @click="handleFieldClick('instructions')"
              :class="{ 'recording-active': isRecording && currentField === 'instructions' }"
            ></textarea>
            <button 
              type="button"
              class="record-button"
              :class="{ 'recording': isRecording && currentField === 'instructions' }"
              @click="startRecording('instructions')"
              :disabled="isRecording"
            >
              <svg v-if="isRecording && currentField === 'instructions'" viewBox="0 0 24 24" fill="currentColor" class="stop-icon">
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

      <!-- Follow-up -->
      <div class="form-section">
        <h3>Follow-up Instructions</h3>
        <div class="field-container">
          <label>Follow-up appointments and monitoring:</label>
          <div class="input-with-button">
            <textarea 
              v-model="localData.followUp"
              placeholder="Click to record or type follow-up instructions..."
              rows="3"
              @input="updateField('followUp', $event.target.value)"
              @click="handleFieldClick('followUp')"
              :class="{ 'recording-active': isRecording && currentField === 'followUp' }"
            ></textarea>
            <button 
              type="button"
              class="record-button"
              :class="{ 'recording': isRecording && currentField === 'followUp' }"
              @click="startRecording('followUp')"
              :disabled="isRecording"
            >
              <svg v-if="isRecording && currentField === 'followUp'" viewBox="0 0 24 24" fill="currentColor" class="stop-icon">
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
  medications: props.patientData.medications || '',
  dosage: props.patientData.dosage || '',
  instructions: props.patientData.instructions || '',
  followUp: props.patientData.followUp || ''
})

// Watch for changes in patientData
watch(() => props.patientData, (newData) => {
  Object.assign(localData, {
    medications: newData.medications || '',
    dosage: newData.dosage || '',
    instructions: newData.instructions || '',
    followUp: newData.followUp || ''
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
  localData[fieldName] = value
  emit('field-update', fieldName, value)
}
</script>

<style scoped>
.prescription-form {
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
