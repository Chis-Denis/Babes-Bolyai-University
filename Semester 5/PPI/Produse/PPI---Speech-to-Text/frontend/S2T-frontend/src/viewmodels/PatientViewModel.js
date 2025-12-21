/**
 * Patient ViewModel
 * Business logic for patient management
 */

import { reactive, ref, computed } from 'vue'
import { PatientForm } from '@/models/PatientForm.js'
import { apiClient } from '@/services/ApiClient.js'
import { exportService } from '@/services/ExportServiceToWord-PDF.js'
import { filterService } from '@/services/FilterService.js'
import { toastService } from '@/services/ToastService.js'
import { errorService } from '@/services/ErrorService.js'
import { cookieService } from '@/services/CookieService.js'

export function usePatientViewModel() {
  // Reactive state
  const state = reactive({
    patients: [],
    currentPatient: null,
    isLoading: false,
    error: null,
    searchQuery: '',
    filters: {
      name: '',
      ageRange: { min: null, max: null },
      gender: '',
      hasRecordings: null
    },
    sortBy: 'name',
    sortOrder: 'asc'
  })

  // Computed properties
  const filteredPatients = computed(() => {
    let result = [...state.patients]

    // Apply search
    if (state.searchQuery) {
      result = filterService.searchPatients(result, state.searchQuery)
    }

    // Apply filters
    result = filterService.filterPatients(result, state.filters)

    // Apply sorting
    result = filterService.sortPatients(result, state.sortBy, state.sortOrder)

    return result
  })

  const patientsCount = computed(() => state.patients.length)
  const filteredPatientsCount = computed(() => filteredPatients.value.length)

  // Methods
  const loadPatients = async () => {
    state.isLoading = true
    state.error = null

    try {
      // Try to load from localStorage first (more reliable than cookies)
      let savedPatients = null
      
      try {
        const localStorageData = localStorage.getItem('s2t-patients')
        if (localStorageData) {
          savedPatients = JSON.parse(localStorageData)
        }
      } catch (error) {
        console.warn('Failed to load from localStorage:', error)
      }
      
      // Fallback to cookies if localStorage is empty
      if (!savedPatients || savedPatients.length === 0) {
        savedPatients = cookieService.getJSON('patients', [])
      }
      
      // If no saved patients, use mock data
      if (savedPatients.length === 0) {
        const mockPatients = [
          new PatientForm({
            id: 1,
            name: 'Ion Popescu',
            age: 45,
            gender: 'Male',
            phone: '0712345678',
            email: 'ion.popescu@email.com',
            medicalHistory: 'Arterial hypertension, type 2 diabetes',
            allergies: 'Penicillin',
            currentMedications: 'Metformin 500mg, Lisinopril 10mg',
            bloodType: 'A+',
            observations: [
              {
                id: 1,
                text: 'Patient presents with chest pain and breathing difficulties.',
                timestamp: new Date().toISOString(),
                type: 'transcription'
              }
            ]
          }),
          new PatientForm({
            id: 2,
            name: 'Maria Ionescu',
            age: 32,
            gender: 'Female',
            phone: '0798765432',
            email: 'maria.ionescu@email.com',
            medicalHistory: 'Iron deficiency anemia',
            allergies: 'No known allergies',
            currentMedications: 'Suplimente de fier',
            bloodType: 'O+'
          }),
          new PatientForm({
            id: 3,
            name: 'Gheorghe Dumitrescu',
            age: 67,
            gender: 'Male',
            phone: '0711111111',
            medicalHistory: 'Ischemic heart disease, arthritis',
            allergies: 'Aspirin',
            currentMedications: 'Aspirin 100mg, Atorvastatin 20mg',
            bloodType: 'B+'
          })
        ]
        
        state.patients = mockPatients
        // Save mock data to both localStorage and cookies
        savePatientsToStorage(mockPatients)
      } else {
        // Load saved patients from storage
        state.patients = savedPatients.map(patientData => new PatientForm(patientData))
      }
      
      toastService.info(`Loaded ${state.patients.length} patients`)
    } catch (error) {
      state.error = error.message
      errorService.handleError(error, 'loadPatients', {
        userMessage: 'Error loading patients'
      })
    } finally {
      state.isLoading = false
    }
  }

  // Helper function to save patients to both localStorage and cookies
  const savePatientsToStorage = (patients) => {
    try {
      // Save to localStorage (primary storage)
      localStorage.setItem('s2t-patients', JSON.stringify(patients))
      
      // Save to cookies as backup
      cookieService.setJSON('patients', patients)
      
      console.log(`Saved ${patients.length} patients to storage`)
    } catch (error) {
      console.error('Failed to save patients to storage:', error)
      // Try cookies as fallback
      try {
        cookieService.setJSON('patients', patients)
      } catch (cookieError) {
        console.error('Failed to save patients to cookies:', cookieError)
      }
    }
  }

  const createPatient = async (patientData) => {
    state.isLoading = true
    state.error = null

    try {
      const newPatient = new PatientForm({
        ...patientData,
        id: Date.now(), // Simple ID generation for MVP
        createdAt: new Date().toISOString(),
        updatedAt: new Date().toISOString()
      })

      // Validate patient data
      const validation = newPatient.validate()
      if (!validation.isValid) {
        const error = new Error(validation.errors.join(', '))
        error.type = 'validation'
        throw error
      }

      state.patients.push(newPatient)
      
      // Show success toast
      toastService.success(`Pacientul "${newPatient.name}" a fost creat cu succes!`)
      
      // Save to storage for persistence
      savePatientsToStorage(state.patients)
      
      return { success: true, patient: newPatient }
    } catch (error) {
      state.error = error.message
      errorService.handleError(error, 'createPatient', {
        userMessage: 'Eroare la crearea pacientului'
      })
      return { success: false, error: error.message }
    } finally {
      state.isLoading = false
    }
  }

  const updatePatient = async (patientId, patientData) => {
    state.isLoading = true
    state.error = null

    try {
      const patientIndex = state.patients.findIndex(p => p.id === patientId)
      if (patientIndex === -1) {
        throw new Error('Patient not found')
      }

      const updatedPatient = new PatientForm({
        ...state.patients[patientIndex],
        ...patientData,
        id: patientId,
        updatedAt: new Date().toISOString()
      })

      // Validate patient data
      const validation = updatedPatient.validate()
      if (!validation.isValid) {
        throw new Error(validation.errors.join(', '))
      }

      state.patients[patientIndex] = updatedPatient
      
      // Save to storage for persistence
      savePatientsToStorage(state.patients)
      
      return { success: true, patient: updatedPatient }
    } catch (error) {
      state.error = error.message
      return { success: false, error: error.message }
    } finally {
      state.isLoading = false
    }
  }

  const deletePatient = async (patientId) => {
    state.isLoading = true
    state.error = null

    try {
      const patientIndex = state.patients.findIndex(p => p.id === patientId)
      if (patientIndex === -1) {
        throw new Error('Patient not found')
      }

      state.patients.splice(patientIndex, 1)
      
      // Save to storage for persistence
      savePatientsToStorage(state.patients)
      
      return { success: true }
    } catch (error) {
      state.error = error.message
      return { success: false, error: error.message }
    } finally {
      state.isLoading = false
    }
  }

  const getPatientById = (patientId) => {
    return state.patients.find(p => p.id === patientId)
  }

  const setCurrentPatient = (patient) => {
    console.log('setCurrentPatient called with:', patient)
    console.log('Patient type:', typeof patient)
    console.log('Patient value:', patient?.value || patient)
    state.currentPatient = patient
    console.log('state.currentPatient set to:', state.currentPatient)
  }

  const addPatientObservation = (patientId, observation) => {
    const patient = getPatientById(patientId)
    if (patient) {
      patient.addObservation(observation)
    }
  }

  const addTranscriptionObservation = (patientId, transcription) => {
    const patient = getPatientById(patientId)
    if (patient) {
      patient.addTranscriptionObservation(transcription)
    }
  }

  const exportPatient = async (patientId, format = 'docx') => {
    try {
      const patient = getPatientById(patientId)
      if (!patient) {
        throw new Error('Patient not found')
      }

      const recordings = patient.observations || []

      if (format === 'docx') {
        return await exportService.exportPatientToWord(patient, recordings)
      } else if (format === 'pdf') {
        return await exportService.exportPatientToPDF(patient, recordings)
      } else {
        throw new Error('Unsupported export format')
      }
    } catch (error) {
      state.error = error.message
      return { success: false, error: error.message }
    }
  }

  const exportAllPatients = async (format = 'docx') => {
    try {
      return await exportService.exportBatchPatients(state.patients, format)
    } catch (error) {
      state.error = error.message
      return { success: false, error: error.message }
    }
  }

  const setSearchQuery = (query) => {
    state.searchQuery = query
  }

  const setFilters = (filters) => {
    state.filters = { ...state.filters, ...filters }
  }

  const setSorting = (sortBy, sortOrder) => {
    state.sortBy = sortBy
    state.sortOrder = sortOrder
  }

  const clearFilters = () => {
    state.searchQuery = ''
    state.filters = {
      name: '',
      ageRange: { min: null, max: null },
      gender: '',
      hasRecordings: null
    }
  }

  return {
    // State
    patients: filteredPatients,
    currentPatient: computed(() => {
      console.log('currentPatient computed called, state.currentPatient:', state.currentPatient)
      return state.currentPatient
    }),
    isLoading: computed(() => state.isLoading),
    error: computed(() => state.error),
    searchQuery: computed(() => state.searchQuery),
    filters: computed(() => state.filters),
    sortBy: computed(() => state.sortBy),
    sortOrder: computed(() => state.sortOrder),

    // Computed
    patientsCount,
    filteredPatientsCount,

    // Methods
    loadPatients,
    createPatient,
    updatePatient,
    deletePatient,
    getPatientById,
    setCurrentPatient,
    addPatientObservation,
    addTranscriptionObservation,
    exportPatient,
    exportAllPatients,
    setSearchQuery,
    setFilters,
    setSorting,
    clearFilters
  }
}
