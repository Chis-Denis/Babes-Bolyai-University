<template>
  <div class="main-view">
    <!-- Header with Navigation -->
    <HeaderBar 
      :current-section="currentSection"
      @section-change="changeSection"
    />
    
    <!-- Main Content Area -->
    <main class="main-content">
      <!-- Home Section -->
      <div v-if="currentSection === 'home'" class="section-content">
        <div class="welcome-section">
          <h2>Welcome to Speech-to-Text Medical System</h2>
          <p>Transform healthcare documentation with AI-powered voice recognition technology</p>
          
          <div class="action-buttons">
            <button @click="changeSection('patients')" class="btn btn-primary patient-btn">
              <span class="btn-icon">👥</span>
              Manage Patients
            </button>
            <button @click="changeSection('recording')" class="btn btn-secondary recording-btn">
              <span class="btn-icon">🎙️</span>
              Start Recording
            </button>
            <button @click="changeSection('documents')" class="btn btn-secondary document-btn">
              <span class="btn-icon">📄</span>
              Document Templates
            </button>
          </div>
        </div>
        
        <div class="features-grid">
          <div class="feature-card recording-card">
            <div class="feature-icon recording-icon">
              <svg viewBox="0 0 24 24" fill="currentColor">
                <path d="M12 14c1.66 0 3-1.34 3-3V5c0-1.66-1.34-3-3-3S9 3.34 9 5v6c0 1.66 1.34 3 3 3z"/>
                <path d="M17 11c0 2.76-2.24 5-5 5s-5-2.24-5-5H5c0 3.53 2.61 6.43 6 6.92V21h2v-3.08c3.39-.49 6-3.39 6-6.92h-2z"/>
              </svg>
            </div>
            <h3>Voice Recording</h3>
            <p>Record medical observations with high-quality audio capture and advanced noise reduction technology</p>
          </div>
          <div class="feature-card document-card">
            <div class="feature-icon document-icon">
              <svg viewBox="0 0 24 24" fill="currentColor">
                <path d="M14,2H6A2,2 0 0,0 4,4V20A2,2 0 0,0 6,22H18A2,2 0 0,0 20,20V8L14,2M18,20H6V4H13V9H18V20Z"/>
              </svg>
            </div>
            <h3>Smart Document Processing</h3>
            <p>Intelligently process transcribed audio into structured medical documents with AI-powered data extraction</p>
          </div>
          <div class="feature-card patient-card">
            <div class="feature-icon patient-icon">
              <svg viewBox="0 0 24 24" fill="currentColor">
                <path d="M16 7a4 4 0 11-8 0 4 4 0 018 0zM12 14a7 7 0 00-7 7h14a7 7 0 00-7-7z"/>
              </svg>
            </div>
            <h3>Patient Management</h3>
            <p>Comprehensive patient record management with secure data storage and easy access to medical history</p>
          </div>
        </div>
      </div>

      <!-- Patients Section -->
      <div v-if="currentSection === 'patients'" class="section-content">
        <PatientManagement />
      </div>

      <!-- Recording Section -->
      <div v-if="currentSection === 'recording'" class="section-content">
        <RecordingManagement />
      </div>

      <!-- Documents Section -->
      <div v-if="currentSection === 'documents'" class="section-content">
        <DocumentManagement />
      </div>
    </main>

    <!-- Global Toast Notifications -->
    <GlobalToast position="top-left" />
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import HeaderBar from '@/components/layout/HeaderBar.vue'
import GlobalToast from '@/components/common/GlobalToast.vue'
import PatientManagement from '@/components/sections/PatientManagement.vue'
import RecordingManagement from '@/components/sections/RecordingManagement.vue'
import DocumentManagement from '@/components/sections/DocumentManagement.vue'
import { cookieService } from '@/services/CookieService.js'
import { errorService } from '@/services/ErrorService.js'

// State
const currentSection = ref('home')

// Methods
const changeSection = (section) => {
  currentSection.value = section
  // Save current section to localStorage for persistence
  localStorage.setItem('s2t-current-section', section)
}

