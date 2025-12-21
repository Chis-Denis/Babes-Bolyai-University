<template>
  <div class="recording-management">
    <div class="page-header">
      <h2>Audio Recording</h2>
      <p>Record medical observations and save them for transcription</p>
    </div>

    <!-- Recording Status -->
    <div class="recording-status-card">
      <div class="status-indicator" :class="{ 'status-indicator--recording': isRecording, 'status-indicator--processing': isProcessing }">
        <div class="status-icon" :class="{ 'status-icon--recording': isRecording, 'status-icon--processing': isProcessing }">
          <svg v-if="isRecording" viewBox="0 0 24 24" fill="currentColor" class="recording-icon-svg">
            <circle cx="12" cy="12" r="8"/>
          </svg>
          <svg v-else-if="isProcessing" viewBox="0 0 24 24" fill="currentColor" class="processing-icon-svg">
            <path d="M12,4V2A10,10 0 0,0 2,12H4A8,8 0 0,1 12,4Z">
              <animateTransform attributeName="transform" attributeType="XML" type="rotate" dur="1s" from="0 12 12" to="360 12 12" repeatCount="indefinite"/>
            </path>
          </svg>
          <svg v-else viewBox="0 0 24 24" fill="currentColor" class="ready-icon-svg">
            <circle cx="12" cy="12" r="8" stroke="currentColor" stroke-width="2" fill="none"/>
          </svg>
        </div>
        <div class="status-text">
          <span v-if="isRecording">Recording...</span>
          <span v-else-if="isProcessing">Processing...</span>
          <span v-else>Ready to record</span>
        </div>
      </div>
      
      <div v-if="isRecording" class="recording-timer">
        <span class="timer-text">{{ formattedDuration }}</span>
      </div>
    </div>

    <!-- Recording Controls -->
    <div class="recording-controls">
      <button
        :class="['record-button', { 'record-button--recording': isRecording, 'record-button--processing': isProcessing }]"
        :disabled="!canRecord || isProcessing"
        @click="toggleRecording"
      >
        <div class="button-icon">
          <svg v-if="isRecording" viewBox="0 0 24 24" fill="currentColor" class="stop-icon">
            <rect x="6" y="6" width="12" height="12" rx="2"/>
          </svg>
          <svg v-else viewBox="0 0 24 24" fill="currentColor" class="mic-icon">
            <path d="M12 14c1.66 0 3-1.34 3-3V5c0-1.66-1.34-3-3-3S9 3.34 9 5v6c0 1.66 1.34 3 3 3z"/>
            <path d="M17 11c0 2.76-2.24 5-5 5s-5-2.24-5-5H5c0 3.53 2.61 6.43 6 6.92V21h2v-3.08c3.39-.49 6-3.39 6-6.92h-2z"/>
          </svg>
        </div>
        <span>{{ isRecording ? 'Stop Recording' : 'Start Recording' }}</span>
      </button>
    </div>

    <!-- Audio Visualization -->
    <div v-if="isRecording" class="audio-visualization">
      <div class="visualization-header">
        <div class="visualization-icon">
          <svg viewBox="0 0 24 24" fill="currentColor">
            <path d="M3,9V15H7L12,20V4L7,9H3M16.5,12C16.5,10.23 15.48,8.71 14,7.97V16C15.48,15.29 16.5,13.76 16.5,12M14,3.23V5.29C16.89,6.15 19,8.83 19,12C19,15.17 16.89,17.85 14,18.71V20.77C18.01,19.86 21,16.28 21,12C21,7.72 18.01,4.14 14,3.23Z"/>
          </svg>
        </div>
        <div class="visualization-title">Audio Level</div>
      </div>
      <div class="audio-bars">
        <div 
          v-for="(bar, index) in 20" 
          :key="index"
          class="audio-bar"
          :class="{ 'audio-bar--active': index % 3 === 0 }"
          :style="{ height: getRandomBarHeight() + '%', animationDelay: (index * 0.1) + 's' }"
        ></div>
      </div>
    </div>

    <!-- Recording History -->
    <div v-if="recordings.length > 0" class="recording-history">
      <div class="history-header">
        <div class="history-title-section">
          <div class="history-icon">
            <svg viewBox="0 0 24 24" fill="currentColor">
              <path d="M12,2A10,10 0 0,0 2,12A10,10 0 0,0 12,22A10,10 0 0,0 22,12A10,10 0 0,0 12,2M12,4A8,8 0 0,1 20,12A8,8 0 0,1 12,20A8,8 0 0,1 4,12A8,8 0 0,1 12,4M12,6A6,6 0 0,0 6,12A6,6 0 0,0 12,18A6,6 0 0,0 18,12A6,6 0 0,0 12,6M12,8A4,4 0 0,1 16,12A4,4 0 0,1 12,16A4,4 0 0,1 8,12A4,4 0 0,1 12,8Z"/>
            </svg>
          </div>
          <h3>Recording History</h3>
        </div>
        <div class="history-stats">
          <div class="stat-item">
            <div class="stat-icon">
              <svg viewBox="0 0 24 24" fill="currentColor">
                <path d="M12,2A10,10 0 0,0 2,12A10,10 0 0,0 12,22A10,10 0 0,0 22,12A10,10 0 0,0 12,2M12,4A8,8 0 0,1 20,12A8,8 0 0,1 12,20A8,8 0 0,1 4,12A8,8 0 0,1 12,4M12,6A6,6 0 0,0 6,12A6,6 0 0,0 12,18A6,6 0 0,0 18,12A6,6 0 0,0 12,6M12,8A4,4 0 0,1 16,12A4,4 0 0,1 12,16A4,4 0 0,1 8,12A4,4 0 0,1 12,8Z"/>
              </svg>
            </div>
            <span class="stat">{{ recordings.length }} recordings</span>
          </div>
          <div class="stat-item">
            <div class="stat-icon">
              <svg viewBox="0 0 24 24" fill="currentColor">
                <path d="M12,20A8,8 0 0,1 4,12A8,8 0 0,1 12,4A8,8 0 0,1 20,12A8,8 0 0,1 12,20M12,2A10,10 0 0,0 2,12A10,10 0 0,0 12,22A10,10 0 0,0 22,12A10,10 0 0,0 12,2M12.5,7V12.25L17,14.92L16.25,16.15L11,13V7H12.5Z"/>
              </svg>
            </div>
            <span class="stat">{{ totalDuration }}</span>
          </div>
        </div>
      </div>
      
      <div class="recordings-list">
        <div 
          v-for="(recording, index) in recordings" 
          :key="recording.id"
          :data-recording-id="recording.id"
          class="recording-item"
          :class="{ 'recording-item--playing': currentAudio?.id === recording.id }"
        >
          <div class="recording-avatar">
            <div class="avatar-circle" :class="getRecordingAvatarClass(index)">
              <svg viewBox="0 0 24 24" fill="currentColor">
                <path d="M12 14c1.66 0 3-1.34 3-3V5c0-1.66-1.34-3-3-3S9 3.34 9 5v6c0 1.66 1.34 3 3 3z"/>
                <path d="M17 11c0 2.76-2.24 5-5 5s-5-2.24-5-5H5c0 3.53 2.61 6.43 6 6.92V21h2v-3.08c3.39-.49 6-3.39 6-6.92h-2z"/>
              </svg>
            </div>
          </div>
          
          <div class="recording-info">
            <div class="recording-title">{{ recording.title }}</div>
            <div class="recording-meta">
              <div class="meta-item">
                <svg viewBox="0 0 24 24" fill="currentColor" class="meta-icon">
                  <path d="M19,3H5C3.89,3 3,3.89 3,5V19A2,2 0 0,0 5,21H19A2,2 0 0,0 21,19V5C21,3.89 20.1,3 19,3M19,5V7H5V5H19M19,19H5V9H19V19M7,11H9V17H7V11M11,11H13V17H11V11M15,11H17V17H15V11Z"/>
                </svg>
                <span class="recording-date">{{ formatDate(recording.timestamp) }}</span>
              </div>
              <div class="meta-item">
                <svg viewBox="0 0 24 24" fill="currentColor" class="meta-icon">
                  <path d="M12,20A8,8 0 0,1 4,12A8,8 0 0,1 12,4A8,8 0 0,1 20,12A8,8 0 0,1 12,20M12,2A10,10 0 0,0 2,12A10,10 0 0,0 12,22A10,10 0 0,0 22,12A10,10 0 0,0 12,2M12.5,7V12.25L17,14.92L16.25,16.15L11,13V7H12.5Z"/>
                </svg>
                <span class="recording-duration">{{ recording.duration }}</span>
              </div>
              <div class="meta-item">
                <svg viewBox="0 0 24 24" fill="currentColor" class="meta-icon">
                  <path d="M14,2H6A2,2 0 0,0 4,4V20A2,2 0 0,0 6,22H18A2,2 0 0,0 20,20V8L14,2M18,20H6V4H13V9H18V20Z"/>
                </svg>
                <span class="recording-size">{{ recording.size }}</span>
              </div>
            </div>
          </div>
          
          <div class="recording-actions">
            <button 
              v-if="recording.audioUrl"
              @click="playRecording(recording.id)"
              class="action-button play-button"
              :disabled="currentAudio && currentAudio.id !== recording.id"
            >
              <svg viewBox="0 0 24 24" fill="currentColor">
                <path d="M8 5v14l11-7z"/>
              </svg>
            </button>
            
            <button 
              @click="deleteRecording(recording.id)"
              class="action-button delete-button"
            >
              <svg viewBox="0 0 24 24" fill="currentColor">
                <path d="M19,4H15.5L14.5,3H9.5L8.5,4H5V6H19M6,19A2,2 0 0,0 8,21H16A2,2 0 0,0 18,19V7H6V19Z"/>
              </svg>
            </button>
          </div>
        </div>
      </div>
    </div>

    <!-- Empty State -->
    <div v-else class="empty-state">
      <div class="empty-icon">
        <svg viewBox="0 0 24 24" fill="currentColor" class="empty-icon-svg">
          <path d="M12 14c1.66 0 3-1.34 3-3V5c0-1.66-1.34-3-3-3S9 3.34 9 5v6c0 1.66 1.34 3 3 3z"/>
          <path d="M17 11c0 2.76-2.24 5-5 5s-5-2.24-5-5H5c0 3.53 2.61 6.43 6 6.92V21h2v-3.08c3.39-.49 6-3.39 6-6.92h-2z"/>
        </svg>
      </div>
      <h3>No recordings yet</h3>
      <p>Start recording to capture medical observations and save them for transcription.</p>
    </div>

    <!-- Delete Confirmation Modal -->
    <ConfirmModal
      :is-open="showDeleteModal"
      title="Delete Recording"
      message="Are you sure you want to delete this recording?"
      details="This action cannot be undone. The recording will be permanently removed."
      confirm-text="Delete"
      cancel-text="Cancel"
      @confirm="confirmDelete"
      @cancel="cancelDelete"
      @close="cancelDelete"
    />
  </div>
