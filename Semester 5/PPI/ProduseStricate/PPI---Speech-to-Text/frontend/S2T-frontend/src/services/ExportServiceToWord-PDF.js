/**
 * Export Service
 * Handles export functionality for Word and PDF documents
 */

import { PatientForm } from '@/models/PatientForm.js'

class ExportService {
  constructor() {
    this.supportedFormats = ['docx', 'pdf']
    this.defaultFormat = 'docx'
  }

  /**
   * Export patient data to Word document
   */
  async exportPatientToWord(patient, recordings = []) {
    try {
      // Generate proper HTML content for Word
      const htmlContent = this.generatePatientDocumentHTML(patient, recordings)
      
      // Create proper Word-compatible HTML with correct MIME type
      const wordHtml = this.wrapForWordDocument(htmlContent)
      
      // Use text/html MIME type for HTML documents that Word can open
      const blob = new Blob([wordHtml], { type: 'text/html;charset=utf-8' })
      const url = URL.createObjectURL(blob)
      
      const link = document.createElement('a')
      link.href = url
      link.download = `Pacient_${patient.name.replace(/\s+/g, '_')}_${this.formatDate(new Date())}.html`
      document.body.appendChild(link)
      link.click()
      document.body.removeChild(link)
      
      URL.revokeObjectURL(url)
      
      return { success: true, filename: link.download }
    } catch (error) {
      console.error('Failed to export patient to Word:', error)
      return { success: false, error: error.message }
    }
  }

  /**
   * Export patient data to PDF
   */
  async exportPatientToPDF(patient, recordings = []) {
    try {
      // Generate HTML content optimized for PDF printing
      const htmlContent = this.generatePatientDocumentHTML(patient, recordings)
      
      // Create a new window for PDF generation
      const printWindow = window.open('', '_blank', 'width=800,height=600')
      
      if (!printWindow) {
        throw new Error('Popup blocked. Please allow popups for this site.')
      }
      
      // Write HTML content to the new window
      printWindow.document.write(htmlContent)
      printWindow.document.close()
      
      // Add PDF-optimized styles
      const style = printWindow.document.createElement('style')
      style.textContent = `
        @media print {
          body { 
            font-family: Arial, sans-serif; 
            margin: 0; 
            padding: 20px; 
            font-size: 12px;
            line-height: 1.4;
          }
          .header { 
            border-bottom: 2px solid #333; 
            padding-bottom: 10px; 
            margin-bottom: 20px; 
          }
          .section { 
            margin-bottom: 15px; 
            page-break-inside: avoid;
          }
          .section h3 { 
            color: #333; 
            border-bottom: 1px solid #ccc; 
            font-size: 14px;
            margin-bottom: 10px;
          }
          .observation { 
            background: #f5f5f5; 
            padding: 8px; 
            margin: 8px 0; 
            border-left: 3px solid #007bff; 
            font-size: 11px;
          }
          .info-grid {
            display: table;
            width: 100%;
          }
          .info-item {
            display: table-row;
          }
          .info-label, .info-value {
            display: table-cell;
            padding: 2px 5px;
            border-bottom: 1px solid #eee;
          }
          .info-label {
            font-weight: bold;
            width: 30%;
          }
        }
        @page {
          margin: 1cm;
          size: A4;
        }
      `
      printWindow.document.head.appendChild(style)
      
      // Wait for content to load, then trigger print
      setTimeout(() => {
        printWindow.focus()
        printWindow.print()
        
        // Close the window after printing
        setTimeout(() => {
          printWindow.close()
        }, 1000)
      }, 500)
      
      return { success: true }
    } catch (error) {
      console.error('Failed to export patient to PDF:', error)
      return { success: false, error: error.message }
    }
  }

