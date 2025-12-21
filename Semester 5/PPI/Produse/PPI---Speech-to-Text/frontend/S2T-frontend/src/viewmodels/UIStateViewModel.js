/**
 * UI State ViewModel
 * Manages application-wide UI state and user interface logic
 */

import { reactive, ref, computed } from 'vue'

export function useUIStateViewModel() {
  // Reactive state
  const state = reactive({
    // Navigation
    currentRoute: '',
    sidebarCollapsed: false,
    breadcrumbs: [],

    // Modals and dialogs
    modals: {
      patientForm: false,
      recordingModal: false,
      exportModal: false,
      settingsModal: false,
      confirmationDialog: false
    },

    // Notifications
    notifications: [],
    toastMessages: [],

    // Loading states
    loading: {
      global: false,
      patients: false,
      recordings: false,
      export: false
    },

    // Theme and appearance
    theme: {
      mode: 'light', // light, dark
      primaryColor: '#667eea',
      accentColor: '#764ba2'
    },

    // User preferences
    preferences: {
      language: 'ro',
      autoSave: true,
      notifications: true,
      compactMode: false
    },

    // Form states
    forms: {
      patientForm: {
        isDirty: false,
        isValid: false,
        errors: []
      },
      recordingForm: {
        isDirty: false,
        isValid: false,
        errors: []
      }
    },

    // Error handling
    errors: {
      global: null,
      forms: {},
      api: {}
    }
  })

  // Computed properties
  const hasNotifications = computed(() => state.notifications.length > 0)

  const hasUnreadNotifications = computed(() => 
    state.notifications.some(n => !n.read)
  )

  const isAnyLoading = computed(() => 
    Object.values(state.loading).some(loading => loading)
  )

  const isDarkMode = computed(() => state.theme.mode === 'dark')

  const activeModals = computed(() => 
    Object.entries(state.modals)
      .filter(([key, value]) => value)
      .map(([key]) => key)
  )

  // Methods
  const setCurrentRoute = (route) => {
    state.currentRoute = route
    updateBreadcrumbs(route)
  }

  const updateBreadcrumbs = (route) => {
    const breadcrumbMap = {
      '/': [{ label: 'Acasă', path: '/' }],
      '/patients': [
        { label: 'Acasă', path: '/' },
        { label: 'Pacienți', path: '/patients' }
      ],
      '/recording': [
        { label: 'Acasă', path: '/' },
        { label: 'Înregistrare', path: '/recording' }
      ],
      '/settings': [
        { label: 'Acasă', path: '/' },
        { label: 'Setări', path: '/settings' }
      ]
    }

    state.breadcrumbs = breadcrumbMap[route] || [{ label: 'Acasă', path: '/' }]
  }

  const toggleSidebar = () => {
    state.sidebarCollapsed = !state.sidebarCollapsed
  }

  const setSidebarCollapsed = (collapsed) => {
    state.sidebarCollapsed = collapsed
  }

  // Modal management
  const openModal = (modalName) => {
    if (state.modals[modalName] !== undefined) {
      state.modals[modalName] = true
    }
  }

  const closeModal = (modalName) => {
    if (state.modals[modalName] !== undefined) {
      state.modals[modalName] = false
    }
  }

  const closeAllModals = () => {
    Object.keys(state.modals).forEach(key => {
      state.modals[key] = false
    })
  }

  // Notification management
  const addNotification = (notification) => {
    const id = Date.now()
    const newNotification = {
      id,
      type: notification.type || 'info',
      title: notification.title || 'Notificare',
      message: notification.message || '',
      timestamp: new Date().toISOString(),
      read: false,
      ...notification
    }

    state.notifications.unshift(newNotification)

    // Keep only last 50 notifications
    if (state.notifications.length > 50) {
      state.notifications = state.notifications.slice(0, 50)
    }

    return id
  }

  const markNotificationAsRead = (notificationId) => {
    const notification = state.notifications.find(n => n.id === notificationId)
    if (notification) {
      notification.read = true
    }
  }

  const markAllNotificationsAsRead = () => {
    state.notifications.forEach(notification => {
      notification.read = true
    })
  }

  const removeNotification = (notificationId) => {
    state.notifications = state.notifications.filter(n => n.id !== notificationId)
  }

  const clearAllNotifications = () => {
    state.notifications = []
  }

  // Toast messages
  const showToast = (message, type = 'info', duration = 5000) => {
    const id = Date.now()
    const toast = {
      id,
      message,
      type,
      duration,
      timestamp: new Date().toISOString()
    }

    state.toastMessages.push(toast)

    // Auto remove after duration
    setTimeout(() => {
      removeToast(id)
    }, duration)

    return id
  }

  const removeToast = (toastId) => {
    state.toastMessages = state.toastMessages.filter(t => t.id !== toastId)
  }

  const clearAllToasts = () => {
    state.toastMessages = []
  }

  // Loading states
  const setLoading = (key, loading) => {
    if (state.loading[key] !== undefined) {
      state.loading[key] = loading
    }
  }

  const setGlobalLoading = (loading) => {
    state.loading.global = loading
  }

  // Theme management
  const setTheme = (theme) => {
    state.theme = { ...state.theme, ...theme }
    applyTheme(state.theme)
  }

  const toggleTheme = () => {
    state.theme.mode = state.theme.mode === 'light' ? 'dark' : 'light'
    applyTheme(state.theme)
  }

  const applyTheme = (theme) => {
    // Apply theme to document root
    document.documentElement.setAttribute('data-theme', theme.mode)
    document.documentElement.style.setProperty('--primary-color', theme.primaryColor)
    document.documentElement.style.setProperty('--accent-color', theme.accentColor)
  }

  // Preferences management
  const updatePreferences = (preferences) => {
    state.preferences = { ...state.preferences, ...preferences }
    savePreferencesToStorage()
  }

  const loadPreferencesFromStorage = () => {
    try {
      const saved = localStorage.getItem('ui-preferences')
      if (saved) {
        const preferences = JSON.parse(saved)
        state.preferences = { ...state.preferences, ...preferences }
      }
    } catch (error) {
      console.error('Failed to load preferences:', error)
    }
  }

  const savePreferencesToStorage = () => {
    try {
      localStorage.setItem('ui-preferences', JSON.stringify(state.preferences))
    } catch (error) {
      console.error('Failed to save preferences:', error)
    }
  }

  // Form state management
  const setFormState = (formName, state) => {
    if (state.forms[formName]) {
      state.forms[formName] = { ...state.forms[formName], ...state }
    }
  }

  const clearFormState = (formName) => {
    if (state.forms[formName]) {
      state.forms[formName] = {
        isDirty: false,
        isValid: false,
        errors: []
      }
    }
  }

  // Error management
  const setError = (category, key, error) => {
    if (!state.errors[category]) {
      state.errors[category] = {}
    }
    state.errors[category][key] = error
  }

  const clearError = (category, key) => {
    if (state.errors[category] && state.errors[category][key]) {
      delete state.errors[category][key]
    }
  }

  const clearAllErrors = () => {
    state.errors = {
      global: null,
      forms: {},
      api: {}
    }
  }

  // Utility methods
  const showSuccess = (message) => {
    showToast(message, 'success')
    addNotification({
      type: 'success',
      title: 'Succes',
      message
    })
  }

  const showError = (message) => {
    showToast(message, 'error')
    addNotification({
      type: 'error',
      title: 'Eroare',
      message
    })
  }

  const showWarning = (message) => {
    showToast(message, 'warning')
    addNotification({
      type: 'warning',
      title: 'Avertisment',
      message
    })
  }

  const showInfo = (message) => {
    showToast(message, 'info')
    addNotification({
      type: 'info',
      title: 'Informație',
      message
    })
  }

  // Initialize
  const initialize = () => {
    loadPreferencesFromStorage()
    applyTheme(state.theme)
  }

  return {
    // State
    currentRoute: computed(() => state.currentRoute),
    sidebarCollapsed: computed(() => state.sidebarCollapsed),
    breadcrumbs: computed(() => state.breadcrumbs),
    modals: computed(() => state.modals),
    notifications: computed(() => state.notifications),
    toastMessages: computed(() => state.toastMessages),
    loading: computed(() => state.loading),
    theme: computed(() => state.theme),
    preferences: computed(() => state.preferences),
    forms: computed(() => state.forms),
    errors: computed(() => state.errors),

    // Computed
    hasNotifications,
    hasUnreadNotifications,
    isAnyLoading,
    isDarkMode,
    activeModals,

    // Methods
    setCurrentRoute,
    toggleSidebar,
    setSidebarCollapsed,
    openModal,
    closeModal,
    closeAllModals,
    addNotification,
    markNotificationAsRead,
    markAllNotificationsAsRead,
    removeNotification,
    clearAllNotifications,
    showToast,
    removeToast,
    clearAllToasts,
    setLoading,
    setGlobalLoading,
    setTheme,
    toggleTheme,
    updatePreferences,
    setFormState,
    clearFormState,
    setError,
    clearError,
    clearAllErrors,
    showSuccess,
    showError,
    showWarning,
    showInfo,
    initialize
  }
}

