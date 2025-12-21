<template>
  <header class="header-bar">
    <div class="header-content">
      <div class="logo-section">
        <div class="logo-icon" @click="navigateToSection('home')">
          <svg viewBox="0 0 24 24" fill="currentColor" class="microphone-icon">
            <path d="M12 14c1.66 0 3-1.34 3-3V5c0-1.66-1.34-3-3-3S9 3.34 9 5v6c0 1.66 1.34 3 3 3z"/>
            <path d="M17 11c0 2.76-2.24 5-5 5s-5-2.24-5-5H5c0 3.53 2.61 6.43 6 6.92V21h2v-3.08c3.39-.49 6-3.39 6-6.92h-2z"/>
          </svg>
        </div>
        <h1 class="app-title" @click="navigateToSection('home')">Speech-to-Text Medical</h1>
      </div>
      <nav class="navigation">
        <button 
          @click="navigateToSection('home')"
          :class="['nav-link', { 'nav-link--active': currentSection === 'home' }]"
        >
          <svg viewBox="0 0 24 24" fill="currentColor" class="nav-icon">
            <path d="M10 20v-6h4v6h5v-8h3L12 3 2 12h3v8z"/>
          </svg>
          <span class="nav-text">Home</span>
        </button>
        <button 
          @click="navigateToSection('patients')"
          :class="['nav-link', { 'nav-link--active': currentSection === 'patients' }]"
        >
          <svg viewBox="0 0 24 24" fill="currentColor" class="nav-icon">
            <path d="M16 4c0-1.11.89-2 2-2s2 .89 2 2-.89 2-2 2-2-.89-2-2zm4 18v-6h2.5l-2.54-7.63A1.5 1.5 0 0 0 18.54 8H17c-.8 0-1.54.37-2.01.99L14 10.5l-1.5-2.01A2.5 2.5 0 0 0 10.5 8H9.46c-.8 0-1.54.37-2.01.99L5 10.5l-1.5-2.01A2.5 2.5 0 0 0 1.5 8H.5v2h1c.28 0 .5.22.5.5s-.22.5-.5.5H.5v2h1c.28 0 .5.22.5.5s-.22.5-.5.5H.5v2h1c.28 0 .5.22.5.5s-.22.5-.5.5H.5V22h2.5v-6H5v6h2v-6h1.5v6H11v-6h1.5v6H15v-6h1.5v6H20z"/>
          </svg>
          <span class="nav-text">Patients</span>
        </button>
        <button 
          @click="navigateToSection('documents')"
          :class="['nav-link', { 'nav-link--active': currentSection === 'documents' }]"
        >
          <svg viewBox="0 0 24 24" fill="currentColor" class="nav-icon">
            <path d="M14,2H6A2,2 0 0,0 4,4V20A2,2 0 0,0 6,22H18A2,2 0 0,0 20,20V8L14,2M18,20H6V4H13V9H18V20Z"/>
          </svg>
          <span class="nav-text">Documents</span>
        </button>
      </nav>
    </div>
  </header>
</template>

<script setup>
import { defineProps, defineEmits } from 'vue'

// Props
const props = defineProps({
  currentSection: {
    type: String,
    default: 'home'
  }
})

// Emits
const emit = defineEmits(['section-change'])

// Methods
const navigateToSection = (section) => {
  emit('section-change', section)
}
</script>

<style scoped>
.header-bar {
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  color: white;
  padding: 1.5rem 0;
  box-shadow: 0 8px 32px rgba(0,0,0,0.15);
  position: sticky;
  top: 0;
  z-index: 1000;
  backdrop-filter: blur(20px);
  border-bottom: 1px solid rgba(255, 255, 255, 0.1);
}

.header-content {
  max-width: 1400px;
  margin: 0 auto;
  padding: 0 2rem;
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.logo-section {
  display: flex;
  align-items: center;
  gap: 0.75rem;
  cursor: pointer;
}

.logo-icon {
  width: 50px;
  height: 50px;
  background: rgba(255, 255, 255, 0.15);
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  transition: all 0.4s cubic-bezier(0.175, 0.885, 0.32, 1.275);
  border: 2px solid rgba(255, 255, 255, 0.2);
  backdrop-filter: blur(10px);
}

.logo-icon:hover {
  background: rgba(255, 255, 255, 0.25);
  transform: scale(1.1) rotate(5deg);
  box-shadow: 0 8px 25px rgba(255, 255, 255, 0.2);
}

.microphone-icon {
  width: 28px;
  height: 28px;
  transition: transform 0.3s ease;
}

.logo-icon:hover .microphone-icon {
  transform: scale(1.1);
}

.app-title {
  margin: 0;
  font-size: 1.8rem;
  font-weight: 700;
  text-shadow: 0 4px 20px rgba(0,0,0,0.3);
  background: linear-gradient(45deg, #ffffff, #f0f0f0);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
  letter-spacing: 0.5px;
  cursor: pointer;
  transition: all 0.3s ease;
}

.app-title:hover {
  transform: scale(1.05);
}

.navigation {
  display: flex;
  gap: 0.5rem;
}

.nav-link {
  color: white;
  text-decoration: none;
  font-weight: 600;
  padding: 0.75rem 1.5rem;
  border-radius: 50px;
  transition: all 0.4s cubic-bezier(0.175, 0.885, 0.32, 1.275);
  display: flex;
  align-items: center;
  gap: 0.75rem;
  position: relative;
  overflow: hidden;
  border: 2px solid transparent;
  backdrop-filter: blur(10px);
  letter-spacing: 0.5px;
  text-transform: uppercase;
  font-size: 0.9rem;
  background: none;
  cursor: pointer;
}

.nav-link::before {
  content: '';
  position: absolute;
  top: 0;
  left: -100%;
  width: 100%;
  height: 100%;
  background: linear-gradient(90deg, transparent, rgba(255,255,255,0.2), transparent);
  transition: left 0.5s;
}

.nav-link:hover::before {
  left: 100%;
}

.nav-link:hover {
  background: rgba(255, 255, 255, 0.15);
  border-color: rgba(255, 255, 255, 0.3);
  transform: translateY(-2px) scale(1.05);
  box-shadow: 0 8px 25px rgba(255, 255, 255, 0.2);
}

.nav-link.router-link-active,
.nav-link--active {
  background: rgba(255, 255, 255, 0.2);
  border-color: rgba(255, 255, 255, 0.4);
  box-shadow: 0 8px 25px rgba(255, 255, 255, 0.3);
}

.nav-icon {
  width: 22px;
  height: 22px;
  transition: transform 0.3s ease;
}

.nav-link:hover .nav-icon {
  transform: scale(1.15) rotate(5deg);
}

.nav-text {
  font-size: 0.9rem;
  letter-spacing: 0.5px;
  font-weight: 600;
}

@media (max-width: 768px) {
  .header-content {
    padding: 0 1rem;
  }
  
  .navigation {
    gap: 0.25rem;
  }
  
  .nav-link {
    padding: 0.5rem 0.75rem;
  }
  
  .nav-text {
    display: none;
  }
  
  .nav-icon {
    width: 20px;
    height: 20px;
  }
}
</style>
