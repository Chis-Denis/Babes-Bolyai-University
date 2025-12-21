/**
 * Patient Form Model
 * Data structure for patient information
 */

export class PatientForm {
  constructor(data = {}) {
    this.id = data.id || null
    this.name = data.name || ''
    this.age = data.age || null
    this.gender = data.gender || ''
    this.dateOfBirth = data.dateOfBirth || ''
    this.phone = data.phone || ''
    this.email = data.email || ''
    this.address = data.address || ''
    this.medicalHistory = data.medicalHistory || ''
    this.allergies = data.allergies || ''
    this.currentMedications = data.currentMedications || ''
    this.bloodType = data.bloodType || ''
    this.emergencyContact = data.emergencyContact || ''
    this.insuranceNumber = data.insuranceNumber || ''
    this.createdAt = data.createdAt || new Date().toISOString()
    this.updatedAt = data.updatedAt || new Date().toISOString()
    this.observations = data.observations || []
  }

  /**
   * Validate patient form data
   */
  validate() {
    const errors = []
    
    if (!this.name.trim()) {
      errors.push('Name is required')
    }
    
    if (!this.age || this.age < 0 || this.age > 150) {
      errors.push('Valid age is required')
    }
    
    if (!this.gender) {
      errors.push('Gender is required')
    }
    
    return {
      isValid: errors.length === 0,
      errors
    }
  }

  /**
   * Convert to JSON for API calls
   */
  toJSON() {
    return {
      id: this.id,
      name: this.name,
      age: this.age,
      gender: this.gender,
      dateOfBirth: this.dateOfBirth,
      phone: this.phone,
      email: this.email,
      address: this.address,
      medicalHistory: this.medicalHistory,
      allergies: this.allergies,
      currentMedications: this.currentMedications,
      bloodType: this.bloodType,
      emergencyContact: this.emergencyContact,
      insuranceNumber: this.insuranceNumber,
      createdAt: this.createdAt,
      updatedAt: this.updatedAt,
      observations: this.observations
    }
  }

  /**
   * Create from JSON data
   */
  static fromJSON(data) {
    return new PatientForm(data)
  }

  /**
   * Get display name for the patient
   */
  getDisplayName() {
    return this.name || 'Unknown Patient'
  }

  /**
   * Get age category
   */
  getAgeCategory() {
    if (this.age < 18) return 'Minor'
    if (this.age < 65) return 'Adult'
    return 'Senior'
  }

  /**
   * Add observation to patient
   */
  addObservation(observation) {
    this.observations.push({
      id: Date.now(),
      text: observation,
      timestamp: new Date().toISOString(),
      type: 'manual'
    })
    this.updatedAt = new Date().toISOString()
  }

  /**
   * Add transcription observation
   */
  addTranscriptionObservation(transcription) {
    this.observations.push({
      id: Date.now(),
      text: transcription,
      timestamp: new Date().toISOString(),
      type: 'transcription'
    })
    this.updatedAt = new Date().toISOString()
  }
}