</template>

<script setup>
import { ref, reactive, computed, onMounted, onUnmounted } from 'vue'
import ConfirmModal from '@/components/common/ConfirmModal.vue'
import { cookieService } from '@/services/CookieService.js'
import { toastService } from '@/services/ToastService.js'

// State
const isRecording = ref(false)
const isProcessing = ref(false)
const canRecord = ref(false)
const recordings = ref([])
const currentAudio = ref(null)
const showDeleteModal = ref(false)
const recordingToDelete = ref(null)

// Recording state
const mediaRecorder = ref(null)
const audioChunks = ref([])
const recordingStartTime = ref(null)
const recordingDuration = ref(0)
const durationInterval = ref(null)

// Computed properties
const formattedDuration = computed(() => {
  const minutes = Math.floor(recordingDuration.value / 60)
  const seconds = recordingDuration.value % 60
  return `${minutes.toString().padStart(2, '0')}:${seconds.toString().padStart(2, '0')}`
})

const totalDuration = computed(() => {
  const totalSeconds = recordings.value.reduce((total, recording) => {
    const [minutes, seconds] = recording.duration.split(':').map(Number)
    return total + (minutes * 60) + seconds
  }, 0)
  
  const hours = Math.floor(totalSeconds / 3600)
  const minutes = Math.floor((totalSeconds % 3600) / 60)
  const seconds = totalSeconds % 60
  
  if (hours > 0) {
    return `${hours}h ${minutes}m ${seconds}s`
  } else if (minutes > 0) {
    return `${minutes}m ${seconds}s`
  } else {
    return `${seconds}s`
  }
})