  /**
   * Wrap HTML content for Word document compatibility
   */
  wrapForWordDocument(htmlContent) {
    return `
      <html xmlns:o="urn:schemas-microsoft-com:office:office"
            xmlns:w="urn:schemas-microsoft-com:office:word"
            xmlns="http://www.w3.org/TR/REC-html40">
      <head>
        <meta charset="utf-8">
        <meta name="ProgId" content="Word.Document">
        <meta name="Generator" content="Microsoft Word 15">
        <meta name="Originator" content="Microsoft Word 15">
        <style>
          @page {
            size: 8.5in 11in;
            margin: 1in;
          }
          body {
            font-family: 'Times New Roman', serif;
            font-size: 12pt;
            line-height: 1.5;
            margin: 0;
            padding: 0;
          }
          h1 {
            font-size: 16pt;
            font-weight: bold;
            text-align: center;
            margin-bottom: 20pt;
          }
          h3 {
            font-size: 14pt;
            font-weight: bold;
            margin-top: 20pt;
            margin-bottom: 10pt;
            border-bottom: 1pt solid #000;
            padding-bottom: 5pt;
          }
          .info-grid {
            display: table;
            width: 100%;
            margin-bottom: 10pt;
          }
          .info-item {
            display: table-row;
          }
          .info-label {
            display: table-cell;
            font-weight: bold;
            width: 30%;
            padding: 2pt 5pt;
            vertical-align: top;
          }
          .info-value {
            display: table-cell;
            padding: 2pt 5pt;
            vertical-align: top;
          }
          .observation {
            background-color: #f8f8f8;
            border-left: 3pt solid #007bff;
            padding: 8pt;
            margin: 8pt 0;
            font-size: 11pt;
          }
          .footer {
            margin-top: 30pt;
            padding-top: 10pt;
            border-top: 1pt solid #ccc;
            font-size: 10pt;
            color: #666;
            text-align: center;
          }
        </style>
      </head>
      <body>
        ${htmlContent}
      </body>
      </html>
    `
  }

  /**
   * Generate HTML content for patient document
   */
  generatePatientDocumentHTML(patient, recordings = []) {
    const currentDate = this.formatDate(new Date())
    
    return `
        <div class="header">
          <h1>FISA PACIENTULUI</h1>
          <div class="date">Generat la: ${currentDate}</div>
        </div>

        <div class="section">
          <h3>INFORMATII PERSONALE</h3>
          <div class="info-grid">
            <div class="info-item">
              <span class="info-label">Nume:</span>
              <span class="info-value">${patient.name}</span>
            </div>
            <div class="info-item">
              <span class="info-label">Varsta:</span>
              <span class="info-value">${patient.age} ani</span>
            </div>
            <div class="info-item">
              <span class="info-label">Sex:</span>
              <span class="info-value">${patient.gender}</span>
            </div>
            <div class="info-item">
              <span class="info-label">Data nasterii:</span>
              <span class="info-value">${patient.dateOfBirth || 'N/A'}</span>
            </div>
            <div class="info-item">
              <span class="info-label">Telefon:</span>
              <span class="info-value">${patient.phone || 'N/A'}</span>
            </div>
            <div class="info-item">
              <span class="info-label">Email:</span>
              <span class="info-value">${patient.email || 'N/A'}</span>
            </div>
            <div class="info-item">
              <span class="info-label">Adresa:</span>
              <span class="info-value">${patient.address || 'N/A'}</span>
            </div>
          </div>
        </div>

        <div class="section">
          <h3>INFORMATII MEDICALE</h3>
          <div class="info-grid">
            <div class="info-item">
              <span class="info-label">Grup sanguin:</span>
              <span class="info-value">${patient.bloodType || 'N/A'}</span>
            </div>
            <div class="info-item">
              <span class="info-label">Numar asigurare:</span>
              <span class="info-value">${patient.insuranceNumber || 'N/A'}</span>
            </div>
          </div>
          <div class="info-item">
            <span class="info-label">Istoric medical:</span>
            <span class="info-value">${patient.medicalHistory || 'Nu exista informatii'}</span>
          </div>
          <div class="info-item">
            <span class="info-label">Alergii:</span>
            <span class="info-value">${patient.allergies || 'Nu sunt cunoscute alergii'}</span>
          </div>
          <div class="info-item">
            <span class="info-label">Medicamente curente:</span>
            <span class="info-value">${patient.currentMedications || 'Nu exista medicamente'}</span>
          </div>
          <div class="info-item">
            <span class="info-label">Contact urgenta:</span>
            <span class="info-value">${patient.emergencyContact || 'Nu exista informatii'}</span>
          </div>
        </div>

        ${recordings.length > 0 ? `
        <div class="section">
          <h3>OBSERVATII SI TRANSCRIPTII</h3>
          ${recordings.map(recording => `
            <div class="observation ${recording.transcription ? 'transcription' : ''}">
              <div class="timestamp">
                ${this.formatDate(recording.timestamp)} - 
                ${recording.transcription ? 'Transcriptie audio' : 'Observatie manuala'}
              </div>
              <div class="content">
                ${recording.transcription || recording.text}
              </div>
            </div>
          `).join('')}
        </div>
        ` : ''}

        <div class="footer">
          <p>Document generat automat de sistemul Speech-to-Text Medical</p>
          <p>Data generarii: ${currentDate}</p>
        </div>
    `
  }

