<template>
  <div class="document-template first-time-patient">
    <h3>First Time New Patient Assessment</h3>
    <div class="form-section">
      <!-- Patient Information -->
      <div class="form-group">
        <label for="patientName">Patient Full Name</label>
        <div class="input-with-button">
          <input 
            type="text" 
            id="patientName" 
            :value="formData.patientName"
            @input="updateField('patientName', $event.target.value)"
            @click="emitFieldClick('patientName')"
            :class="{ 'recording-active': isMicrophoneActive && activeRecordingField === 'patientName' }"
            placeholder="Enter patient's full name"
          />
          <button 
            type="button"
            class="record-button"
            :class="{ 'recording': isRecording && currentRecordingField === 'patientName' }"
            @click="startRecording('patientName')"
            :disabled="isRecording"
          >
            {{ isRecording && currentRecordingField === 'patientName' ? '⏹️' : '🎙️' }}
          </button>
        </div>
      </div>

      <div class="form-group">
        <label for="dateOfBirth">Date of Birth</label>
        <div class="input-with-button">
          <input 
            type="date" 
            id="dateOfBirth" 
            :value="formData.dateOfBirth"
            @input="updateField('dateOfBirth', $event.target.value)"
            @click="emitFieldClick('dateOfBirth')"
            :class="{ 'recording-active': isMicrophoneActive && activeRecordingField === 'dateOfBirth' }"
          />
          <button 
            type="button"
            class="record-button"
            :class="{ 'recording': isRecording && currentRecordingField === 'dateOfBirth' }"
            @click="startRecording('dateOfBirth')"
            :disabled="isRecording"
          >
            {{ isRecording && currentRecordingField === 'dateOfBirth' ? '⏹️' : '🎙️' }}
          </button>
        </div>
      </div>

      <div class="form-group">
        <label for="gender">Gender</label>
        <div class="input-with-button">
          <select 
            id="gender" 
            :value="formData.gender"
            @change="updateField('gender', $event.target.value)"
            @click="emitFieldClick('gender')"
            :class="{ 'recording-active': isMicrophoneActive && activeRecordingField === 'gender' }"
          >
            <option value="">Select Gender</option>
            <option value="Male">Male</option>
            <option value="Female">Female</option>
            <option value="Other">Other</option>
            <option value="Prefer not to say">Prefer not to say</option>
          </select>
          <button 
            type="button"
            class="record-button"
            :class="{ 'recording': isRecording && currentRecordingField === 'gender' }"
            @click="startRecording('gender')"
            :disabled="isRecording"
          >
            {{ isRecording && currentRecordingField === 'gender' ? '⏹️' : '🎙️' }}
          </button>
        </div>
      </div>

      <div class="form-group">
        <label for="contactInfo">Contact Information</label>
        <div class="input-with-button">
          <textarea 
            id="contactInfo" 
            :value="formData.contactInfo"
            @input="updateField('contactInfo', $event.target.value)"
            @click="emitFieldClick('contactInfo')"
            :class="{ 'recording-active': isMicrophoneActive && activeRecordingField === 'contactInfo' }"
            placeholder="Phone number, email address, emergency contact"
          ></textarea>
          <button 
            type="button"
            class="record-button"
            :class="{ 'recording': isRecording && currentRecordingField === 'contactInfo' }"
            @click="startRecording('contactInfo')"
            :disabled="isRecording"
          >
            {{ isRecording && currentRecordingField === 'contactInfo' ? '⏹️' : '🎙️' }}
          </button>
        </div>
      </div>

      <!-- Medical History -->
      <div class="form-group">
        <label for="chiefComplaint">Chief Complaint</label>
        <div class="input-with-button">
          <textarea 
            id="chiefComplaint" 
            :value="formData.chiefComplaint"
            @input="updateField('chiefComplaint', $event.target.value)"
            @click="emitFieldClick('chiefComplaint')"
            :class="{ 'recording-active': isMicrophoneActive && activeRecordingField === 'chiefComplaint' }"
            placeholder="Primary reason for visit"
          ></textarea>
          <button 
            type="button"
            class="record-button"
            :class="{ 'recording': isRecording && currentRecordingField === 'chiefComplaint' }"
            @click="startRecording('chiefComplaint')"
            :disabled="isRecording"
          >
            {{ isRecording && currentRecordingField === 'chiefComplaint' ? '⏹️' : '🎙️' }}
          </button>
        </div>
      </div>

      <div class="form-group">
        <label for="presentIllness">History of Present Illness</label>
        <div class="input-with-button">
          <textarea 
            id="presentIllness" 
            :value="formData.presentIllness"
            @input="updateField('presentIllness', $event.target.value)"
            @click="emitFieldClick('presentIllness')"
            :class="{ 'recording-active': isMicrophoneActive && activeRecordingField === 'presentIllness' }"
            placeholder="Detailed description of current symptoms and their progression"
          ></textarea>
          <button 
            type="button"
            class="record-button"
            :class="{ 'recording': isRecording && currentRecordingField === 'presentIllness' }"
            @click="startRecording('presentIllness')"
            :disabled="isRecording"
          >
            {{ isRecording && currentRecordingField === 'presentIllness' ? '⏹️' : '🎙️' }}
          </button>
        </div>
      </div>

      <div class="form-group">
        <label for="pastMedicalHistory">Past Medical History</label>
        <div class="input-with-button">
          <textarea 
            id="pastMedicalHistory" 
            :value="formData.pastMedicalHistory"
            @input="updateField('pastMedicalHistory', $event.target.value)"
            @click="emitFieldClick('pastMedicalHistory')"
            :class="{ 'recording-active': isMicrophoneActive && activeRecordingField === 'pastMedicalHistory' }"
            placeholder="Previous medical conditions, surgeries, hospitalizations"
          ></textarea>
          <button 
            type="button"
            class="record-button"
            :class="{ 'recording': isRecording && currentRecordingField === 'pastMedicalHistory' }"
            @click="startRecording('pastMedicalHistory')"
            :disabled="isRecording"
          >
            {{ isRecording && currentRecordingField === 'pastMedicalHistory' ? '⏹️' : '🎙️' }}
          </button>
        </div>
      </div>

      <div class="form-group">
        <label for="medications">Current Medications</label>
        <div class="input-with-button">
          <textarea 
            id="medications" 
            :value="formData.medications"
            @input="updateField('medications', $event.target.value)"
            @click="emitFieldClick('medications')"
            :class="{ 'recording-active': isMicrophoneActive && activeRecordingField === 'medications' }"
            placeholder="List all current medications, dosages, and frequency"
          ></textarea>
          <button 
            type="button"
            class="record-button"
            :class="{ 'recording': isRecording && currentRecordingField === 'medications' }"
            @click="startRecording('medications')"
            :disabled="isRecording"
          >
            {{ isRecording && currentRecordingField === 'medications' ? '⏹️' : '🎙️' }}
          </button>
        </div>
      </div>

      <div class="form-group">
        <label for="allergies">Allergies</label>
        <div class="input-with-button">
          <textarea 
            id="allergies" 
            :value="formData.allergies"
            @input="updateField('allergies', $event.target.value)"
            @click="emitFieldClick('allergies')"
            :class="{ 'recording-active': isMicrophoneActive && activeRecordingField === 'allergies' }"
            placeholder="Drug allergies, food allergies, environmental allergies"
          ></textarea>
          <button 
            type="button"
            class="record-button"
            :class="{ 'recording': isRecording && currentRecordingField === 'allergies' }"
            @click="startRecording('allergies')"
            :disabled="isRecording"
          >
            {{ isRecording && currentRecordingField === 'allergies' ? '⏹️' : '🎙️' }}
          </button>
        </div>
      </div>

      <div class="form-group">
        <label for="familyHistory">Family History</label>
        <div class="input-with-button">
          <textarea 
            id="familyHistory" 
            :value="formData.familyHistory"
            @input="updateField('familyHistory', $event.target.value)"
            @click="emitFieldClick('familyHistory')"
            :class="{ 'recording-active': isMicrophoneActive && activeRecordingField === 'familyHistory' }"
            placeholder="Relevant family medical history"
          ></textarea>
          <button 
            type="button"
            class="record-button"
            :class="{ 'recording': isRecording && currentRecordingField === 'familyHistory' }"
            @click="startRecording('familyHistory')"
            :disabled="isRecording"
          >
            {{ isRecording && currentRecordingField === 'familyHistory' ? '⏹️' : '🎙️' }}
          </button>
        </div>
      </div>

      <div class="form-group">
        <label for="socialHistory">Social History</label>
        <div class="input-with-button">
          <textarea 
            id="socialHistory" 
            :value="formData.socialHistory"
            @input="updateField('socialHistory', $event.target.value)"
            @click="emitFieldClick('socialHistory')"
            :class="{ 'recording-active': isMicrophoneActive && activeRecordingField === 'socialHistory' }"
            placeholder="Smoking, alcohol, occupation, lifestyle factors"
          ></textarea>
          <button 
            type="button"
            class="record-button"
            :class="{ 'recording': isRecording && currentRecordingField === 'socialHistory' }"
            @click="startRecording('socialHistory')"
            :disabled="isRecording"
          >
            {{ isRecording && currentRecordingField === 'socialHistory' ? '⏹️' : '🎙️' }}
          </button>
        </div>
      </div>

      <!-- Physical Examination -->
      <div class="form-group">
        <label for="vitalSigns">Vital Signs</label>
        <div class="input-with-button">
          <textarea 
            id="vitalSigns" 
            :value="formData.vitalSigns"
            @input="updateField('vitalSigns', $event.target.value)"
            @click="emitFieldClick('vitalSigns')"
            :class="{ 'recording-active': isMicrophoneActive && activeRecordingField === 'vitalSigns' }"
            placeholder="Blood pressure, heart rate, temperature, respiratory rate, oxygen saturation"
          ></textarea>
          <button 
            type="button"
            class="record-button"
            :class="{ 'recording': isRecording && currentRecordingField === 'vitalSigns' }"
            @click="startRecording('vitalSigns')"
            :disabled="isRecording"
          >
            {{ isRecording && currentRecordingField === 'vitalSigns' ? '⏹️' : '🎙️' }}
          </button>
        </div>
      </div>

      <div class="form-group">
        <label for="physicalExam">Physical Examination Findings</label>
        <div class="input-with-button">
          <textarea 
            id="physicalExam" 
            :value="formData.physicalExam"
            @input="updateField('physicalExam', $event.target.value)"
            @click="emitFieldClick('physicalExam')"
            :class="{ 'recording-active': isMicrophoneActive && activeRecordingField === 'physicalExam' }"
            placeholder="General appearance, head/neck, cardiovascular, respiratory, abdominal, neurological findings"
          ></textarea>
          <button 
            type="button"
            class="record-button"
            :class="{ 'recording': isRecording && currentRecordingField === 'physicalExam' }"
            @click="startRecording('physicalExam')"
            :disabled="isRecording"
          >
            {{ isRecording && currentRecordingField === 'physicalExam' ? '⏹️' : '🎙️' }}
          </button>
        </div>
      </div>

      <!-- Assessment and Plan -->
      <div class="form-group">
        <label for="assessment">Assessment</label>
        <div class="input-with-button">
          <textarea 
            id="assessment" 
            :value="formData.assessment"
            @input="updateField('assessment', $event.target.value)"
            @click="emitFieldClick('assessment')"
            :class="{ 'recording-active': isMicrophoneActive && activeRecordingField === 'assessment' }"
            placeholder="Clinical impression and working diagnosis"
          ></textarea>
          <button 
            type="button"
            class="record-button"
            :class="{ 'recording': isRecording && currentRecordingField === 'assessment' }"
            @click="startRecording('assessment')"
            :disabled="isRecording"
          >
            {{ isRecording && currentRecordingField === 'assessment' ? '⏹️' : '🎙️' }}
          </button>
        </div>
      </div>

      <div class="form-group">
        <label for="plan">Treatment Plan</label>
        <div class="input-with-button">
          <textarea 
            id="plan" 
            :value="formData.plan"
            @input="updateField('plan', $event.target.value)"
            @click="emitFieldClick('plan')"
            :class="{ 'recording-active': isMicrophoneActive && activeRecordingField === 'plan' }"
            placeholder="Diagnostic tests, medications, follow-up instructions"
          ></textarea>
          <button 
            type="button"
            class="record-button"
            :class="{ 'recording': isRecording && currentRecordingField === 'plan' }"
            @click="startRecording('plan')"
            :disabled="isRecording"
          >
            {{ isRecording && currentRecordingField === 'plan' ? '⏹️' : '🎙️' }}
          </button>
        </div>
      </div>

      <div class="form-group">
        <label for="followUp">Follow-up Instructions</label>
        <div class="input-with-button">
          <textarea 
            id="followUp" 
            :value="formData.followUp"
            @input="updateField('followUp', $event.target.value)"
            @click="emitFieldClick('followUp')"
            :class="{ 'recording-active': isMicrophoneActive && activeRecordingField === 'followUp' }"
            placeholder="When to return, warning signs to watch for, lifestyle modifications"
          ></textarea>
          <button 
            type="button"
            class="record-button"
            :class="{ 'recording': isRecording && currentRecordingField === 'followUp' }"
            @click="startRecording('followUp')"
            :disabled="isRecording"
          >
            {{ isRecording && currentRecordingField === 'followUp' ? '⏹️' : '🎙️' }}
          </button>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { reactive, inject, watch, ref } from 'vue'

