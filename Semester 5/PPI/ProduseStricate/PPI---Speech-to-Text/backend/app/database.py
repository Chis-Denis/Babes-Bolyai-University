"""
Database setup and configuration
"""
import sqlite3
import os
from datetime import datetime
from typing import Optional

DB_PATH = os.path.join(os.path.dirname(__file__), '..', 'medical_records.db')

def get_db_connection():
    """Get database connection"""
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row  # Enable column access by name
    return conn


def init_db():
    """Initialize database with tables if they don't exist"""
    conn = get_db_connection()
    cursor = conn.cursor()
    
    # Create patients table
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS patients (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            age INTEGER,
            gender TEXT,
            date_of_birth TEXT,
            phone TEXT,
            email TEXT,
            address TEXT,
            medical_history TEXT,
            allergies TEXT,
            current_medications TEXT,
            blood_type TEXT,
            insurance_number TEXT,
            emergency_contact TEXT,
            created_at TEXT NOT NULL,
            updated_at TEXT NOT NULL
        )
    ''')
    
    # Create new_patient_forms table (unique per patient)
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS new_patient_forms (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            patient_id INTEGER NOT NULL UNIQUE,
            custom_name TEXT,
            date TEXT NOT NULL,
            patient_name TEXT,
            date_of_birth TEXT,
            gender TEXT,
            contact_info TEXT,
            chief_complaint TEXT,
            present_illness TEXT,
            past_medical_history TEXT,
            medications TEXT,
            allergies TEXT,
            family_history TEXT,
            social_history TEXT,
            vital_signs TEXT,
            physical_exam TEXT,
            assessment TEXT,
            plan TEXT,
            follow_up TEXT,
            created_at TEXT NOT NULL,
            updated_at TEXT NOT NULL,
            FOREIGN KEY (patient_id) REFERENCES patients(id) ON DELETE CASCADE
        )
    ''')
    
    # Create medical_reports table (multiple per patient)
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS medical_reports (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            patient_id INTEGER NOT NULL,
            custom_name TEXT,
            date TEXT NOT NULL,
            chief_complaint TEXT,
            history_of_present_illness TEXT,
            physical_examination TEXT,
            diagnosis TEXT,
            treatment TEXT,
            recommendations TEXT,
            created_at TEXT NOT NULL,
            updated_at TEXT NOT NULL,
            FOREIGN KEY (patient_id) REFERENCES patients(id) ON DELETE CASCADE
        )
    ''')
    
    # Create consultation_forms table (multiple per patient)
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS consultation_forms (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            patient_id INTEGER NOT NULL,
            custom_name TEXT,
            date TEXT NOT NULL,
            symptoms TEXT,
            vital_signs TEXT,
            assessment TEXT,
            plan TEXT,
            created_at TEXT NOT NULL,
            updated_at TEXT NOT NULL,
            FOREIGN KEY (patient_id) REFERENCES patients(id) ON DELETE CASCADE
        )
    ''')
    
    # Create prescription_forms table (multiple per patient)
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS prescription_forms (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            patient_id INTEGER NOT NULL,
            custom_name TEXT,
            date TEXT NOT NULL,
            medications TEXT,
            dosage TEXT,
            instructions TEXT,
            follow_up TEXT,
            created_at TEXT NOT NULL,
            updated_at TEXT NOT NULL,
            FOREIGN KEY (patient_id) REFERENCES patients(id) ON DELETE CASCADE
        )
    ''')
    
    conn.commit()
    conn.close()
    print("Database initialized successfully")


def check_and_init_db():
    """Check if database exists, if not create it"""
    if not os.path.exists(DB_PATH):
        print("Database not found. Creating new database...")
        init_db()
    else:
        print("Database exists. Checking tables...")
        conn = get_db_connection()
        cursor = conn.cursor()
        
        # Check if tables exist
        cursor.execute("SELECT name FROM sqlite_master WHERE type='table' AND name='patients'")
        if not cursor.fetchone():
            print("Tables not found. Initializing database...")
            init_db()
        else:
            cursor.execute("SELECT COUNT(*) FROM patients")
            patient_count = cursor.fetchone()[0]
            print(f"Database and tables exist. Found {patient_count} patients.")
        
        conn.close()




if __name__ == '__main__':
    check_and_init_db()

