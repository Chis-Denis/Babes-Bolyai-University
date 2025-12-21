"""
Patient management endpoints
"""
from fastapi import APIRouter, HTTPException
from datetime import datetime
from typing import List, Optional
import sqlite3
from app.models import PatientCreate, PatientUpdate, PatientResponse
from app.database import get_db_connection

router = APIRouter()


def row_to_dict(cursor, row):
    """Convert database row to dictionary"""
    return dict(zip([col[0] for col in cursor.description], row))


@router.get("/", response_model=List[PatientResponse])
async def get_patients():
    """Get all patients"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    cursor.execute('''
        SELECT * FROM patients ORDER BY created_at DESC
    ''')
    
    rows = cursor.fetchall()
    patients = []
    
    for row in rows:
        patients.append({
            'id': row['id'],
            'name': row['name'],
            'age': row['age'],
            'gender': row['gender'],
            'date_of_birth': row['date_of_birth'],
            'phone': row['phone'],
            'email': row['email'],
            'address': row['address'],
            'medical_history': row['medical_history'],
            'allergies': row['allergies'],
            'current_medications': row['current_medications'],
            'blood_type': row['blood_type'],
            'insurance_number': row['insurance_number'],
            'emergency_contact': row['emergency_contact'],
            'created_at': row['created_at'],
            'updated_at': row['updated_at']
        })
    
    conn.close()
    return patients


@router.get("/{patient_id}", response_model=PatientResponse)
async def get_patient(patient_id: int):
    """Get a single patient by ID"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    cursor.execute('SELECT * FROM patients WHERE id = ?', (patient_id,))
    row = cursor.fetchone()
    conn.close()
    
    if not row:
        raise HTTPException(status_code=404, detail="Patient not found")
    
    return {
        'id': row['id'],
        'name': row['name'],
        'age': row['age'],
        'gender': row['gender'],
        'date_of_birth': row['date_of_birth'],
        'phone': row['phone'],
        'email': row['email'],
        'address': row['address'],
        'medical_history': row['medical_history'],
        'allergies': row['allergies'],
        'current_medications': row['current_medications'],
        'blood_type': row['blood_type'],
        'insurance_number': row['insurance_number'],
        'emergency_contact': row['emergency_contact'],
        'created_at': row['created_at'],
        'updated_at': row['updated_at']
    }


@router.post("/", response_model=PatientResponse)
async def create_patient(patient: PatientCreate):
    """Create a new patient"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    current_time = datetime.now().isoformat()
    
    cursor.execute('''
        INSERT INTO patients (
            name, age, gender, date_of_birth, phone, email, address,
            medical_history, allergies, current_medications, blood_type,
            insurance_number, emergency_contact, created_at, updated_at
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    ''', (
        patient.name, patient.age, patient.gender, patient.date_of_birth,
        patient.phone, patient.email, patient.address,
        patient.medical_history, patient.allergies, patient.current_medications,
        patient.blood_type, patient.insurance_number, patient.emergency_contact,
        current_time, current_time
    ))
    
    conn.commit()
    patient_id = cursor.lastrowid
    conn.close()
    
    return {
        **patient.model_dump(),
        'id': patient_id,
        'created_at': current_time,
        'updated_at': current_time
    }


@router.put("/{patient_id}", response_model=PatientResponse)
async def update_patient(patient_id: int, patient: PatientUpdate):
    """Update an existing patient"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    # Check if patient exists
    cursor.execute('SELECT * FROM patients WHERE id = ?', (patient_id,))
    if not cursor.fetchone():
        conn.close()
        raise HTTPException(status_code=404, detail="Patient not found")
    
    current_time = datetime.now().isoformat()
    
    cursor.execute('''
        UPDATE patients SET
            name = ?, age = ?, gender = ?, date_of_birth = ?,
            phone = ?, email = ?, address = ?,
            medical_history = ?, allergies = ?, current_medications = ?,
            blood_type = ?, insurance_number = ?, emergency_contact = ?,
            updated_at = ?
        WHERE id = ?
    ''', (
        patient.name, patient.age, patient.gender, patient.date_of_birth,
        patient.phone, patient.email, patient.address,
        patient.medical_history, patient.allergies, patient.current_medications,
        patient.blood_type, patient.insurance_number, patient.emergency_contact,
        current_time, patient_id
    ))
    
    conn.commit()
    conn.close()
    
    return {
        **patient.model_dump(),
        'id': patient_id,
        'created_at': '',
        'updated_at': current_time
    }


@router.delete("/{patient_id}")
async def delete_patient(patient_id: int):
    """Delete a patient and all associated documents"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    # Check if patient exists
    cursor.execute('SELECT * FROM patients WHERE id = ?', (patient_id,))
    patient = cursor.fetchone()
    if not patient:
        conn.close()
        raise HTTPException(status_code=404, detail="Patient not found")
    
    print(f"Deleting patient with ID: {patient_id}")
    
    # Delete all documents associated with this patient
    # Delete in order to respect foreign key constraints if they exist
    cursor.execute('DELETE FROM prescription_forms WHERE patient_id = ?', (patient_id,))
    deleted_prescriptions = cursor.rowcount
    print(f"Deleted {deleted_prescriptions} prescription forms")
    
    cursor.execute('DELETE FROM consultation_forms WHERE patient_id = ?', (patient_id,))
    deleted_consultations = cursor.rowcount
    print(f"Deleted {deleted_consultations} consultation forms")
    
    cursor.execute('DELETE FROM medical_reports WHERE patient_id = ?', (patient_id,))
    deleted_reports = cursor.rowcount
    print(f"Deleted {deleted_reports} medical reports")
    
    cursor.execute('DELETE FROM new_patient_forms WHERE patient_id = ?', (patient_id,))
    deleted_new_patient_forms = cursor.rowcount
    print(f"Deleted {deleted_new_patient_forms} new patient forms")
    
    # Finally delete the patient
    cursor.execute('DELETE FROM patients WHERE id = ?', (patient_id,))
    print(f"Deleted patient {patient_id}")
    
    conn.commit()
    conn.close()
    
    return {
        "message": "Patient and all associated documents deleted successfully",
        "deleted_documents": {
            "prescription_forms": deleted_prescriptions,
            "consultation_forms": deleted_consultations,
            "medical_reports": deleted_reports,
            "new_patient_forms": deleted_new_patient_forms
        }
    }

