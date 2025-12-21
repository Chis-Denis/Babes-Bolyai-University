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
      console.log('🔵 Loading patients from backend API...')
      // Load patients from backend API
      const patientsData = await apiClient.getPatients()
      console.log('✅ Backend API response:', patientsData)
      
      state.patients = patientsData.map(patientData => new PatientForm({
        id: patientData.id,
        name: patientData.name,
        age: patientData.age,
        gender: patientData.gender,
        dateOfBirth: patientData.date_of_birth,
        phone: patientData.phone,
        email: patientData.email,
        address: patientData.address,
        medicalHistory: patientData.medical_history,
        allergies: patientData.allergies,
        currentMedications: patientData.current_medications,
        bloodType: patientData.blood_type,
        insuranceNumber: patientData.insurance_number,
        emergencyContact: patientData.emergency_contact,
        createdAt: patientData.created_at,
        updatedAt: patientData.updated_at
      }))
      
      console.log(`✅ Loaded ${state.patients.length} patients from database`)
      toastService.info(`Loaded ${state.patients.length} patients`)
    } catch (error) {
      console.error('❌ Error loading patients:', error)
      state.error = error.message
      toastService.error('Failed to load patients', error.message)
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
      // Prepare data for backend API (convert camelCase to snake_case)
      const apiData = {
        name: patientData.name,
        age: patientData.age,
        gender: patientData.gender,
        date_of_birth: patientData.dateOfBirth,
        phone: patientData.phone,
        email: patientData.email,
        address: patientData.address,
        medical_history: patientData.medicalHistory,
        allergies: patientData.allergies,
        current_medications: patientData.currentMedications,
        blood_type: patientData.bloodType,
        insurance_number: patientData.insuranceNumber,
        emergency_contact: patientData.emergencyContact
      }

      // Create patient via API
      const createdPatient = await apiClient.createPatient(apiData)
      
      // Convert snake_case response to camelCase for frontend
      const newPatient = new PatientForm({
        id: createdPatient.id,
        name: createdPatient.name,
        age: createdPatient.age,
        gender: createdPatient.gender,
        dateOfBirth: createdPatient.date_of_birth,
        phone: createdPatient.phone,
        email: createdPatient.email,
        address: createdPatient.address,
        medicalHistory: createdPatient.medical_history,
        allergies: createdPatient.allergies,
        currentMedications: createdPatient.current_medications,
        bloodType: createdPatient.blood_type,
        insuranceNumber: createdPatient.insurance_number,
        emergencyContact: createdPatient.emergency_contact,
        createdAt: createdPatient.created_at,
        updatedAt: createdPatient.updated_at
      })

      state.patients.push(newPatient)
      
      // Show success toast
      toastService.success(`Pacientul "${newPatient.name}" a fost creat cu succes!`)
      
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
      // Prepare data for backend API (convert camelCase to snake_case)
      const apiData = {
        name: patientData.name,
        age: patientData.age,
        gender: patientData.gender,
        date_of_birth: patientData.dateOfBirth,
        phone: patientData.phone,
        email: patientData.email,
        address: patientData.address,
        medical_history: patientData.medicalHistory,
        allergies: patientData.allergies,
        current_medications: patientData.currentMedications,
        blood_type: patientData.bloodType,
        insurance_number: patientData.insuranceNumber,
        emergency_contact: patientData.emergencyContact
      }

      // Update patient via API
      const updatedPatientData = await apiClient.updatePatient(patientId, apiData)
      
      // Convert snake_case response to camelCase for frontend
      const updatedPatient = new PatientForm({
        id: updatedPatientData.id,
        name: updatedPatientData.name,
        age: updatedPatientData.age,
        gender: updatedPatientData.gender,
        dateOfBirth: updatedPatientData.date_of_birth,
        phone: updatedPatientData.phone,
        email: updatedPatientData.email,
        address: updatedPatientData.address,
        medicalHistory: updatedPatientData.medical_history,
        allergies: updatedPatientData.allergies,
        currentMedications: updatedPatientData.current_medications,
        bloodType: updatedPatientData.blood_type,
        insuranceNumber: updatedPatientData.insurance_number,
        emergencyContact: updatedPatientData.emergency_contact,
        createdAt: updatedPatientData.created_at,
        updatedAt: updatedPatientData.updated_at
      })

      // Update local state
      const patientIndex = state.patients.findIndex(p => p.id === patientId)
      if (patientIndex !== -1) {
        state.patients[patientIndex] = updatedPatient
      }
      
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
      // Delete patient via API
      await apiClient.deletePatient(patientId)

      // Remove from local state
      const patientIndex = state.patients.findIndex(p => p.id === patientId)
      if (patientIndex !== -1) {
        state.patients.splice(patientIndex, 1)
      }
      
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
