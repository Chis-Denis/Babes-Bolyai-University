<template>
  <teleport to="body">
    <div class="toast-container" :class="`toast-container--${position}`">
      <transition-group name="toast" tag="div">
        <div
          v-for="toast in toasts"
          :key="toast.id"
          :class="getToastClasses(toast)"
          @click="handleToastClick(toast)"
        >
          <div class="toast-icon">
            <span v-if="toast.type === 'loading'" class="loading-spinner">⏳</span>
            <span v-else>{{ toast.icon }}</span>
          </div>
          
          <div class="toast-content">
            <div v-if="toast.title" class="toast-title">{{ toast.title }}</div>
            <div class="toast-message">{{ toast.message }}</div>
            
            <!-- Toast Actions -->
            <div v-if="toast.actions && toast.actions.length > 0" class="toast-actions">
              <button
                v-for="action in toast.actions"
                :key="action.label"
                :class="getActionClasses(action)"
                @click.stop="handleAction(toast, action)"
              >
                {{ action.label }}
              </button>
            </div>
          </div>
          
          <button 
            v-if="toast.closable" 
            class="toast-close" 
            @click.stop="removeToast(toast.id)"
            aria-label="Închide"
          >
            ✕
          </button>
        </div>
      </transition-group>
    </div>
  </teleport>
</template>

<script setup>
import { computed, onMounted, onUnmounted } from 'vue'
import { toastService } from '@/services/ToastService.js'

// Props
const props = defineProps({
  position: {
    type: String,
    default: 'top-right',
    validator: (value) => ['top-right', 'top-left', 'bottom-right', 'bottom-left', 'top-center', 'bottom-center'].includes(value)
  }
})

// Data
const toasts = computed(() => toastService.getToastsByPosition(props.position))

// Methods
const getToastClasses = (toast) => {
  return [
    'toast',
    `toast--${toast.type}`,
    {
      'toast--persistent': toast.persistent,
      'toast--clickable': toast.actions && toast.actions.length > 0
    }
  ]
}

const getActionClasses = (action) => {
  return [
    'toast-action',
    `toast-action--${action.variant || 'primary'}`
  ]
}

const handleToastClick = (toast) => {
  if (toast.onClick) {
    toast.onClick(toast)
  }
}

const handleAction = (toast, action) => {
  if (action.handler) {
    action.handler(toast, action)
  }
  if (action.closeOnClick !== false) {
    removeToast(toast.id)
  }
}

const removeToast = (toastId) => {
  toastService.remove(toastId)
}

// Lifecycle
onMounted(() => {
  // Initialize toast service if needed
})

onUnmounted(() => {
  // Cleanup if needed
})
</script>

<style scoped>
.toast-container {
  position: fixed;
  z-index: 1000;
  display: flex;
  flex-direction: column;
  gap: 0.5rem;
  max-width: 250px;
  width: 100%;
  padding: 0.5rem;
  pointer-events: none;
}

.toast-container--top-right {
  top: 0.5rem;
  right: 0.5rem;
}

.toast-container--top-left {
  top: 0.5rem;
  left: 0.5rem;
}

.toast-container--bottom-right {
  bottom: 1rem;
  right: 1rem;
  flex-direction: column-reverse;
}

.toast-container--bottom-left {
  bottom: 1rem;
  left: 1rem;
  flex-direction: column-reverse;
}

.toast-container--top-center {
  top: 1rem;
  left: 50%;
  transform: translateX(-50%);
}

.toast-container--bottom-center {
  bottom: 1rem;
  left: 50%;
  transform: translateX(-50%);
  flex-direction: column-reverse;
}

.toast {
  display: flex;
  align-items: flex-start;
  gap: 0.4rem;
  padding: 0.5rem;
  background: white;
  border-radius: 4px;
  box-shadow: 0 4px 6px -1px rgba(0, 0, 0, 0.1), 0 2px 4px -1px rgba(0, 0, 0, 0.06);
  border-left: 2px solid;
  cursor: default;
  transition: all 0.3s ease;
  position: relative;
  overflow: hidden;
  pointer-events: auto;
  min-width: 180px;
  max-width: 220px;
}

.toast--clickable {
  cursor: pointer;
}