// Lifecycle
onMounted(() => {
  // Initialize services
  cookieService.cleanup()
  console.log('Error service initialized')
  
  // Set up global error handler
  window.addEventListener('unhandledrejection', (event) => {
    errorService.handleError(event.reason, 'unhandledPromiseRejection')
  })
  
  window.addEventListener('error', (event) => {
    errorService.handleError(event.error, 'globalError')
  })
  
  // Restore last section from localStorage
  const savedSection = localStorage.getItem('s2t-current-section')
  if (savedSection && ['home', 'patients', 'recording', 'documents'].includes(savedSection)) {
    currentSection.value = savedSection
  }
})
</script>

<style scoped>
.main-view {
  min-height: 100vh;
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  position: relative;
  overflow: hidden;
}

.main-view::before {
  content: '';
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: 
    radial-gradient(circle at 20% 80%, rgba(120, 119, 198, 0.3) 0%, transparent 50%),
    radial-gradient(circle at 80% 20%, rgba(255, 255, 255, 0.1) 0%, transparent 50%),
    radial-gradient(circle at 40% 40%, rgba(120, 119, 198, 0.2) 0%, transparent 50%);
  animation: float 20s ease-in-out infinite;
}

@keyframes float {
  0%, 100% { transform: translateY(0px) rotate(0deg); }
  33% { transform: translateY(-20px) rotate(1deg); }
  66% { transform: translateY(10px) rotate(-1deg); }
}

.main-content {
  max-width: 1400px;
  margin: 0 auto;
  padding: 2rem;
  position: relative;
  z-index: 1;
}

.section-content {
  animation: fadeIn 0.3s ease-in-out;
}

@keyframes fadeIn {
  from { opacity: 0; transform: translateY(20px); }
  to { opacity: 1; transform: translateY(0); }
}

/* Home Section Styles */
.welcome-section {
  text-align: center;
  margin-bottom: 4rem;
  background: rgba(255, 255, 255, 0.1);
  backdrop-filter: blur(20px);
  padding: 3rem;
  border-radius: 20px;
  box-shadow: 
    0 8px 32px rgba(0,0,0,0.1),
    inset 0 1px 0 rgba(255,255,255,0.2);
  border: 1px solid rgba(255, 255, 255, 0.2);
}

.welcome-section h2 {
  color: white;
  font-size: 2.5rem;
  font-weight: 700;
  margin-bottom: 1rem;
  text-shadow: 0 2px 10px rgba(0,0,0,0.5);
}

.welcome-section p {
  color: rgba(255, 255, 255, 0.9);
  font-size: 1.2rem;
  margin-bottom: 2rem;
  text-shadow: 0 1px 5px rgba(0,0,0,0.3);
}

.action-buttons {
  display: flex;
  gap: 1rem;
  justify-content: center;
  flex-wrap: wrap;
}

.btn {
  padding: 1rem 2rem;
  border-radius: 12px;
  font-weight: 600;
  font-size: 1rem;
  cursor: pointer;
  transition: all 0.3s ease;
  border: none;
  text-decoration: none;
  display: inline-flex;
  align-items: center;
  justify-content: center;
  position: relative;
  overflow: hidden;
  backdrop-filter: blur(20px);
  text-shadow: 0 1px 3px rgba(0,0,0,0.3);
}

.btn::before {
  content: '';
  position: absolute;
  top: 0;
  left: -100%;
  width: 100%;
  height: 100%;
  background: linear-gradient(90deg, transparent, rgba(255,255,255,0.2), transparent);
  transition: left 0.5s;
}

.btn:hover::before {
  left: 100%;
}

.btn-icon {
  margin-right: 0.5rem;
  font-size: 1.2rem;
}

.btn-primary {
  background: rgba(168, 230, 207, 0.2);
  color: white;
  border: 2px solid rgba(168, 230, 207, 0.4);
}

.btn-primary:hover {
  background: rgba(168, 230, 207, 0.3);
  border-color: rgba(168, 230, 207, 0.6);
  transform: translateY(-2px);
  box-shadow: 0 8px 25px rgba(168, 230, 207, 0.3);
}

.btn-secondary {
  background: rgba(102, 126, 234, 0.2);
  color: white;
  border: 2px solid rgba(102, 126, 234, 0.4);
}

