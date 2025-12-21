"""
New Patient Forms endpoints (unique per patient)
"""
from fastapi import APIRouter, HTTPException
from datetime import datetime
from typing import List
from pydantic import BaseModel
from app.database import get_db_connection

router = APIRouter()


class NewPatientFormBase(BaseModel):
    patient_id: int
    custom_name: str = None
    date: str
    patient_name: str = None
    date_of_birth: str = None
    gender: str = None
    contact_info: str = None
    chief_complaint: str = None
    present_illness: str = None
    past_medical_history: str = None
    medications: str = None
    allergies: str = None
    family_history: str = None
    social_history: str = None
    vital_signs: str = None
    physical_exam: str = None
    assessment: str = None
    plan: str = None
    follow_up: str = None


class NewPatientFormResponse(NewPatientFormBase):
    id: int
    created_at: str
    updated_at: str
    
    class Config:
        from_attributes = True


@router.get("/patient/{patient_id}", response_model=List[NewPatientFormResponse])
async def get_new_patient_form(patient_id: int):
    """Get new patient form for a specific patient (unique per patient)"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    cursor.execute('''
        SELECT * FROM new_patient_forms WHERE patient_id = ?
    ''', (patient_id,))
    
    rows = cursor.fetchall()
    conn.close()
    
    if not rows:
        return []
    
    return [dict(row) for row in rows]


@router.get("/{form_id}", response_model=NewPatientFormResponse)
async def get_new_patient_form_by_id(form_id: int):
    """Get a single new patient form by ID"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    cursor.execute('SELECT * FROM new_patient_forms WHERE id = ?', (form_id,))
    row = cursor.fetchone()
    conn.close()
    
    if not row:
        raise HTTPException(status_code=404, detail="Form not found")
    
    return dict(row)


@router.post("/", response_model=NewPatientFormResponse)
async def create_new_patient_form(form_data: NewPatientFormBase):
    """Create or update new patient form (unique per patient)"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    # Verify patient exists
    cursor.execute('SELECT * FROM patients WHERE id = ?', (form_data.patient_id,))
    if not cursor.fetchone():
        conn.close()
        raise HTTPException(status_code=404, detail="Patient not found")
    
    # Check if form already exists for this patient
    cursor.execute('SELECT * FROM new_patient_forms WHERE patient_id = ?', (form_data.patient_id,))
    existing = cursor.fetchone()
    
    current_time = datetime.now().isoformat()
    
    if existing:
        # Update existing form
        cursor.execute('''
            UPDATE new_patient_forms SET
                custom_name = ?, date = ?, patient_name = ?, date_of_birth = ?,
                gender = ?, contact_info = ?, chief_complaint = ?, present_illness = ?,
                past_medical_history = ?, medications = ?, allergies = ?,
                family_history = ?, social_history = ?, vital_signs = ?,
                physical_exam = ?, assessment = ?, plan = ?, follow_up = ?,
                updated_at = ?
            WHERE patient_id = ?
        ''', (
            form_data.custom_name, form_data.date, form_data.patient_name,
            form_data.date_of_birth, form_data.gender, form_data.contact_info,
            form_data.chief_complaint, form_data.present_illness, form_data.past_medical_history,
            form_data.medications, form_data.allergies, form_data.family_history,
            form_data.social_history, form_data.vital_signs, form_data.physical_exam,
            form_data.assessment, form_data.plan, form_data.follow_up, current_time,
            form_data.patient_id
        ))
        form_id = existing['id']
    else:
        # Create new form
        cursor.execute('''
            INSERT INTO new_patient_forms (
                patient_id, custom_name, date, patient_name, date_of_birth, gender,
                contact_info, chief_complaint, present_illness, past_medical_history,
                medications, allergies, family_history, social_history, vital_signs,
                physical_exam, assessment, plan, follow_up, created_at, updated_at
            ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
        ''', (
            form_data.patient_id, form_data.custom_name, form_data.date,
            form_data.patient_name, form_data.date_of_birth, form_data.gender,
            form_data.contact_info, form_data.chief_complaint, form_data.present_illness,
            form_data.past_medical_history, form_data.medications, form_data.allergies,
            form_data.family_history, form_data.social_history, form_data.vital_signs,
            form_data.physical_exam, form_data.assessment, form_data.plan,
            form_data.follow_up, current_time, current_time
        ))
        form_id = cursor.lastrowid
    
    conn.commit()
    conn.close()
    
    return {**form_data.model_dump(), 'id': form_id, 'created_at': current_time, 'updated_at': current_time}


@router.put("/{form_id}", response_model=NewPatientFormResponse)
async def update_new_patient_form(form_id: int, form_data: NewPatientFormBase):
    """Update an existing new patient form"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    # Check if form exists
    cursor.execute('SELECT * FROM new_patient_forms WHERE id = ?', (form_id,))
    existing = cursor.fetchone()
    if not existing:
        conn.close()
        raise HTTPException(status_code=404, detail="Form not found")
    
    # Verify patient exists
    cursor.execute('SELECT * FROM patients WHERE id = ?', (form_data.patient_id,))
    if not cursor.fetchone():
        conn.close()
        raise HTTPException(status_code=404, detail="Patient not found")
    
    current_time = datetime.now().isoformat()
    
    cursor.execute('''
        UPDATE new_patient_forms SET
            patient_id = ?, custom_name = ?, date = ?, patient_name = ?,
            date_of_birth = ?, gender = ?, contact_info = ?, chief_complaint = ?,
            present_illness = ?, past_medical_history = ?, medications = ?,
            allergies = ?, family_history = ?, social_history = ?,
            vital_signs = ?, physical_exam = ?, assessment = ?, plan = ?,
            follow_up = ?, updated_at = ?
        WHERE id = ?
    ''', (
        form_data.patient_id, form_data.custom_name, form_data.date,
        form_data.patient_name, form_data.date_of_birth, form_data.gender,
        form_data.contact_info, form_data.chief_complaint, form_data.present_illness,
        form_data.past_medical_history, form_data.medications, form_data.allergies,
        form_data.family_history, form_data.social_history, form_data.vital_signs,
        form_data.physical_exam, form_data.assessment, form_data.plan,
        form_data.follow_up, current_time, form_id
    ))
    
    conn.commit()
    conn.close()
    
    # Get the original created_at from the existing form
    created_at = dict(existing)['created_at'] if existing else current_time
    
    return {**form_data.model_dump(), 'id': form_id, 'created_at': created_at, 'updated_at': current_time}


@router.delete("/{form_id}")
async def delete_new_patient_form_by_id(form_id: int):
    """Delete new patient form by ID"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    cursor.execute('DELETE FROM new_patient_forms WHERE id = ?', (form_id,))
    conn.commit()
    conn.close()
    
    return {"message": "Form deleted successfully"}


@router.delete("/patient/{patient_id}")
async def delete_new_patient_form(patient_id: int):
    """Delete new patient form for a specific patient"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    cursor.execute('DELETE FROM new_patient_forms WHERE patient_id = ?', (patient_id,))
    conn.commit()
    conn.close()
    
    return {"message": "Form deleted successfully"}