const props = defineProps({
  isMicrophoneActive: Boolean,
  patientData: {
    type: Object,
    default: () => ({})
  }
})

const emit = defineEmits(['field-click', 'field-update'])

// Recording state
const isRecording = ref(false)
const currentRecordingField = ref(null)
const recordingStarted = ref(false)

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

const activeRecordingField = inject('activeRecordingField')

const emitFieldClick = (fieldName) => {
  emit('field-click', fieldName)
}

const updateField = (fieldName, value) => {
  console.log(`FirstTimeNewPatient: Updating field ${fieldName} with value:`, value)
  formData[fieldName] = value
  emit('field-update', fieldName, value)
  console.log(`FirstTimeNewPatient: Emitted field-update event for ${fieldName}`)
}

const startRecording = async (fieldName) => {
  if (isRecording.value) {
    console.log('Already recording, please wait...')
    return
  }

  try {
    console.log(`Starting recording for field: ${fieldName}`)
    isRecording.value = true
    currentRecordingField.value = fieldName
    recordingStarted.value = true
    
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
    currentRecordingField.value = null
    recordingStarted.value = false
    alert('Microphone access denied. Please allow microphone access to use speech-to-text.')
  }
}

const stopRecording = (fieldName) => {
  console.log(`Stopping recording for field: ${fieldName}`)
  
  // Only update the field if recording was actually started
  const wasRecording = recordingStarted.value && currentRecordingField.value === fieldName
  
  isRecording.value = false
  currentRecordingField.value = null
  recordingStarted.value = false
  
  // Here you would implement the actual speech-to-text functionality
  // For now, we'll just simulate it
  console.log(`Speech-to-text processing for ${fieldName} would happen here`)
  
  // Only update the field if this was actually a recording session
  // Don't overwrite user input with simulated text
  if (wasRecording) {
    // Simulate transcribed text (replace with actual speech-to-text result)
    const simulatedText = `Transcribed text for ${fieldName} field`
    updateField(fieldName, simulatedText)
  }
}

