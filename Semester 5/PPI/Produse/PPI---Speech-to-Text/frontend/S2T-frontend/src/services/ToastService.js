class ToastService {
  constructor() {
    this.toasts = []
    this.nextId = 1
  }

  // Add a new toast
  add(toast) {
    const id = this.nextId++
    const newToast = {
      id,
      type: 'info',
      position: 'top-right',
      duration: 4000,
      closable: true,
      persistent: false,
      ...toast
    }

    this.toasts.push(newToast)

    // Auto-remove if not persistent
    if (!newToast.persistent && newToast.duration > 0) {
      setTimeout(() => {
        this.remove(id)
      }, newToast.duration)
    }

    return id
  }

  // Remove a toast by ID
  remove(toastId) {
    this.toasts = this.toasts.filter(toast => toast.id !== toastId)
  }

  // Get toasts by position
  getToastsByPosition(position) {
    return this.toasts.filter(toast => toast.position === position)
  }

  // Clear all toasts
  clear() {
    this.toasts = []
  }

  // Convenience methods
  success(title, message = '', options = {}) {
    return this.add({
      type: 'success',
      title,
      message,
      icon: '✅',
      ...options
    })
  }

  error(title, message = '', options = {}) {
    return this.add({
      type: 'error',
      title,
      message,
      icon: '❌',
      ...options
    })
  }

  warning(title, message = '', options = {}) {
    return this.add({
      type: 'warning',
      title,
      message,
      icon: '⚠️',
      ...options
    })
  }

  info(title, message = '', options = {}) {
    return this.add({
      type: 'info',
      title,
      message,
      icon: 'ℹ️',
      ...options
    })
  }

  loading(title, message = '', options = {}) {
    return this.add({
      type: 'loading',
      title,
      message,
      icon: '⏳',
      persistent: true,
      closable: false,
      ...options
    })
  }
}

// Create singleton instance
const toastService = new ToastService()

export { toastService }