// Methods
const checkMicrophonePermission = async () => {
  try {
    const stream = await navigator.mediaDevices.getUserMedia({ audio: true })
    canRecord.value = true
    stream.getTracks().forEach(track => track.stop())
  } catch (error) {
    console.error('Microphone access denied:', error)
    canRecord.value = false
    toastService.error('Microphone Access Denied', 'Please allow microphone access to record audio')
  }
}

const startRecording = async () => {
  try {
    const stream = await navigator.mediaDevices.getUserMedia({ audio: true })
    
    mediaRecorder.value = new MediaRecorder(stream)
    audioChunks.value = []
    
    mediaRecorder.value.ondataavailable = (event) => {
      if (event.data.size > 0) {
        audioChunks.value.push(event.data)
      }
    }
    
    mediaRecorder.value.start()
    isRecording.value = true
    recordingStartTime.value = Date.now()
    recordingDuration.value = 0
    
    // Start duration timer
    durationInterval.value = setInterval(() => {
      recordingDuration.value = Math.floor((Date.now() - recordingStartTime.value) / 1000)
    }, 1000)
    
    console.log('Recording started')
    
  } catch (error) {
    console.error('Failed to start recording:', error)
    toastService.error('Recording Failed', 'Could not start recording. Please check your microphone.')
    isRecording.value = false
  }
}