// Watch for changes in patientData to sync with form
watch(() => props.patientData, (newData) => {
  // Only update formData if the new data is different from current formData
  // This prevents overwriting user input
  const hasChanges = Object.keys(newData).some(key => {
    const newValue = newData[key] || ''
    const currentValue = formData[key] || ''
    return newValue !== currentValue
  })
  
  if (hasChanges) {
    console.log('Updating formData from patientData:', newData)
    Object.assign(formData, {
      patientName: newData.name || '',
      dateOfBirth: newData.dateOfBirth || '',
      gender: newData.gender || '',
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
</script>

<style scoped>
.document-template {
  padding: 1.5rem;
  border: 1px solid #e0e0e0;
  border-radius: 8px;
  background-color: #fdfdfd;
}

.document-template h3 {
  color: #34495e;
  margin-bottom: 1.5rem;
  text-align: center;
  font-size: 1.8rem;
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

.input-with-button {
  display: flex;
  gap: 0.5rem;
  align-items: flex-start;
}

.input-with-button input[type="text"],
.input-with-button input[type="date"],
.input-with-button select,
.input-with-button textarea {
  flex: 1;
  padding: 0.8rem 1rem;
  border: 1px solid #ccc;
  border-radius: 6px;
  font-size: 1rem;
  transition: all 0.2s ease-in-out;
  box-sizing: border-box;
}

.input-with-button textarea {
  min-height: 100px;
  resize: vertical;
}

.input-with-button select {
  cursor: pointer;
}

.record-button {
  padding: 0.8rem;
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  color: white;
  border: none;
  border-radius: 6px;
  cursor: pointer;
  font-size: 1.2rem;
  transition: all 0.3s ease;
  min-width: 50px;
  height: fit-content;
  display: flex;
  align-items: center;
  justify-content: center;
}

.record-button:hover:not(:disabled) {
  transform: translateY(-2px);
  box-shadow: 0 4px 12px rgba(102, 126, 234, 0.3);
}

.record-button:disabled {
  opacity: 0.6;
  cursor: not-allowed;
  transform: none;
}

.record-button:active {
  transform: translateY(0);
}

.record-button.recording {
  background: linear-gradient(135deg, #e74c3c 0%, #c0392b 100%);
  animation: recordingPulse 1s infinite;
}

.record-button.recording:hover {
  box-shadow: 0 4px 12px rgba(231, 76, 60, 0.3);
}

/* Recording active state */
.recording-active {
  border-color: #667eea !important;
  box-shadow: 0 0 0 2px rgba(102, 126, 234, 0.2) !important;
  animation: recordingPulse 2s infinite;
}

@keyframes recordingPulse {
  0% { box-shadow: 0 0 0 2px rgba(102, 126, 234, 0.2); }
  50% { box-shadow: 0 0 0 4px rgba(102, 126, 234, 0.4); }
  100% { box-shadow: 0 0 0 2px rgba(102, 126, 234, 0.2); }
}

.form-group input[type="text"]:focus,
.form-group input[type="date"]:focus,
.form-group select:focus,
.form-group textarea:focus {
  border-color: #667eea;
  box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.2);
  outline: none;
}

.recording-active {
  border-color: #ffc107 !important;
  box-shadow: 0 0 0 3px rgba(255, 193, 7, 0.4) !important;
  animation: pulse-border 1.5s infinite alternate;
}

@keyframes pulse-border {
  from {
    box-shadow: 0 0 0 3px rgba(255, 193, 7, 0.4);
  }
  to {
    box-shadow: 0 0 0 6px rgba(255, 193, 7, 0.7);
  }
}
</style>