  /**
   * Export multiple patients to batch document
   */
  async exportBatchPatients(patients, format = 'docx') {
    try {
      const htmlContent = this.generateBatchDocumentHTML(patients)
      
      const blob = new Blob([htmlContent], { 
        type: format === 'pdf' ? 'text/html' : 'application/msword' 
      })
      const url = URL.createObjectURL(blob)
      
      const link = document.createElement('a')
      link.href = url
      link.download = `Pacienti_${this.formatDate(new Date())}.${format === 'pdf' ? 'html' : 'doc'}`
      document.body.appendChild(link)
      link.click()
      document.body.removeChild(link)
      
      URL.revokeObjectURL(url)
      
      return { success: true, filename: link.download }
    } catch (error) {
      console.error('Failed to export batch patients:', error)
      return { success: false, error: error.message }
    }
  }

  /**
   * Generate batch document HTML
   */
  generateBatchDocumentHTML(patients) {
    const currentDate = this.formatDate(new Date())
    
    return `
      <!DOCTYPE html>
      <html>
      <head>
        <meta charset="UTF-8">
        <title>Patient List - ${currentDate}</title>
        <style>
          body { font-family: Arial, sans-serif; margin: 20px; }
          .header { text-align: center; margin-bottom: 30px; border-bottom: 2px solid #333; padding-bottom: 15px; }
          .patient-summary { border: 1px solid #ccc; margin-bottom: 20px; padding: 15px; border-radius: 5px; }
          .patient-summary h3 { margin-top: 0; color: #333; }
          .info-row { margin-bottom: 5px; }
          .info-label { font-weight: bold; display: inline-block; width: 150px; }
        </style>
      </head>
      <body>
        <div class="header">
          <h1>LISTA PACIENTI</h1>
          <p>Generat la: ${currentDate}</p>
        </div>
        
        ${patients.map(patient => `
          <div class="patient-summary">
            <h3>${patient.name}</h3>
            <div class="info-row">
              <span class="info-label">Age:</span> ${patient.age} years old
            </div>
            <div class="info-row">
              <span class="info-label">Sex:</span> ${patient.gender}
            </div>
            <div class="info-row">
              <span class="info-label">Telefon:</span> ${patient.phone || 'N/A'}
            </div>
            <div class="info-row">
              <span class="info-label">Last visit:</span> ${this.formatDate(patient.updatedAt)}
            </div>
          </div>
        `).join('')}
      </body>
      </html>
    `
  }

  /**
   * Format date for display
   */
  formatDate(date) {
    const d = new Date(date)
    return d.toLocaleDateString('ro-RO', {
      year: 'numeric',
      month: '2-digit',
      day: '2-digit',
      hour: '2-digit',
      minute: '2-digit'
    })
  }

  /**
   * Get supported export formats
   */
  getSupportedFormats() {
    return this.supportedFormats
  }

  /**
   * Validate export data
   */
  validateExportData(patient, recordings = []) {
    const errors = []
    
    if (!patient || !patient.name) {
      errors.push('Patient data is required')
    }
    
    if (!patient.age || patient.age < 0) {
      errors.push('Valid patient age is required')
    }
    
    return {
      isValid: errors.length === 0,
      errors
    }
  }
}

// Create and export singleton instance
export const exportService = new ExportService()
export default exportService