const stopRecording = async () => {
  try {
    isRecording.value = false
    isProcessing.value = true
    
    // Stop the MediaRecorder
    if (mediaRecorder.value && mediaRecorder.value.state === 'recording') {
      mediaRecorder.value.stop()
    }
    
    // Clear duration timer
    if (durationInterval.value) {
      clearInterval(durationInterval.value)
      durationInterval.value = null
    }
    
    // Wait for the recording to finish processing
    await new Promise((resolve) => {
      if (mediaRecorder.value) {
        mediaRecorder.value.onstop = () => {
          // Create audio blob from chunks
          const audioBlob = new Blob(audioChunks.value, { type: 'audio/wav' })
          
          // Create audio URL
          const audioUrl = URL.createObjectURL(audioBlob)
          
          // Generate proper sequential recording number
          const getNextRecordingNumber = () => {
            if (recordings.value.length === 0) return 1
            
            // Find the highest recording number from existing recordings
            const existingNumbers = recordings.value
              .map(r => {
                const match = r.title.match(/Recording (\d+)/)
                return match ? parseInt(match[1]) : 0
              })
              .filter(num => num > 0)
            
            return existingNumbers.length > 0 ? Math.max(...existingNumbers) + 1 : 1
          }
          
          // Create recording entry with real audio
          const recording = {
            id: Date.now(),
            title: `Recording ${getNextRecordingNumber()}`,
            timestamp: new Date(),
            duration: formattedDuration.value,
            size: `${(audioBlob.size / (1024 * 1024)).toFixed(1)} MB`,
            audioBlob: audioBlob,
            audioUrl: audioUrl
          }
          
          recordings.value.unshift(recording)
          
          // Save recordings to storage for persistence
          saveRecordingsToStorage(recordings.value)
          
          // Show success message
          toastService.success('Recording saved!', `Recording ${recording.title} has been saved successfully`)
          
          resolve()
        }
      } else {
        resolve()
      }
    })
    
    setTimeout(() => {
      isProcessing.value = false
    }, 1000)
    
    console.log('Recording stopped and saved with real audio')
    
  } catch (error) {
    console.error('Failed to stop recording:', error)
    toastService.error('Recording failed', 'Could not save recording')
    isProcessing.value = false
  }
}

const toggleRecording = () => {
  if (isRecording.value) {
    stopRecording()
  } else {
    startRecording()
  }
}

const playRecording = (recordingId) => {
  const recording = recordings.value.find(r => r.id === recordingId)
  if (!recording || !recording.audioUrl) {
    toastService.error('Playback failed', 'Audio data not available for this recording')
    return
  }
  
  try {
    // Stop current audio if playing
    if (currentAudio.value) {
      currentAudio.value.pause()
      currentAudio.value = null
    }
    
    const audio = new Audio(recording.audioUrl)
    currentAudio.value = { id: recordingId, audio }
    
    // Add visual feedback
    const recordingElement = document.querySelector(`[data-recording-id="${recordingId}"]`)
    if (recordingElement) {
      recordingElement.classList.add('playing')
    }
    
    audio.onended = () => {
      console.log('Playback completed')
      recordingElement.classList.remove('playing')
      toastService.info('Playback complete', 'Recording playback finished')
      currentAudio.value = null
    }
    
    audio.onerror = (error) => {
      console.error('Audio error:', error)
      toastService.error('Playback failed', 'Error playing audio')
      recordingElement.classList.remove('playing')
      currentAudio.value = null
    }
    
    audio.play()
    console.log('Audio playback started for recording:', recording.title)
    
  } catch (error) {
    console.error('Error playing recording:', error)
    toastService.error('Playback failed', 'An error occurred while playing the recording')
    
    // Remove visual feedback on error
    const recordingElement = document.querySelector(`[data-recording-id="${recordingId}"]`)
    if (recordingElement) {
      recordingElement.classList.remove('playing')
    }
  }
}

const deleteRecording = (recordingId) => {
  recordingToDelete.value = recordingId
  showDeleteModal.value = true
}

const confirmDelete = () => {
  if (recordingToDelete.value) {
    recordings.value = recordings.value.filter(r => r.id !== recordingToDelete.value)
    
    // Renumber all recordings after deletion to maintain sequential order
    renumberRecordings()
    
    // Save recordings to storage after deletion and renumbering
    saveRecordingsToStorage(recordings.value)
    
    console.log('Recording deleted and recordings renumbered:', recordingToDelete.value)
    toastService.success('Recording deleted', 'The recording has been successfully removed')
  }
  showDeleteModal.value = false
  recordingToDelete.value = null
}

const cancelDelete = () => {
  showDeleteModal.value = false
  recordingToDelete.value = null
}

const formatDate = (dateString) => {
  return new Date(dateString).toLocaleString('en-US', {
    year: 'numeric',
    month: 'short',
    day: 'numeric',
    hour: '2-digit',
    minute: '2-digit'
  })
}

const getRandomBarHeight = () => {
  return Math.random() * 100
}

const getRecordingAvatarClass = (index) => {
  const colors = ['avatar-blue', 'avatar-green', 'avatar-purple', 'avatar-orange', 'avatar-teal', 'avatar-pink']
  return colors[index % colors.length]
}

