"""
Pydantic models for request/response validation
"""
from pydantic import BaseModel
from typing import Optional, Dict, Any
from datetime import datetime


class PatientBase(BaseModel):
    """Base patient model"""
    name: str
    age: Optional[int] = None
    gender: Optional[str] = None
    date_of_birth: Optional[str] = None
    phone: Optional[str] = None
    email: Optional[str] = None
    address: Optional[str] = None
    medical_history: Optional[str] = None
    allergies: Optional[str] = None
    current_medications: Optional[str] = None
    blood_type: Optional[str] = None
    insurance_number: Optional[str] = None
    emergency_contact: Optional[str] = None


class PatientCreate(PatientBase):
    """Patient creation model"""
    pass


class PatientUpdate(PatientBase):
    """Patient update model"""
    pass


class PatientResponse(PatientBase):
    """Patient response model"""
    id: int
    created_at: str
    updated_at: str
    
    class Config:
        from_attributes = True


class MedicalDocumentBase(BaseModel):
    """Base medical document model"""
    patient_id: int
    patient_name: str
    document_type: str
    document_id: str
    custom_name: Optional[str] = None
    date: str
    data: Dict[str, Any]


class MedicalDocumentCreate(MedicalDocumentBase):
    """Medical document creation model"""
    pass


class MedicalDocumentUpdate(MedicalDocumentBase):
    """Medical document update model"""
    pass


class MedicalDocumentResponse(MedicalDocumentBase):
    """Medical document response model"""
    id: int
    created_at: str
    updated_at: str
    
    class Config:
        from_attributes = True


class TranscriptionRequest(BaseModel):
    """Speech-to-text transcription request"""
    audio_file: bytes


class TranscriptionResponse(BaseModel):
    """Speech-to-text transcription response"""
    text: str


class ProcessRecordingRequest(BaseModel):
    """Process recording with field list request"""
    fields: list[str]


class ProcessRecordingResponse(BaseModel):
    """Process recording response with parsed data"""
    raw_transcript: str
    parsed_json: Dict[str, Any]
