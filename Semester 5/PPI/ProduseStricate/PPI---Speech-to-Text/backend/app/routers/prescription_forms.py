"""Prescription Forms endpoints (multiple per patient)"""
from fastapi import APIRouter, HTTPException
from datetime import datetime
from typing import List, Optional
from pydantic import BaseModel
from app.database import get_db_connection

router = APIRouter()


class PrescriptionFormBase(BaseModel):
    patient_id: int
    custom_name: Optional[str] = None
    date: str
    medications: Optional[str] = None
    dosage: Optional[str] = None
    instructions: Optional[str] = None
    follow_up: Optional[str] = None


class PrescriptionFormResponse(PrescriptionFormBase):
    id: int
    created_at: str
    updated_at: str


@router.get("/patient/{patient_id}", response_model=List[PrescriptionFormResponse])
async def get_prescription_forms(patient_id: int):
    """Get all prescription forms for a specific patient"""
    conn = get_db_connection()
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM prescription_forms WHERE patient_id = ? ORDER BY created_at DESC', (patient_id,))
    rows = cursor.fetchall()
    conn.close()
    return [dict(row) for row in rows]


@router.get("/{form_id}", response_model=PrescriptionFormResponse)
async def get_prescription_form(form_id: int):
    """Get a single prescription form by ID"""
    conn = get_db_connection()
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM prescription_forms WHERE id = ?', (form_id,))
    row = cursor.fetchone()
    conn.close()
    if not row:
        raise HTTPException(status_code=404, detail="Form not found")
    return dict(row)


@router.post("/", response_model=PrescriptionFormResponse)
async def create_prescription_form(form_data: PrescriptionFormBase):
    """Create a new prescription form"""
    conn = get_db_connection()
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM patients WHERE id = ?', (form_data.patient_id,))
    if not cursor.fetchone():
        conn.close()
        raise HTTPException(status_code=404, detail="Patient not found")
    
    current_time = datetime.now().isoformat()
    cursor.execute('''
        INSERT INTO prescription_forms (patient_id, custom_name, date, medications, dosage, instructions, follow_up, created_at, updated_at)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
    ''', (form_data.patient_id, form_data.custom_name, form_data.date, form_data.medications, form_data.dosage, form_data.instructions, form_data.follow_up, current_time, current_time))
    
    conn.commit()
    form_id = cursor.lastrowid
    conn.close()
    
    return {**form_data.model_dump(), 'id': form_id, 'created_at': current_time, 'updated_at': current_time}


@router.put("/{form_id}", response_model=PrescriptionFormResponse)
async def update_prescription_form(form_id: int, form_data: PrescriptionFormBase):
    """Update an existing prescription form"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    # Fetch existing record to get created_at
    cursor.execute('SELECT * FROM prescription_forms WHERE id = ?', (form_id,))
    existing = cursor.fetchone()
    if not existing:
        conn.close()
        raise HTTPException(status_code=404, detail="Form not found")
    
    current_time = datetime.now().isoformat()
    cursor.execute('''
        UPDATE prescription_forms SET custom_name = ?, date = ?, medications = ?, dosage = ?, instructions = ?, follow_up = ?, updated_at = ?
        WHERE id = ?
    ''', (form_data.custom_name, form_data.date, form_data.medications, form_data.dosage, form_data.instructions, form_data.follow_up, current_time, form_id))
    
    conn.commit()
    conn.close()
    
    # Get created_at from existing record
    created_at = dict(existing)['created_at']
    
    return {**form_data.model_dump(), 'id': form_id, 'created_at': created_at, 'updated_at': current_time}


@router.delete("/{form_id}")
async def delete_prescription_form(form_id: int):
    """Delete a prescription form"""
    conn = get_db_connection()
    cursor = conn.cursor()
    cursor.execute('DELETE FROM prescription_forms WHERE id = ?', (form_id,))
    conn.commit()
    conn.close()
    return {"message": "Form deleted successfully"}