// Helper function to renumber recordings sequentially
const renumberRecordings = () => {
  recordings.value = recordings.value.map((recording, index) => ({
    ...recording,
    title: `Recording ${index + 1}`
  }))
}

// Storage functions
const saveRecordingsToStorage = (recordings) => {
  try {
    const recordingsToSave = recordings.map(recording => ({
      id: recording.id,
      title: recording.title,
      timestamp: recording.timestamp,
      duration: recording.duration,
      size: recording.size,
      audioData: recording.audioBlob ? null : null // Audio blob cannot be directly stored
    }))
    localStorage.setItem('s2t-recordings', JSON.stringify(recordingsToSave))
    cookieService.setJSON('recordings', recordingsToSave)
    console.log(`Saved ${recordings.length} recordings to storage`)
  } catch (error) {
    console.error('Failed to save recordings to storage:', error)
    try {
      cookieService.setJSON('recordings', recordings)
    } catch (cookieError) {
      console.error('Failed to save recordings to cookies:', cookieError)
    }
  }
}

const loadRecordingsFromStorage = () => {
  try {
    let savedRecordings = null
    
    // Try to load from localStorage first
    try {
      const localStorageData = localStorage.getItem('s2t-recordings')
      if (localStorageData) {
        savedRecordings = JSON.parse(localStorageData)
      }
    } catch (error) {
      console.warn('Failed to load recordings from localStorage:', error)
    }
    
    // Fallback to cookies if localStorage is empty
    if (!savedRecordings || savedRecordings.length === 0) {
      savedRecordings = cookieService.getJSON('recordings', [])
    }
    
    if (savedRecordings && savedRecordings.length > 0) {
      // Convert saved recordings back to the format expected by the component
      const loadedRecordings = savedRecordings.map((savedRecording, index) => ({
        ...savedRecording,
        timestamp: new Date(savedRecording.timestamp),
        // Ensure proper sequential numbering for loaded recordings
        title: `Recording ${index + 1}`,
        // Note: audioBlob and audioUrl will be null for loaded recordings
        // This is a limitation of browser storage - we can't persist actual audio data
        // In a real app, you'd upload to a server or use IndexedDB for binary data
        audioBlob: null,
        audioUrl: null
      }))
      
      recordings.value = loadedRecordings
      
      // Renumber all recordings to ensure proper sequential order
      renumberRecordings()
      
      console.log(`Loaded ${loadedRecordings.length} recordings from storage with proper numbering`)
    }
  } catch (error) {
    console.error('Failed to load recordings from storage:', error)
  }
}

// Lifecycle
onMounted(async () => {
  await checkMicrophonePermission()
  loadRecordingsFromStorage()
  console.log('Recording management mounted')
})

onUnmounted(() => {
  // Clean up any ongoing recording
  if (isRecording.value) {
    stopRecording()
  }
  
  // Clean up audio
  if (currentAudio.value) {
    currentAudio.value.pause()
    currentAudio.value = null
  }
  
  // Clear interval
  if (durationInterval.value) {
    clearInterval(durationInterval.value)
  }
})
</script>

<style scoped>
.recording-management {
  max-width: 1400px;
  margin: 0 auto;
  padding: 2rem;
}

.page-header {
  text-align: center;
  margin-bottom: 3rem;
  background: rgba(255, 255, 255, 0.1);
  backdrop-filter: blur(20px);
  padding: 2rem;
  border-radius: 20px;
  box-shadow: 
    0 8px 32px rgba(0,0,0,0.1),
    inset 0 1px 0 rgba(255,255,255,0.2);
  border: 1px solid rgba(255, 255, 255, 0.2);
}

.page-header h2 {
  color: white;
  margin: 0 0 1rem 0;
  font-size: 2.5rem;
  font-weight: 700;
  text-shadow: 0 2px 10px rgba(0,0,0,0.5);
}

.page-header p {
  color: rgba(255, 255, 255, 0.9);
  font-size: 1.2rem;
  margin: 0;
  text-shadow: 0 1px 5px rgba(0,0,0,0.3);
}

.recording-status-card {
  background: rgba(255, 255, 255, 0.1);
  backdrop-filter: blur(20px);
  padding: 2rem;
  border-radius: 20px;
  box-shadow: 
    0 8px 32px rgba(0,0,0,0.1),
    inset 0 1px 0 rgba(255,255,255,0.2);
  border: 1px solid rgba(255, 255, 255, 0.2);
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 2rem;
}

.status-indicator {
  display: flex;
  align-items: center;
  gap: 1rem;
}

.status-indicator--recording {
  animation: pulse 2s infinite;
}

@keyframes pulse {
  0% { opacity: 1; }
  50% { opacity: 0.7; }
  100% { opacity: 1; }
}

.status-icon {
  width: 50px;
  height: 50px;
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  transition: all 0.3s ease;
  box-shadow: 0 4px 15px rgba(0,0,0,0.2);
}

