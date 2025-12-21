"""Consultation Forms endpoints (multiple per patient)"""
from fastapi import APIRouter, HTTPException
from datetime import datetime
from typing import List, Optional
from pydantic import BaseModel
from app.database import get_db_connection

router = APIRouter()


class ConsultationFormBase(BaseModel):
    patient_id: int
    custom_name: Optional[str] = None
    date: str
    symptoms: Optional[str] = None
    vital_signs: Optional[str] = None
    assessment: Optional[str] = None
    plan: Optional[str] = None


class ConsultationFormResponse(ConsultationFormBase):
    id: int
    created_at: str
    updated_at: str


@router.get("/patient/{patient_id}", response_model=List[ConsultationFormResponse])
async def get_consultation_forms(patient_id: int):
    """Get all consultation forms for a specific patient"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    cursor.execute('SELECT * FROM consultation_forms WHERE patient_id = ? ORDER BY created_at DESC', (patient_id,))
    rows = cursor.fetchall()
    conn.close()
    
    return [dict(row) for row in rows]


@router.get("/{form_id}", response_model=ConsultationFormResponse)
async def get_consultation_form(form_id: int):
    """Get a single consultation form by ID"""
    conn = get_db_connection()
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM consultation_forms WHERE id = ?', (form_id,))
    row = cursor.fetchone()
    conn.close()
    if not row:
        raise HTTPException(status_code=404, detail="Form not found")
    return dict(row)


@router.post("/", response_model=ConsultationFormResponse)
async def create_consultation_form(form_data: ConsultationFormBase):
    """Create a new consultation form"""
    conn = get_db_connection()
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM patients WHERE id = ?', (form_data.patient_id,))
    if not cursor.fetchone():
        conn.close()
        raise HTTPException(status_code=404, detail="Patient not found")
    
    current_time = datetime.now().isoformat()
    cursor.execute('''
        INSERT INTO consultation_forms (patient_id, custom_name, date, symptoms, vital_signs, assessment, plan, created_at, updated_at)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
    ''', (form_data.patient_id, form_data.custom_name, form_data.date, form_data.symptoms, form_data.vital_signs, form_data.assessment, form_data.plan, current_time, current_time))
    
    conn.commit()
    form_id = cursor.lastrowid
    conn.close()
    
    return {**form_data.model_dump(), 'id': form_id, 'created_at': current_time, 'updated_at': current_time}


@router.put("/{form_id}", response_model=ConsultationFormResponse)
async def update_consultation_form(form_id: int, form_data: ConsultationFormBase):
    """Update an existing consultation form"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    # Fetch existing record to get created_at
    cursor.execute('SELECT * FROM consultation_forms WHERE id = ?', (form_id,))
    existing = cursor.fetchone()
    if not existing:
        conn.close()
        raise HTTPException(status_code=404, detail="Form not found")
    
    current_time = datetime.now().isoformat()
    cursor.execute('''
        UPDATE consultation_forms SET custom_name = ?, date = ?, symptoms = ?, vital_signs = ?, assessment = ?, plan = ?, updated_at = ?
        WHERE id = ?
    ''', (form_data.custom_name, form_data.date, form_data.symptoms, form_data.vital_signs, form_data.assessment, form_data.plan, current_time, form_id))
    
    conn.commit()
    conn.close()
    
    # Get created_at from existing record
    created_at = dict(existing)['created_at']
    
    return {**form_data.model_dump(), 'id': form_id, 'created_at': created_at, 'updated_at': current_time}


@router.delete("/{form_id}")
async def delete_consultation_form(form_id: int):
    """Delete a consultation form"""
    conn = get_db_connection()
    cursor = conn.cursor()
    cursor.execute('DELETE FROM consultation_forms WHERE id = ?', (form_id,))
    conn.commit()
    conn.close()
    return {"message": "Form deleted successfully"}

