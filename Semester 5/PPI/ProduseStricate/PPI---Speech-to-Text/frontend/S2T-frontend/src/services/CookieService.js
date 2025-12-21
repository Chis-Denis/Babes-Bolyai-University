/**
 * Cookie Service
 * Comprehensive cookie management system
 */

import Cookies from 'js-cookie'

class CookieService {
  constructor() {
    this.defaultOptions = {
      expires: 365, // 1 year
      secure: process.env.NODE_ENV === 'production',
      sameSite: 'lax'
    }
    
    this.cookieKeys = {
      // User preferences
      theme: 's2t_theme',
      language: 's2t_language',
      sidebarCollapsed: 's2t_sidebar_collapsed',
      compactMode: 's2t_compact_mode',
      
      // Application state
      lastRoute: 's2t_last_route',
      searchFilters: 's2t_search_filters',
      userPreferences: 's2t_user_preferences',
      
      // Session data
      sessionId: 's2t_session_id',
      lastActivity: 's2t_last_activity',
      
      // Feature flags
      featureFlags: 's2t_feature_flags'
    }
  }

  /**
   * Set a cookie with default options
   */
  set(name, value, options = {}) {
    const cookieOptions = { ...this.defaultOptions, ...options }
    
    try {
      Cookies.set(name, value, cookieOptions)
      return true
    } catch (error) {
      console.error('Failed to set cookie:', error)
      return false
    }
  }

  /**
   * Get a cookie value
   */
  get(name, defaultValue = null) {
    try {
      const value = Cookies.get(name)
      return value !== undefined ? value : defaultValue
    } catch (error) {
      console.error('Failed to get cookie:', error)
      return defaultValue
    }
  }

  /**
   * Remove a cookie
   */
  remove(name) {
    try {
      Cookies.remove(name)
      return true
    } catch (error) {
      console.error('Failed to remove cookie:', error)
      return false
    }
  }

  /**
   * Check if a cookie exists
   */
  exists(name) {
    return Cookies.get(name) !== undefined
  }

  /**
   * Get all cookies
   */
  getAll() {
    try {
      return Cookies.get()
    } catch (error) {
      console.error('Failed to get all cookies:', error)
      return {}
    }
  }

  /**
   * Clear all application cookies
   */
  clearAll() {
    try {
      Object.values(this.cookieKeys).forEach(key => {
        Cookies.remove(key)
      })
      return true
    } catch (error) {
      console.error('Failed to clear all cookies:', error)
      return false
    }
  }

  // Theme management
  setTheme(theme) {
    return this.set(this.cookieKeys.theme, theme)
  }

  getTheme() {
    return this.get(this.cookieKeys.theme, 'light')
  }

  // Language management
  setLanguage(language) {
    return this.set(this.cookieKeys.language, language)
  }

  getLanguage() {
    return this.get(this.cookieKeys.language, 'ro')
  }

  // Sidebar state
  setSidebarCollapsed(collapsed) {
    return this.set(this.cookieKeys.sidebarCollapsed, collapsed.toString())
  }

  getSidebarCollapsed() {
    return this.get(this.cookieKeys.sidebarCollapsed, 'false') === 'true'
  }

  // Compact mode
  setCompactMode(enabled) {
    return this.set(this.cookieKeys.compactMode, enabled.toString())
  }

  getCompactMode() {
    return this.get(this.cookieKeys.compactMode, 'false') === 'true'
  }

  // Last route
  setLastRoute(route) {
    return this.set(this.cookieKeys.lastRoute, route)
  }

  getLastRoute() {
    return this.get(this.cookieKeys.lastRoute, '/')
  }

  // Search filters
  setSearchFilters(filters) {
    try {
      const filtersJson = JSON.stringify(filters)
      return this.set(this.cookieKeys.searchFilters, filtersJson)
    } catch (error) {
      console.error('Failed to save search filters:', error)
      return false
    }
  }

  getSearchFilters() {
    try {
      const filtersJson = this.get(this.cookieKeys.searchFilters)
      return filtersJson ? JSON.parse(filtersJson) : {}
    } catch (error) {
      console.error('Failed to load search filters:', error)
      return {}
    }
  }

