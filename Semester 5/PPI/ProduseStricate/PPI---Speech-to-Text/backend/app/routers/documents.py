"""
Medical document management endpoints
"""
from fastapi import APIRouter, HTTPException
from datetime import datetime
from typing import List
import json
from app.models import MedicalDocumentCreate, MedicalDocumentUpdate, MedicalDocumentResponse
from app.database import get_db_connection

router = APIRouter()


@router.get("/", response_model=List[MedicalDocumentResponse])
async def get_documents():
    """Get all medical documents"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    cursor.execute('''
        SELECT * FROM medical_documents ORDER BY created_at DESC
    ''')
    
    rows = cursor.fetchall()
    documents = []
    
    for row in rows:
        documents.append({
            'id': row['id'],
            'patient_id': row['patient_id'],
            'patient_name': row['patient_name'],
            'document_type': row['document_type'],
            'document_id': row['document_id'],
            'custom_name': row['custom_name'],
            'date': row['date'],
            'data': json.loads(row['data']) if row['data'] else {},
            'created_at': row['created_at'],
            'updated_at': row['updated_at']
        })
    
    conn.close()
    return documents


@router.get("/patient/{patient_id}", response_model=List[MedicalDocumentResponse])
async def get_documents_by_patient(patient_id: int):
    """Get all medical documents for a specific patient"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    cursor.execute('''
        SELECT * FROM medical_documents WHERE patient_id = ? ORDER BY created_at DESC
    ''', (patient_id,))
    
    rows = cursor.fetchall()
    documents = []
    
    for row in rows:
        documents.append({
            'id': row['id'],
            'patient_id': row['patient_id'],
            'patient_name': row['patient_name'],
            'document_type': row['document_type'],
            'document_id': row['document_id'],
            'custom_name': row['custom_name'],
            'date': row['date'],
            'data': json.loads(row['data']) if row['data'] else {},
            'created_at': row['created_at'],
            'updated_at': row['updated_at']
        })
    
    conn.close()
    return documents


@router.get("/{document_id}", response_model=MedicalDocumentResponse)
async def get_document(document_id: int):
    """Get a single medical document by ID"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    cursor.execute('SELECT * FROM medical_documents WHERE id = ?', (document_id,))
    row = cursor.fetchone()
    conn.close()
    
    if not row:
        raise HTTPException(status_code=404, detail="Document not found")
    
    return {
        'id': row['id'],
        'patient_id': row['patient_id'],
        'patient_name': row['patient_name'],
        'document_type': row['document_type'],
        'document_id': row['document_id'],
        'custom_name': row['custom_name'],
        'date': row['date'],
        'data': json.loads(row['data']) if row['data'] else {},
        'created_at': row['created_at'],
        'updated_at': row['updated_at']
    }


@router.post("/", response_model=MedicalDocumentResponse)
async def create_document(document: MedicalDocumentCreate):
    """Create a new medical document"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    # Verify patient exists
    cursor.execute('SELECT * FROM patients WHERE id = ?', (document.patient_id,))
    if not cursor.fetchone():
        conn.close()
        raise HTTPException(status_code=404, detail="Patient not found")
    
    current_time = datetime.now().isoformat()
    
    cursor.execute('''
        INSERT INTO medical_documents (
            patient_id, patient_name, document_type, document_id,
            custom_name, date, data, created_at, updated_at
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
    ''', (
        document.patient_id, document.patient_name, document.document_type,
        document.document_id, document.custom_name, document.date,
        json.dumps(document.data), current_time, current_time
    ))
    
    conn.commit()
    document_id = cursor.lastrowid
    conn.close()
    
    return {
        **document.model_dump(),
        'id': document_id,
        'created_at': current_time,
        'updated_at': current_time
    }


@router.put("/{document_id}", response_model=MedicalDocumentResponse)
async def update_document(document_id: int, document: MedicalDocumentUpdate):
    """Update an existing medical document"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    # Check if document exists
    cursor.execute('SELECT * FROM medical_documents WHERE id = ?', (document_id,))
    existing = cursor.fetchone()
    if not existing:
        conn.close()
        raise HTTPException(status_code=404, detail="Document not found")
    
    current_time = datetime.now().isoformat()
    
    cursor.execute('''
        UPDATE medical_documents SET
            patient_id = ?, patient_name = ?, document_type = ?,
            document_id = ?, custom_name = ?, date = ?, data = ?,
            updated_at = ?
        WHERE id = ?
    ''', (
        document.patient_id, document.patient_name, document.document_type,
        document.document_id, document.custom_name, document.date,
        json.dumps(document.data), current_time, document_id
    ))
    
    conn.commit()
    conn.close()
    
    return {
        **document.model_dump(),
        'id': document_id,
        'created_at': existing['created_at'],
        'updated_at': current_time
    }


@router.delete("/{document_id}")
async def delete_document(document_id: int):
    """Delete a medical document"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    # Check if document exists
    cursor.execute('SELECT * FROM medical_documents WHERE id = ?', (document_id,))
    if not cursor.fetchone():
        conn.close()
        raise HTTPException(status_code=404, detail="Document not found")
    
    cursor.execute('DELETE FROM medical_documents WHERE id = ?', (document_id,))
    conn.commit()
    conn.close()
    
    return {"message": "Document deleted successfully"}

