/**
 * API Client Service
 * Handles all HTTP communication with the backend
 */

import axios from 'axios'
import { appConfig } from '@/config/app.config.js'

class ApiClient {
  constructor() {
    this.baseURL = appConfig.api.baseUrl
    this.timeout = appConfig.api.timeout
    
    // Create axios instance
    this.client = axios.create({
      baseURL: this.baseURL,
      timeout: this.timeout,
      headers: {
        'Content-Type': 'application/json',
        'Accept': 'application/json'
      }
    })

    // Add request interceptor for general headers
    this.client.interceptors.request.use(
      (config) => {
        // No authentication headers needed
        return config
      },
      (error) => {
        return Promise.reject(error)
      }
    )

    // Add response interceptor for error handling
    this.client.interceptors.response.use(
      (response) => response,
      (error) => {
        // Handle general errors without authentication logic
        return Promise.reject(error)
      }
    )
  }

  /**
   * Generic GET request
   */
  async get(endpoint, params = {}) {
    try {
      const response = await this.client.get(endpoint, { params })
      return response.data
    } catch (error) {
      console.error('GET request failed:', error)
      throw this.handleError(error)
    }
  }

  /**
   * Generic POST request
   */
  async post(endpoint, data = {}) {
    try {
      const response = await this.client.post(endpoint, data)
      return response.data
    } catch (error) {
      console.error('POST request failed:', error)
      throw this.handleError(error)
    }
  }

  /**
   * Generic PUT request
   */
  async put(endpoint, data = {}) {
    try {
      const response = await this.client.put(endpoint, data)
      return response.data
    } catch (error) {
      console.error('PUT request failed:', error)
      throw this.handleError(error)
    }
  }

  /**
   * Generic DELETE request
   */
  async delete(endpoint) {
    try {
      const response = await this.client.delete(endpoint)
      return response.data
    } catch (error) {
      console.error('DELETE request failed:', error)
      throw this.handleError(error)
    }
  }

  /**
   * Upload file
   */
  async uploadFile(endpoint, file, onProgress = null) {
    try {
      const formData = new FormData()
      formData.append('file', file)

      const config = {
        headers: {
          'Content-Type': 'multipart/form-data'
        }
      }

      if (onProgress) {
        config.onUploadProgress = (progressEvent) => {
          const percentCompleted = Math.round((progressEvent.loaded * 100) / progressEvent.total)
          onProgress(percentCompleted)
        }
      }

      const response = await this.client.post(endpoint, formData, config)
      return response.data
    } catch (error) {
      console.error('File upload failed:', error)
      throw this.handleError(error)
    }
  }

  /**
   * Handle API errors
   */
  handleError(error) {
    if (error.response) {
      // Server responded with error status
      return {
        status: error.response.status,
        message: error.response.data?.message || 'Server error occurred',
        data: error.response.data
      }
    } else if (error.request) {
      // Request was made but no response received
      return {
        status: 0,
        message: 'Network error - please check your connection',
        data: null
      }
    } else {
      // Something else happened
      return {
        status: -1,
        message: error.message || 'An unexpected error occurred',
        data: null
      }
    }
  }

  /**
   * Test API connection
   */
  async testConnection() {
    try {
      const response = await this.get('/')
      return { success: true, data: response }
    } catch (error) {
      return { success: false, error: error.message }
    }
  }
}

// Create and export singleton instance
export const apiClient = new ApiClient()
export default apiClient

