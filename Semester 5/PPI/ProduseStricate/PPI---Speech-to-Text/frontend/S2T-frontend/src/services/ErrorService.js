/**
 * Error Service
 * Comprehensive error handling and management system
 */

import { reactive, ref } from 'vue'
import { toastService } from './ToastService.js'

class ErrorService {
  constructor() {
    this.errors = reactive([])
    this.globalError = ref(null)
    this.maxErrors = 50
    this.errorCategories = {
      validation: 'Validation Error',
      network: 'Network Error',
      permission: 'Permission Error',
      server: 'Server Error',
      client: 'Client Error',
      unknown: 'Unknown Error'
    }
  }

  /**
   * Handle and categorize an error
   */
  handleError(error, context = null, options = {}) {
    const errorInfo = this.categorizeError(error, context, options)
    
    // Add to errors list
    this.addError(errorInfo)
    
    // Show toast notification if enabled
    if (options.showToast !== false) {
      this.showErrorToast(errorInfo)
    }
    
    // Log error to console in development
    if (process.env.NODE_ENV === 'development') {
      console.error('Error handled:', errorInfo)
    }
    
    return errorInfo
  }

  /**
   * Categorize error and extract information
   */
  categorizeError(error, context, options) {
    const errorInfo = {
      id: Date.now() + Math.random(),
      message: this.extractErrorMessage(error),
      type: this.determineErrorType(error),
      category: this.categorizeErrorType(error),
      context: context,
      timestamp: new Date().toISOString(),
      stack: error.stack || null,
      code: error.code || null,
      status: error.status || error.statusCode || null,
      details: error.details || null,
      userMessage: options.userMessage || this.getUserFriendlyMessage(error),
      severity: this.determineSeverity(error),
      recoverable: this.isRecoverable(error),
      retryable: this.isRetryable(error),
      originalError: error
    }

    return errorInfo
  }

  /**
   * Extract error message from various error formats
   */
  extractErrorMessage(error) {
    if (typeof error === 'string') {
      return error
    }
    
    if (error.message) {
      return error.message
    }
    
    if (error.error && error.error.message) {
      return error.error.message
    }
    
    if (error.data && error.data.message) {
      return error.data.message
    }
    
    return 'An unknown error occurred'
  }

  /**
   * Determine error type
   */
  determineErrorType(error) {
    if (error.status === 403 || error.statusCode === 403) {
      return 'permission'
    }
    
    if (error.status >= 500 || error.statusCode >= 500) {
      return 'server'
    }
    
    if (error.status >= 400 || error.statusCode >= 400) {
      return 'client'
    }
    
    if (error.name === 'ValidationError' || error.type === 'validation') {
      return 'validation'
    }
    
    if (error.name === 'NetworkError' || error.message?.includes('network')) {
      return 'network'
    }
    
    return 'unknown'
  }

  /**
   * Categorize error type for display
   */
  categorizeErrorType(error) {
    const type = this.determineErrorType(error)
    return this.errorCategories[type] || this.errorCategories.unknown
  }

  /**
   * Get user-friendly error message
   */
  getUserFriendlyMessage(error) {
    const type = this.determineErrorType(error)
    
    const messages = {
      validation: 'Please check the entered data and try again.',
      network: 'Please check your internet connection and try again.',
      permission: 'You do not have permission to perform this action.',
      server: 'A server problem occurred. Please try again later.',
      client: 'A problem occurred with the request. Please try again.',
      unknown: 'An unexpected problem occurred. Please contact support.'
    }
    
    return messages[type] || messages.unknown
  }

  /**
   * Determine error severity
   */
  determineSeverity(error) {
    const type = this.determineErrorType(error)
    
    const severityMap = {
      validation: 'warning',
      network: 'error',
      permission: 'warning',
      server: 'error',
      client: 'warning',
      unknown: 'error'
    }
    
    return severityMap[type] || 'error'
  }

  /**
   * Check if error is recoverable
   */
  isRecoverable(error) {
    const type = this.determineErrorType(error)
    return ['validation', 'network', 'client'].includes(type)
  }

  /**
   * Check if error is retryable
   */
  isRetryable(error) {
    const type = this.determineErrorType(error)
    return ['network', 'server'].includes(type)
  }

  /**
   * Add error to errors list
   */
  addError(errorInfo) {
    this.errors.push(errorInfo)
    
    // Remove oldest errors if we exceed max count
    if (this.errors.length > this.maxErrors) {
      this.errors.shift()
    }
  }

  /**
   * Set global error
   */
  setGlobalError(error, context = null) {
    this.globalError.value = this.categorizeError(error, context)
    this.handleError(error, context)
  }

  /**
   * Clear global error
   */
  clearGlobalError() {
    this.globalError.value = null
  }

  /**
   * Show error toast notification
   */
  showErrorToast(errorInfo) {
    const toastOptions = {
      duration: errorInfo.severity === 'error' ? 7000 : 5000,
      persistent: !errorInfo.recoverable
    }
    
    if (errorInfo.severity === 'error') {
      toastService.error(errorInfo.userMessage, toastOptions)
    } else {
      toastService.warning(errorInfo.userMessage, toastOptions)
    }
  }

  /**
   * Remove error by ID
   */
  removeError(errorId) {
    const index = this.errors.findIndex(error => error.id === errorId)
    if (index > -1) {
      this.errors.splice(index, 1)
    }
  }

  /**
   * Clear all errors
   */
  clearErrors() {
    this.errors.splice(0)
    this.clearGlobalError()
  }

  /**
   * Clear errors by type
   */
  clearErrorsByType(type) {
    this.errors.splice(0, this.errors.length, ...this.errors.filter(error => error.type !== type))
  }

  /**
   * Get errors by type
   */
  getErrorsByType(type) {
    return this.errors.filter(error => error.type === type)
  }

  /**
   * Get errors by severity
   */
  getErrorsBySeverity(severity) {
    return this.errors.filter(error => error.severity === severity)
  }

  /**
   * Get recent errors
   */
  getRecentErrors(count = 10) {
    return this.errors.slice(-count).reverse()
  }

  /**
   * Get error count
   */
  getErrorCount() {
    return this.errors.length
  }

  /**
   * Get error count by type
   */
  getErrorCountByType(type) {
    return this.errors.filter(error => error.type === type).length
  }

  /**
   * Check if there are any errors
   */
  hasErrors() {
    return this.errors.length > 0 || this.globalError.value !== null
  }

  /**
   * Create validation error
   */
  createValidationError(message, field = null) {
    const error = new Error(message)
    error.type = 'validation'
    error.field = field
    return this.handleError(error, 'validation')
  }

  /**
   * Create network error
   */
  createNetworkError(message = 'Network connection failed') {
    const error = new Error(message)
    error.type = 'network'
    return this.handleError(error, 'network')
  }

  /**
   * Retry failed operation
   */
  async retry(operation, maxRetries = 3, delay = 1000) {
    let lastError = null
    
    for (let attempt = 1; attempt <= maxRetries; attempt++) {
      try {
        return await operation()
      } catch (error) {
        lastError = error
        
        if (attempt === maxRetries) {
          this.handleError(error, 'retry', { 
            userMessage: `Operation failed after ${maxRetries} attempts.`
          })
          throw error
        }
        
        // Wait before retry
        await new Promise(resolve => setTimeout(resolve, delay * attempt))
      }
    }
  }
}

// Create and export singleton instance
export const errorService = new ErrorService()
export default errorService

