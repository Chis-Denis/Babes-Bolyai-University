"""Medical Reports endpoints (multiple per patient)"""
from fastapi import APIRouter, HTTPException
from datetime import datetime
from typing import List, Optional
from pydantic import BaseModel
from app.database import get_db_connection

router = APIRouter()


class MedicalReportBase(BaseModel):
    patient_id: int
    custom_name: Optional[str] = None
    date: str
    chief_complaint: Optional[str] = None
    history_of_present_illness: Optional[str] = None
    physical_examination: Optional[str] = None
    diagnosis: Optional[str] = None
    treatment: Optional[str] = None
    recommendations: Optional[str] = None


class MedicalReportResponse(MedicalReportBase):
    id: int
    created_at: str
    updated_at: str


@router.get("/patient/{patient_id}", response_model=List[MedicalReportResponse])
async def get_medical_reports(patient_id: int):
    """Get all medical reports for a specific patient"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    cursor.execute('''
        SELECT * FROM medical_reports WHERE patient_id = ? ORDER BY created_at DESC
    ''', (patient_id,))
    
    rows = cursor.fetchall()
    conn.close()
    
    return [dict(row) for row in rows]


@router.get("/{form_id}", response_model=MedicalReportResponse)
async def get_medical_report(form_id: int):
    """Get a single medical report by ID"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    cursor.execute('SELECT * FROM medical_reports WHERE id = ?', (form_id,))
    row = cursor.fetchone()
    conn.close()
    
    if not row:
        raise HTTPException(status_code=404, detail="Report not found")
    
    return dict(row)


@router.post("/", response_model=MedicalReportResponse)
async def create_medical_report(form_data: MedicalReportBase):
    """Create a new medical report"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    cursor.execute('SELECT * FROM patients WHERE id = ?', (form_data.patient_id,))
    if not cursor.fetchone():
        conn.close()
        raise HTTPException(status_code=404, detail="Patient not found")
    
    current_time = datetime.now().isoformat()
    
    cursor.execute('''
        INSERT INTO medical_reports (
            patient_id, custom_name, date, chief_complaint,
            history_of_present_illness, physical_examination, diagnosis, treatment,
            recommendations, created_at, updated_at
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    ''', (
        form_data.patient_id, form_data.custom_name, form_data.date,
        form_data.chief_complaint, form_data.history_of_present_illness,
        form_data.physical_examination, form_data.diagnosis, form_data.treatment,
        form_data.recommendations, current_time, current_time
    ))
    
    conn.commit()
    form_id = cursor.lastrowid
    conn.close()
    
    return {**form_data.model_dump(), 'id': form_id, 'created_at': current_time, 'updated_at': current_time}


@router.put("/{form_id}", response_model=MedicalReportResponse)
async def update_medical_report(form_id: int, form_data: MedicalReportBase):
    """Update an existing medical report"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    # Fetch existing record to get created_at
    cursor.execute('SELECT * FROM medical_reports WHERE id = ?', (form_id,))
    existing = cursor.fetchone()
    if not existing:
        conn.close()
        raise HTTPException(status_code=404, detail="Report not found")
    
    current_time = datetime.now().isoformat()
    
    cursor.execute('''
        UPDATE medical_reports SET
            custom_name = ?, date = ?, chief_complaint = ?,
            history_of_present_illness = ?, physical_examination = ?,
            diagnosis = ?, treatment = ?, recommendations = ?, updated_at = ?
        WHERE id = ?
    ''', (
        form_data.custom_name, form_data.date, form_data.chief_complaint,
        form_data.history_of_present_illness, form_data.physical_examination,
        form_data.diagnosis, form_data.treatment, form_data.recommendations,
        current_time, form_id
    ))
    
    conn.commit()
    conn.close()
    
    # Get created_at from existing record
    created_at = dict(existing)['created_at']
    
    return {**form_data.model_dump(), 'id': form_id, 'created_at': created_at, 'updated_at': current_time}


@router.delete("/{form_id}")
async def delete_medical_report(form_id: int):
    """Delete a medical report"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    cursor.execute('DELETE FROM medical_reports WHERE id = ?', (form_id,))
    conn.commit()
    conn.close()
    
    return {"message": "Report deleted successfully"}