.toast--clickable:hover {
  transform: translateY(-2px);
  box-shadow: 0 8px 12px -1px rgba(0, 0, 0, 0.15), 0 4px 6px -1px rgba(0, 0, 0, 0.1);
}

.toast--success {
  border-left-color: #10b981;
  background: #f0fdf4;
}

.toast--error {
  border-left-color: #ef4444;
  background: #fef2f2;
}

.toast--warning {
  border-left-color: #f59e0b;
  background: #fffbeb;
}

.toast--info {
  border-left-color: #3b82f6;
  background: #eff6ff;
}

.toast--loading {
  border-left-color: #6b7280;
  background: #f9fafb;
}

.toast--confirm {
  border-left-color: #8b5cf6;
  background: #faf5ff;
}

.toast-icon {
  flex-shrink: 0;
  font-size: 0.8rem;
  line-height: 1;
  margin-top: 0.125rem;
}

.loading-spinner {
  animation: spin 1s linear infinite;
}

@keyframes spin {
  from { transform: rotate(0deg); }
  to { transform: rotate(360deg); }
}

.toast-content {
  flex: 1;
  min-width: 0;
}

.toast-title {
  font-weight: 600;
  font-size: 0.7rem;
  color: #1f2937;
  margin-bottom: 0.1rem;
}

.toast-message {
  font-size: 0.7rem;
  color: #6b7280;
  line-height: 1.2;
}

.toast-actions {
  display: flex;
  gap: 0.5rem;
  margin-top: 0.75rem;
}

.toast-action {
  padding: 0.375rem 0.75rem;
  border-radius: 4px;
  font-size: 0.75rem;
  font-weight: 500;
  cursor: pointer;
  transition: all 0.2s;
  border: none;
}

.toast-action--primary {
  background: #3b82f6;
  color: white;
}

.toast-action--primary:hover {
  background: #2563eb;
}

.toast-action--secondary {
  background: #6b7280;
  color: white;
}

.toast-action--secondary:hover {
  background: #4b5563;
}

.toast-action--success {
  background: #10b981;
  color: white;
}

.toast-action--success:hover {
  background: #059669;
}

.toast-action--danger {
  background: #ef4444;
  color: white;
}

.toast-action--danger:hover {
  background: #dc2626;
}

.toast-close {
  flex-shrink: 0;
  background: none;
  border: none;
  font-size: 0.7rem;
  color: #9ca3af;
  cursor: pointer;
  padding: 0.1rem;
  border-radius: 2px;
  transition: all 0.2s;
  line-height: 1;
}

.toast-close:hover {
  background: rgba(0, 0, 0, 0.05);
  color: #6b7280;
}

/* Toast transitions */
.toast-enter-active {
  transition: all 0.3s ease;
}

.toast-leave-active {
  transition: all 0.3s ease;
}

.toast-enter-from {
  opacity: 0;
  transform: translateX(-100%);
}

.toast-leave-to {
  opacity: 0;
  transform: translateX(-100%);
}

.toast-move {
  transition: transform 0.3s ease;
}

/* Dark mode support */
@media (prefers-color-scheme: dark) {
  .toast {
    background: #1f2937;
    color: #f9fafb;
  }
  
  .toast--success {
    background: #064e3b;
  }
  
  .toast--error {
    background: #7f1d1d;
  }
  
  .toast--warning {
    background: #78350f;
  }
  
  .toast--info {
    background: #1e3a8a;
  }
  
  .toast--loading {
    background: #374151;
  }
  
  .toast--confirm {
    background: #581c87;
  }
  
  .toast-title {
    color: #f9fafb;
  }
  
  .toast-message {
    color: #d1d5db;
  }
  
  .toast-close {
    color: #9ca3af;
  }
  
  .toast-close:hover {
    background: rgba(255, 255, 255, 0.1);
    color: #d1d5db;
  }
}

/* Mobile responsiveness */
@media (max-width: 640px) {
  .toast-container {
    left: 1rem;
    right: 1rem;
    max-width: none;
  }
  
  .toast {
    min-width: auto;
  }
  
  .toast-actions {
    flex-direction: column;
  }
  
  .toast-action {
    width: 100%;
  }
}
</style>

