<template>
  <button
    :class="buttonClasses"
    :disabled="disabled || loading"
    @click="handleClick"
    :type="type"
  >
    <span v-if="loading" class="button-loading">
      <svg class="animate-spin h-4 w-4" fill="none" viewBox="0 0 24 24">
        <circle class="opacity-25" cx="12" cy="12" r="10" stroke="currentColor" stroke-width="4"></circle>
        <path class="opacity-75" fill="currentColor" d="M4 12a8 8 0 018-8V0C5.373 0 0 5.373 0 12h4zm2 5.291A7.962 7.962 0 014 12H0c0 3.042 1.135 5.824 3 7.938l3-2.647z"></path>
      </svg>
    </span>
    <span v-else-if="icon" class="button-icon">{{ icon }}</span>
    <span class="button-text">
      <slot>{{ text }}</slot>
    </span>
  </button>
</template>

<script setup>
import { computed } from 'vue'

// Props
const props = defineProps({
  variant: {
    type: String,
    default: 'primary',
    validator: (value) => ['primary', 'secondary', 'success', 'warning', 'error', 'outline', 'ghost'].includes(value)
  },
  size: {
    type: String,
    default: 'medium',
    validator: (value) => ['small', 'medium', 'large'].includes(value)
  },
  disabled: {
    type: Boolean,
    default: false
  },
  loading: {
    type: Boolean,
    default: false
  },
  icon: {
    type: String,
    default: ''
  },
  text: {
    type: String,
    default: ''
  },
  type: {
    type: String,
    default: 'button'
  },
  fullWidth: {
    type: Boolean,
    default: false
  }
})

// Emits
const emit = defineEmits(['click'])

// Computed
const buttonClasses = computed(() => {
  const baseClasses = [
    'button',
    `button--${props.variant}`,
    `button--${props.size}`,
    {
      'button--disabled': props.disabled,
      'button--loading': props.loading,
      'button--full-width': props.fullWidth
    }
  ]

  return baseClasses
})

// Methods
const handleClick = (event) => {
  if (!props.disabled && !props.loading) {
    emit('click', event)
  }
}
</script>

<style scoped>
.button {
  display: inline-flex;
  align-items: center;
  justify-content: center;
  gap: 0.5rem;
  border: none;
  border-radius: 6px;
  font-weight: 500;
  text-decoration: none;
  cursor: pointer;
  transition: all 0.2s ease-in-out;
  position: relative;
  outline: none;
}

.button:focus {
  box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.3);
}

.button--small {
  padding: 0.5rem 1rem;
  font-size: 0.875rem;
  min-height: 2rem;
}

.button--medium {
  padding: 0.75rem 1.5rem;
  font-size: 1rem;
  min-height: 2.5rem;
}

.button--large {
  padding: 1rem 2rem;
  font-size: 1.125rem;
  min-height: 3rem;
}

.button--primary {
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  color: white;
  text-shadow: 0 1px 3px rgba(0,0,0,0.3);
  box-shadow: 0 2px 8px rgba(102, 126, 234, 0.3);
}

.button--primary:hover:not(.button--disabled) {
  transform: translateY(-1px);
  box-shadow: 0 4px 12px rgba(102, 126, 234, 0.4);
}

.button--secondary {
  background: rgba(255, 255, 255, 0.15);
  color: white;
  border: 1px solid rgba(255, 255, 255, 0.3);
  backdrop-filter: blur(20px);
  text-shadow: 0 1px 3px rgba(0,0,0,0.3);
  box-shadow: 0 4px 15px rgba(0,0,0,0.1);
}

.button--secondary:hover:not(.button--disabled) {
  background: rgba(255, 255, 255, 0.25);
  border-color: rgba(255, 255, 255, 0.5);
  transform: translateY(-1px);
  box-shadow: 0 6px 20px rgba(0,0,0,0.15);
}

.button--success {
  background: #28a745;
  color: white;
}

.button--success:hover:not(.button--disabled) {
  background: #218838;
  transform: translateY(-1px);
}

.button--warning {
  background: #ffc107;
  color: #212529;
}

.button--warning:hover:not(.button--disabled) {
  background: #e0a800;
  transform: translateY(-1px);
}

.button--error {
  background: #dc3545;
  color: white;
}

.button--error:hover:not(.button--disabled) {
  background: #c82333;
  transform: translateY(-1px);
}

.button--outline {
  background: rgba(255, 255, 255, 0.15);
  border: 1px solid rgba(255, 255, 255, 0.3);
  color: white;
  backdrop-filter: blur(20px);
  text-shadow: 0 1px 3px rgba(0,0,0,0.3);
  box-shadow: 0 4px 15px rgba(0,0,0,0.1);
}

.button--outline:hover:not(.button--disabled) {
  background: rgba(255, 255, 255, 0.25);
  border-color: rgba(255, 255, 255, 0.5);
  color: white;
  transform: translateY(-1px);
  box-shadow: 0 6px 20px rgba(0,0,0,0.15);
}

.button--ghost {
  background: transparent;
  color: #667eea;
}

.button--ghost:hover:not(.button--disabled) {
  background: rgba(102, 126, 234, 0.1);
}

.button--disabled {
  opacity: 0.6;
  cursor: not-allowed;
  transform: none !important;
  box-shadow: none !important;
}

.button--loading {
  cursor: not-allowed;
}

.button--full-width {
  width: 100%;
}

.button-loading {
  display: flex;
  align-items: center;
}

.button-icon {
  font-size: 1.2em;
}

.button-text {
  flex: 1;
}

.animate-spin {
  animation: spin 1s linear infinite;
}

@keyframes spin {
  from {
    transform: rotate(0deg);
  }
  to {
    transform: rotate(360deg);
  }
}
</style>

