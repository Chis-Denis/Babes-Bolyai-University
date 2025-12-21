<template>
  <div :class="spinnerClasses" :style="spinnerStyles">
    <div class="spinner-inner">
      <div class="spinner-circle"></div>
      <div class="spinner-circle"></div>
      <div class="spinner-circle"></div>
    </div>
    <span v-if="text" class="spinner-text">{{ text }}</span>
  </div>
</template>

<script setup>
import { computed } from 'vue'

// Props
const props = defineProps({
  size: {
    type: String,
    default: 'medium',
    validator: (value) => ['small', 'medium', 'large'].includes(value)
  },
  color: {
    type: String,
    default: '#667eea'
  },
  text: {
    type: String,
    default: ''
  },
  centered: {
    type: Boolean,
    default: false
  },
  overlay: {
    type: Boolean,
    default: false
  }
})

// Computed
const spinnerClasses = computed(() => {
  return [
    'spinner',
    `spinner--${props.size}`,
    {
      'spinner--centered': props.centered,
      'spinner--overlay': props.overlay
    }
  ]
})

const spinnerStyles = computed(() => {
  return {
    '--spinner-color': props.color
  }
})
</script>

<style scoped>
.spinner {
  display: inline-flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 0.5rem;
}

.spinner--centered {
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
}

.spinner--overlay {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: rgba(255, 255, 255, 0.8);
  z-index: 9999;
}

.spinner-inner {
  display: flex;
  gap: 0.25rem;
}

.spinner-circle {
  width: 0.5rem;
  height: 0.5rem;
  border-radius: 50%;
  background: var(--spinner-color);
  animation: spinner-bounce 1.4s ease-in-out infinite both;
}

.spinner-circle:nth-child(1) {
  animation-delay: -0.32s;
}

.spinner-circle:nth-child(2) {
  animation-delay: -0.16s;
}

.spinner-circle:nth-child(3) {
  animation-delay: 0s;
}

.spinner--small .spinner-circle {
  width: 0.25rem;
  height: 0.25rem;
}

.spinner--medium .spinner-circle {
  width: 0.5rem;
  height: 0.5rem;
}

.spinner--large .spinner-circle {
  width: 0.75rem;
  height: 0.75rem;
}

.spinner-text {
  font-size: 0.875rem;
  color: #6b7280;
  font-weight: 500;
}

.spinner--small .spinner-text {
  font-size: 0.75rem;
}

.spinner--large .spinner-text {
  font-size: 1rem;
}

@keyframes spinner-bounce {
  0%, 80%, 100% {
    transform: scale(0);
  }
  40% {
    transform: scale(1);
  }
}

/* Dark mode support */
@media (prefers-color-scheme: dark) {
  .spinner--overlay {
    background: rgba(0, 0, 0, 0.8);
  }
  
  .spinner-text {
    color: #9ca3af;
  }
}
</style>