  // User preferences
  setUserPreferences(preferences) {
    try {
      const preferencesJson = JSON.stringify(preferences)
      return this.set(this.cookieKeys.userPreferences, preferencesJson)
    } catch (error) {
      console.error('Failed to save user preferences:', error)
      return false
    }
  }

  getUserPreferences() {
    try {
      const preferencesJson = this.get(this.cookieKeys.userPreferences)
      return preferencesJson ? JSON.parse(preferencesJson) : {}
    } catch (error) {
      console.error('Failed to load user preferences:', error)
      return {}
    }
  }

  // Session management
  setSessionId(sessionId) {
    return this.set(this.cookieKeys.sessionId, sessionId, {
      expires: 1 // 1 day for session
    })
  }

  getSessionId() {
    return this.get(this.cookieKeys.sessionId)
  }

  setLastActivity() {
    return this.set(this.cookieKeys.lastActivity, Date.now().toString(), {
      expires: 1 // 1 day
    })
  }

  getLastActivity() {
    const lastActivity = this.get(this.cookieKeys.lastActivity)
    return lastActivity ? parseInt(lastActivity) : null
  }

  // Feature flags
  setFeatureFlags(flags) {
    try {
      const flagsJson = JSON.stringify(flags)
      return this.set(this.cookieKeys.featureFlags, flagsJson)
    } catch (error) {
      console.error('Failed to save feature flags:', error)
      return false
    }
  }

  getFeatureFlags() {
    try {
      const flagsJson = this.get(this.cookieKeys.featureFlags)
      return flagsJson ? JSON.parse(flagsJson) : {}
    } catch (error) {
      console.error('Failed to load feature flags:', error)
      return {}
    }
  }

  // Utility methods
  setJSON(name, value, options = {}) {
    try {
      const jsonValue = JSON.stringify(value)
      return this.set(name, jsonValue, options)
    } catch (error) {
      console.error('Failed to set JSON cookie:', error)
      return false
    }
  }

  getJSON(name, defaultValue = null) {
    try {
      const jsonValue = this.get(name)
      return jsonValue ? JSON.parse(jsonValue) : defaultValue
    } catch (error) {
      console.error('Failed to get JSON cookie:', error)
      return defaultValue
    }
  }

  // Cookie consent management
  setCookieConsent(given) {
    return this.set('cookie_consent', given.toString(), {
      expires: 365 * 10 // 10 years
    })
  }

  getCookieConsent() {
    return this.get('cookie_consent') === 'true'
  }

  hasCookieConsent() {
    return this.exists('cookie_consent')
  }

  // Data export/import
  exportCookies() {
    const cookies = this.getAll()
    return {
      timestamp: new Date().toISOString(),
      cookies,
      version: '1.0'
    }
  }

  importCookies(data) {
    try {
      if (data.cookies) {
        Object.entries(data.cookies).forEach(([name, value]) => {
          this.set(name, value)
        })
        return true
      }
      return false
    } catch (error) {
      console.error('Failed to import cookies:', error)
      return false
    }
  }

  // Cookie analytics
  getCookieStats() {
    const allCookies = this.getAll()
    const appCookies = Object.values(this.cookieKeys).reduce((acc, key) => {
      if (this.exists(key)) {
        acc[key] = this.get(key)
      }
      return acc
    }, {})

    return {
      total: Object.keys(allCookies).length,
      application: Object.keys(appCookies).length,
      external: Object.keys(allCookies).length - Object.keys(appCookies).length,
      appCookies
    }
  }

  // Cleanup expired cookies
  cleanup() {
    // This is handled automatically by the browser
    // But we can remove any cookies we know are expired
    const lastActivity = this.getLastActivity()
    if (lastActivity && Date.now() - lastActivity > 24 * 60 * 60 * 1000) { // 24 hours
      this.remove(this.cookieKeys.sessionId)
      this.remove(this.cookieKeys.lastActivity)
    }
  }
}

// Create and export singleton instance
export const cookieService = new CookieService()
export default cookieService