.status-icon--recording {
  background: linear-gradient(135deg, #ff6b6b 0%, #ee5a24 100%);
  animation: recordingPulse 2s infinite;
}

.status-icon--processing {
  background: linear-gradient(135deg, #ffa726 0%, #ff9800 100%);
  animation: processingSpin 1s linear infinite;
}

.status-icon svg {
  width: 24px;
  height: 24px;
  color: white;
  filter: drop-shadow(0 2px 4px rgba(0,0,0,0.3));
}

@keyframes recordingPulse {
  0% { transform: scale(1); box-shadow: 0 4px 15px rgba(255, 107, 107, 0.4); }
  50% { transform: scale(1.1); box-shadow: 0 6px 20px rgba(255, 107, 107, 0.6); }
  100% { transform: scale(1); box-shadow: 0 4px 15px rgba(255, 107, 107, 0.4); }
}

@keyframes processingSpin {
  0% { transform: rotate(0deg); }
  100% { transform: rotate(360deg); }
}

.status-text {
  color: white;
  font-size: 1.2rem;
  font-weight: 600;
  text-shadow: 0 2px 8px rgba(0,0,0,0.3);
}

.recording-timer {
  background: rgba(255, 255, 255, 0.2);
  padding: 1rem 2rem;
  border-radius: 50px;
  border: 2px solid rgba(255, 255, 255, 0.3);
}

.timer-text {
  color: white;
  font-size: 1.5rem;
  font-weight: 700;
  font-family: 'Courier New', monospace;
  text-shadow: 0 2px 8px rgba(0,0,0,0.3);
}

.recording-controls {
  display: flex;
  justify-content: center;
  margin-bottom: 3rem;
}

.record-button {
  display: flex;
  align-items: center;
  gap: 1rem;
  padding: 1.5rem 3rem;
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  color: white;
  border: none;
  border-radius: 50px;
  font-size: 1.2rem;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.3s ease;
  box-shadow: 0 8px 25px rgba(102, 126, 234, 0.3);
  text-shadow: 0 1px 3px rgba(0,0,0,0.3);
  position: relative;
  overflow: hidden;
}

.record-button::before {
  content: '';
  position: absolute;
  top: 0;
  left: -100%;
  width: 100%;
  height: 100%;
  background: linear-gradient(90deg, transparent, rgba(255,255,255,0.2), transparent);
  transition: left 0.5s;
}

.record-button:hover::before {
  left: 100%;
}

.record-button:hover:not(:disabled) {
  transform: translateY(-3px);
  box-shadow: 0 12px 35px rgba(102, 126, 234, 0.4);
}

.record-button--recording {
  background: linear-gradient(135deg, #ff6b6b 0%, #ee5a24 100%);
  box-shadow: 0 8px 25px rgba(255, 107, 107, 0.3);
  animation: recordingPulse 2s infinite;
}

.record-button--recording:hover:not(:disabled) {
  box-shadow: 0 12px 35px rgba(255, 107, 107, 0.4);
}

.record-button--processing {
  background: linear-gradient(135deg, #ffa726 0%, #ff9800 100%);
  box-shadow: 0 8px 25px rgba(255, 167, 38, 0.3);
  animation: processingSpin 1s linear infinite;
}

.record-button--processing:hover:not(:disabled) {
  box-shadow: 0 12px 35px rgba(255, 167, 38, 0.4);
}

.button-icon {
  display: flex;
  align-items: center;
  justify-content: center;
}

.button-icon svg {
  width: 1.5rem;
  height: 1.5rem;
  filter: drop-shadow(0 2px 4px rgba(0,0,0,0.2));
}

.audio-visualization {
  background: rgba(255, 255, 255, 0.1);
  backdrop-filter: blur(20px);
  padding: 2rem;
  border-radius: 20px;
  box-shadow: 
    0 8px 32px rgba(0,0,0,0.1),
    inset 0 1px 0 rgba(255,255,255,0.2);
  border: 1px solid rgba(255, 255, 255, 0.2);
  margin-bottom: 3rem;
  text-align: center;
}

.visualization-header {
  display: flex;
  align-items: center;
  gap: 1rem;
  margin-bottom: 1.5rem;
}

.visualization-icon {
  width: 40px;
  height: 40px;
  background: linear-gradient(135deg, #4ecdc4 0%, #44a08d 100%);
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  box-shadow: 0 4px 15px rgba(78, 205, 196, 0.4);
}

.visualization-icon svg {
  width: 20px;
  height: 20px;
  color: white;
  filter: drop-shadow(0 2px 4px rgba(0,0,0,0.3));
}

.visualization-title {
  color: white;
  font-size: 1.2rem;
  font-weight: 600;
  text-shadow: 0 2px 8px rgba(0,0,0,0.3);
}

.audio-bars {
  display: flex;
  justify-content: center;
  align-items: end;
  gap: 0.25rem;
  height: 100px;
}

.audio-bar {
  width: 8px;
  background: linear-gradient(to top, #667eea, #764ba2);
  border-radius: 4px 4px 0 0;
  transition: height 0.1s ease;
  animation: audioWave 0.5s ease-in-out infinite alternate;
}

.audio-bar--active {
  background: linear-gradient(to top, #ff6b6b, #ee5a24);
  animation: audioWaveActive 0.3s ease-in-out infinite alternate;
}

@keyframes audioWave {
  0% { opacity: 0.7; }
  100% { opacity: 1; }
}

@keyframes audioWaveActive {
  0% { opacity: 0.8; transform: scaleY(1); }
  100% { opacity: 1; transform: scaleY(1.1); }
}

.recording-history {
  background: rgba(255, 255, 255, 0.1);
  backdrop-filter: blur(20px);
  padding: 2rem;
  border-radius: 20px;
  box-shadow: 
    0 8px 32px rgba(0,0,0,0.1),
    inset 0 1px 0 rgba(255,255,255,0.2);
  border: 1px solid rgba(255, 255, 255, 0.2);
}

.history-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 2rem;
}

.history-title-section {
  display: flex;
  align-items: center;
  gap: 1rem;
}

.history-icon {
  width: 50px;
  height: 50px;
  background: linear-gradient(135deg, #a8e6cf 0%, #7fcdcd 100%);
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  box-shadow: 0 4px 15px rgba(168, 230, 207, 0.4);
  flex-shrink: 0;
}

.history-icon svg {
  width: 24px;
  height: 24px;
  color: white;
  filter: drop-shadow(0 2px 4px rgba(0,0,0,0.3));
}

.history-header h3 {
  color: white;
  margin: 0;
  font-size: 1.8rem;
  font-weight: 700;
  text-shadow: 0 2px 8px rgba(0,0,0,0.3);
  line-height: 1.2;
}

.history-stats {
  display: flex;
  gap: 1rem;
  align-items: center;
}

.stat-item {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  background: rgba(255, 255, 255, 0.1);
  padding: 0.5rem 1rem;
  border-radius: 20px;
  border: 1px solid rgba(255, 255, 255, 0.2);
  height: fit-content;
}

.stat-icon {
  width: 20px;
  height: 20px;
  background: linear-gradient(135deg, #ff9a9e 0%, #fecfef 100%);
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  flex-shrink: 0;
}

.stat-icon svg {
  width: 12px;
  height: 12px;
  color: white;
  filter: drop-shadow(0 1px 2px rgba(0,0,0,0.3));
}

.stat {
  color: rgba(255, 255, 255, 0.9);
  font-size: 0.9rem;
  font-weight: 500;
  text-shadow: 0 1px 5px rgba(0,0,0,0.2);
  white-space: nowrap;
}

.recordings-list {
  display: grid;
  gap: 1rem;
}

.recording-item {
  background: rgba(255, 255, 255, 0.05);
  backdrop-filter: blur(10px);
  padding: 1.5rem;
  border-radius: 15px;
  border: 1px solid rgba(255, 255, 255, 0.1);
  display: flex;
  align-items: center;
  gap: 1.5rem;
  transition: all 0.3s ease;
  min-height: 80px;
}

.recording-item:hover {
  background: rgba(255, 255, 255, 0.1);
  border-color: rgba(255, 255, 255, 0.2);
  transform: translateY(-2px);
}

.recording-item--playing {
  background: rgba(102, 126, 234, 0.2);
  border-color: rgba(102, 126, 234, 0.4);
  animation: playingPulse 2s infinite;
}

@keyframes playingPulse {
  0% { box-shadow: 0 0 0 0 rgba(102, 126, 234, 0.4); }
  70% { box-shadow: 0 0 0 10px rgba(102, 126, 234, 0); }
  100% { box-shadow: 0 0 0 0 rgba(102, 126, 234, 0); }
}

.recording-info {
  flex: 1;
  display: flex;
  flex-direction: column;
  justify-content: center;
  min-height: 50px;
}

.recording-avatar {
  display: flex;
  align-items: center;
  flex-shrink: 0;
}

.avatar-circle {
  width: 50px;
  height: 50px;
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  box-shadow: 0 4px 15px rgba(0,0,0,0.2);
  transition: all 0.3s ease;
}

.recording-item:hover .avatar-circle {
  transform: scale(1.1);
  box-shadow: 0 6px 20px rgba(0,0,0,0.3);
}

.avatar-circle svg {
  width: 24px;
  height: 24px;
  color: white;
  filter: drop-shadow(0 2px 4px rgba(0,0,0,0.3));
}

/* Avatar color variations */
.avatar-blue { background: linear-gradient(135deg, #4ecdc4 0%, #44a08d 100%); }
.avatar-green { background: linear-gradient(135deg, #a8e6cf 0%, #7fcdcd 100%); }
.avatar-purple { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); }
.avatar-orange { background: linear-gradient(135deg, #ff6b6b 0%, #ee5a24 100%); }
.avatar-teal { background: linear-gradient(135deg, #26d0ce 0%, #1a2980 100%); }
.avatar-pink { background: linear-gradient(135deg, #ff9a9e 0%, #fecfef 100%); }

.recording-title {
  color: white;
  font-size: 1.2rem;
  font-weight: 600;
  margin-bottom: 0.5rem;
  text-shadow: 0 2px 8px rgba(0,0,0,0.3);
  line-height: 1.3;
}

.recording-meta {
  display: flex;
  gap: 1.5rem;
  flex-wrap: wrap;
  align-items: center;
}

.meta-item {
  display: flex;
  align-items: center;
  gap: 0.5rem;
}

.meta-icon {
  width: 16px;
  height: 16px;
  color: rgba(255, 255, 255, 0.7);
  filter: drop-shadow(0 1px 2px rgba(0,0,0,0.2));
  flex-shrink: 0;
}

.recording-date,
.recording-duration,
.recording-size {
  color: rgba(255, 255, 255, 0.7);
  font-size: 0.9rem;
  text-shadow: 0 1px 5px rgba(0,0,0,0.2);
  line-height: 1.2;
}

.recording-actions {
  display: flex;
  gap: 0.75rem;
  align-items: center;
  flex-shrink: 0;
}

.action-button {
  width: 40px;
  height: 40px;
  border: none;
  border-radius: 50%;
  cursor: pointer;
  transition: all 0.3s ease;
  display: flex;
  align-items: center;
  justify-content: center;
}

.play-button {
  background: rgba(78, 205, 196, 0.2);
  color: white;
  border: 1px solid rgba(78, 205, 196, 0.4);
}

.play-button:hover:not(:disabled) {
  background: rgba(78, 205, 196, 0.3);
  border-color: rgba(78, 205, 196, 0.6);
  transform: scale(1.1);
  box-shadow: 0 4px 12px rgba(78, 205, 196, 0.3);
}

.delete-button {
  background: rgba(255, 107, 107, 0.2);
  color: white;
  border: 1px solid rgba(255, 107, 107, 0.4);
}

.delete-button:hover {
  background: rgba(255, 107, 107, 0.3);
  border-color: rgba(255, 107, 107, 0.6);
  transform: scale(1.1);
  box-shadow: 0 4px 12px rgba(255, 107, 107, 0.3);
}

.action-button:disabled {
  opacity: 0.5;
  cursor: not-allowed;
  transform: none;
}

.action-button svg {
  width: 1rem;
  height: 1rem;
  filter: drop-shadow(0 1px 2px rgba(0,0,0,0.2));
}

.empty-state {
  text-align: center;
  background: rgba(255, 255, 255, 0.1);
  backdrop-filter: blur(20px);
  padding: 3rem;
  border-radius: 20px;
  box-shadow: 
    0 8px 32px rgba(0,0,0,0.1),
    inset 0 1px 0 rgba(255,255,255,0.2);
  border: 1px solid rgba(255, 255, 255, 0.2);
  margin-top: 2rem;
}

.empty-icon {
  width: 100px;
  height: 100px;
  margin: 0 auto 1.5rem;
  background: linear-gradient(135deg, #ff6b6b 0%, #ee5a24 100%);
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  box-shadow: 0 8px 25px rgba(255, 107, 107, 0.4);
  transition: all 0.3s ease;
}

.empty-icon:hover {
  transform: scale(1.1);
  box-shadow: 0 12px 35px rgba(255, 107, 107, 0.6);
}

.empty-icon-svg {
  width: 40px;
  height: 40px;
  color: white;
  filter: drop-shadow(0 2px 4px rgba(0,0,0,0.2));
}

.empty-state h3 {
  color: white;
  margin-bottom: 0.5rem;
  font-size: 1.8rem;
  font-weight: 700;
  text-shadow: 0 2px 8px rgba(0,0,0,0.3);
}

.empty-state p {
  color: rgba(255, 255, 255, 0.8);
  margin: 0;
  font-size: 1.1rem;
  font-weight: 500;
  text-shadow: 0 1px 5px rgba(0,0,0,0.2);
}

/* Responsive design */
@media (max-width: 768px) {
  .recording-management {
    padding: 1rem;
  }
  
  .recording-status-card {
    flex-direction: column;
    gap: 1rem;
    text-align: center;
  }
  
  .record-button {
    padding: 1rem 2rem;
    font-size: 1rem;
  }
  
  .recording-item {
    flex-direction: column;
    gap: 1rem;
    align-items: stretch;
  }
  
  .recording-actions {
    justify-content: center;
  }
  
  .recording-meta {
    justify-content: center;
  }
}
</style>
