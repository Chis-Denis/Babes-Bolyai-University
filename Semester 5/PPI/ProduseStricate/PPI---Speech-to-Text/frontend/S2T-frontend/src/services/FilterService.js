/**
 * Filter Service
 * Handles data filtering and search functionality
 */

class FilterService {
  constructor() {
    this.filters = {
      patients: {
        name: '',
        ageRange: { min: null, max: null },
        gender: '',
        dateRange: { start: null, end: null },
        hasRecordings: null
      },
      recordings: {
        patientId: null,
        dateRange: { start: null, end: null },
        duration: { min: null, max: null },
        hasTranscription: null,
        confidence: { min: null, max: null }
      }
    }
  }

  /**
   * Filter patients based on criteria
   */
  filterPatients(patients, criteria = {}) {
    if (!Array.isArray(patients)) return []

    return patients.filter(patient => {
      // Name filter
      if (criteria.name && !patient.name.toLowerCase().includes(criteria.name.toLowerCase())) {
        return false
      }

      // Age range filter
      if (criteria.ageRange) {
        if (criteria.ageRange.min !== null && patient.age < criteria.ageRange.min) {
          return false
        }
        if (criteria.ageRange.max !== null && patient.age > criteria.ageRange.max) {
          return false
        }
      }

      // Gender filter - only apply if gender is specified (non-empty string)
      if (criteria.gender && criteria.gender.trim() !== '' && patient.gender !== criteria.gender) {
        return false
      }

      // Date range filter
      if (criteria.dateRange) {
        const patientDate = new Date(patient.updatedAt || patient.createdAt)
        if (criteria.dateRange.start && patientDate < new Date(criteria.dateRange.start)) {
          return false
        }
        if (criteria.dateRange.end && patientDate > new Date(criteria.dateRange.end)) {
          return false
        }
      }

      // Has recordings filter
      if (criteria.hasRecordings !== null) {
        const hasRecordings = patient.observations && patient.observations.length > 0
        if (criteria.hasRecordings && !hasRecordings) {
          return false
        }
        if (!criteria.hasRecordings && hasRecordings) {
          return false
        }
      }

      return true
    })
  }

  /**
   * Filter recordings based on criteria
   */
  filterRecordings(recordings, criteria = {}) {
    if (!Array.isArray(recordings)) return []

    return recordings.filter(recording => {
      // Patient ID filter
      if (criteria.patientId && recording.patientId !== criteria.patientId) {
        return false
      }

      // Date range filter
      if (criteria.dateRange) {
        const recordingDate = new Date(recording.timestamp)
        if (criteria.dateRange.start && recordingDate < new Date(criteria.dateRange.start)) {
          return false
        }
        if (criteria.dateRange.end && recordingDate > new Date(criteria.dateRange.end)) {
          return false
        }
      }

      // Duration filter
      if (criteria.duration) {
        if (criteria.duration.min !== null && recording.duration < criteria.duration.min) {
          return false
        }
        if (criteria.duration.max !== null && recording.duration > criteria.duration.max) {
          return false
        }
      }

      // Has transcription filter
      if (criteria.hasTranscription !== null) {
        const hasTranscription = recording.transcription && recording.transcription.text
        if (criteria.hasTranscription && !hasTranscription) {
          return false
        }
        if (!criteria.hasTranscription && hasTranscription) {
          return false
        }
      }

      // Confidence filter
      if (criteria.confidence) {
        if (criteria.confidence.min !== null && recording.transcription?.confidence < criteria.confidence.min) {
          return false
        }
        if (criteria.confidence.max !== null && recording.transcription?.confidence > criteria.confidence.max) {
          return false
        }
      }

      return true
    })
  }

  /**
   * Search patients by text query
   */
  searchPatients(patients, query) {
    if (!query || !query.trim()) return patients

    const searchTerm = query.toLowerCase()
    
    return patients.filter(patient => {
      return (
        patient.name.toLowerCase().includes(searchTerm) ||
        patient.phone?.toLowerCase().includes(searchTerm) ||
        patient.email?.toLowerCase().includes(searchTerm) ||
        patient.medicalHistory?.toLowerCase().includes(searchTerm) ||
        patient.allergies?.toLowerCase().includes(searchTerm) ||
        patient.currentMedications?.toLowerCase().includes(searchTerm)
      )
    })
  }