.btn-secondary:hover {
  background: rgba(102, 126, 234, 0.3);
  border-color: rgba(102, 126, 234, 0.6);
  transform: translateY(-2px);
  box-shadow: 0 8px 25px rgba(102, 126, 234, 0.3);
}

/* Specific button colors */
.recording-btn {
  background: rgba(255, 107, 107, 0.2);
  border: 2px solid rgba(255, 107, 107, 0.4);
}

.recording-btn:hover {
  background: rgba(255, 107, 107, 0.3);
  border-color: rgba(255, 107, 107, 0.6);
  box-shadow: 0 8px 25px rgba(255, 107, 107, 0.3);
}

.document-btn {
  background: rgba(78, 205, 196, 0.2);
  border: 2px solid rgba(78, 205, 196, 0.4);
}

.document-btn:hover {
  background: rgba(78, 205, 196, 0.3);
  border-color: rgba(78, 205, 196, 0.6);
  box-shadow: 0 8px 25px rgba(78, 205, 196, 0.3);
}

.features-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
  gap: 2rem;
}

.feature-card {
  background: rgba(255, 255, 255, 0.1);
  backdrop-filter: blur(20px);
  padding: 2rem;
  border-radius: 20px;
  box-shadow: 
    0 8px 32px rgba(0,0,0,0.1),
    inset 0 1px 0 rgba(255,255,255,0.2);
  border: 1px solid rgba(255, 255, 255, 0.2);
  text-align: center;
  transition: all 0.3s ease;
}

.feature-card:hover {
  transform: translateY(-5px);
  box-shadow: 
    0 15px 40px rgba(0,0,0,0.15),
    inset 0 1px 0 rgba(255,255,255,0.3);
}

.feature-icon {
  width: 60px;
  height: 60px;
  margin: 0 auto 1.5rem;
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  transition: all 0.3s ease;
}

/* Recording Icon - Orange/Red gradient */
.recording-icon {
  background: linear-gradient(135deg, #ff6b6b 0%, #ee5a24 100%);
  box-shadow: 0 4px 15px rgba(255, 107, 107, 0.4);
}

.recording-card:hover .recording-icon {
  transform: scale(1.1);
  box-shadow: 0 6px 20px rgba(255, 107, 107, 0.6);
}

/* Document Icon - Blue gradient */
.document-icon {
  background: linear-gradient(135deg, #4ecdc4 0%, #44a08d 100%);
  box-shadow: 0 4px 15px rgba(78, 205, 196, 0.4);
}

.document-card:hover .document-icon {
  transform: scale(1.1);
  box-shadow: 0 6px 20px rgba(78, 205, 196, 0.6);
}

/* Patient Icon - Green gradient */
.patient-icon {
  background: linear-gradient(135deg, #a8e6cf 0%, #7fcdcd 100%);
  box-shadow: 0 4px 15px rgba(168, 230, 207, 0.4);
}

.patient-card:hover .patient-icon {
  transform: scale(1.1);
  box-shadow: 0 6px 20px rgba(168, 230, 207, 0.6);
}

.feature-icon svg {
  width: 30px;
  height: 30px;
  color: white;
  filter: drop-shadow(0 2px 4px rgba(0,0,0,0.2));
}

.feature-card h3 {
  color: white;
  font-size: 1.5rem;
  font-weight: 700;
  margin-bottom: 1rem;
  text-shadow: 0 2px 8px rgba(0,0,0,0.3);
}

.feature-card p {
  color: rgba(255, 255, 255, 0.8);
  line-height: 1.6;
  text-shadow: 0 1px 5px rgba(0,0,0,0.2);
}

/* Responsive Design */
@media (max-width: 768px) {
  .main-content {
    padding: 1rem;
  }
  
  .welcome-section {
    padding: 2rem 1rem;
  }
  
  .welcome-section h2 {
    font-size: 2rem;
  }
  
  .action-buttons {
    flex-direction: column;
    align-items: center;
  }
  
  .btn {
    width: 100%;
    max-width: 300px;
  }
  
  .features-grid {
    grid-template-columns: 1fr;
  }
}
</style>