  /**
   * Search recordings by text query
   */
  searchRecordings(recordings, query) {
    if (!query || !query.trim()) return recordings

    const searchTerm = query.toLowerCase()
    
    return recordings.filter(recording => {
      return (
        recording.transcription?.text.toLowerCase().includes(searchTerm) ||
        recording.patientName?.toLowerCase().includes(searchTerm)
      )
    })
  }

  /**
   * Sort patients by criteria
   */
  sortPatients(patients, sortBy = 'name', sortOrder = 'asc') {
    return [...patients].sort((a, b) => {
      let aValue = a[sortBy]
      let bValue = b[sortBy]

      // Handle date sorting
      if (sortBy === 'createdAt' || sortBy === 'updatedAt') {
        aValue = new Date(aValue)
        bValue = new Date(bValue)
      }

      // Handle string sorting
      if (typeof aValue === 'string') {
        aValue = aValue.toLowerCase()
        bValue = bValue.toLowerCase()
      }

      if (sortOrder === 'desc') {
        return bValue > aValue ? 1 : -1
      } else {
        return aValue > bValue ? 1 : -1
      }
    })
  }

  /**
   * Sort recordings by criteria
   */
  sortRecordings(recordings, sortBy = 'timestamp', sortOrder = 'desc') {
    return [...recordings].sort((a, b) => {
      let aValue = a[sortBy]
      let bValue = b[sortBy]

      // Handle date sorting
      if (sortBy === 'timestamp' || sortBy === 'startTime') {
        aValue = new Date(aValue)
        bValue = new Date(bValue)
      }

      // Handle nested properties
      if (sortBy === 'confidence') {
        aValue = a.transcription?.confidence || 0
        bValue = b.transcription?.confidence || 0
      }

      if (sortOrder === 'desc') {
        return bValue > aValue ? 1 : -1
      } else {
        return aValue > bValue ? 1 : -1
      }
    })
  }

  /**
   * Get filter options for patients
   */
  getPatientFilterOptions(patients) {
    const genders = [...new Set(patients.map(p => p.gender).filter(Boolean))]
    const ageGroups = this.getAgeGroups(patients)
    const hasRecordingsOptions = [
      { value: true, label: 'With recordings' },
      { value: false, label: 'Without recordings' }
    ]

    return {
      genders: genders.map(gender => ({ value: gender, label: gender })),
      ageGroups,
      hasRecordingsOptions
    }
  }

  /**
   * Get age groups from patients
   */
  getAgeGroups(patients) {
    const ages = patients.map(p => p.age).filter(age => age > 0)
    const minAge = Math.min(...ages)
    const maxAge = Math.max(...ages)

    const groups = []
    for (let i = Math.floor(minAge / 10) * 10; i <= Math.ceil(maxAge / 10) * 10; i += 10) {
      groups.push({
        value: { min: i, max: i + 9 },
        label: `${i}-${i + 9} ani`
      })
    }

    return groups
  }

  /**
   * Clear all filters
   */
  clearFilters() {
    this.filters = {
      patients: {
        name: '',
        ageRange: { min: null, max: null },
        gender: '',
        dateRange: { start: null, end: null },
        hasRecordings: null
      },
      recordings: {
        patientId: null,
        dateRange: { start: null, end: null },
        duration: { min: null, max: null },
        hasTranscription: null,
        confidence: { min: null, max: null }
      }
    }
  }

  /**
   * Get current filters
   */
  getFilters() {
    return this.filters
  }

  /**
   * Set filter value
   */
  setFilter(category, key, value) {
    if (this.filters[category]) {
      this.filters[category][key] = value
    }
  }
}

// Create and export singleton instance
export const filterService = new FilterService()
export default filterService

