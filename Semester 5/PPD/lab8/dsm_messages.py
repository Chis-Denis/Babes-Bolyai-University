from enum import Enum
from typing import Optional


class MessageType(Enum):
    WRITE = "WRITE"                    # Initial write request
    WRITE_ACK = "WRITE_ACK"            # Acknowledgment of write request
    WRITE_COMMIT = "WRITE_COMMIT"      # Commit message with agreed timestamp
    CAS = "CAS"                        # Compare-and-swap request
    CAS_ACK = "CAS_ACK"                # Acknowledgment of CAS request
    CAS_COMMIT = "CAS_COMMIT"          # Commit message for successful CAS
    CAS_ABORT = "CAS_ABORT"            # Abort message for failed CAS


class Message:
    def __init__(self, msg_type: MessageType, var_id: int, process_id: int, 
                 value: Optional[int] = None, old_value: Optional[int] = None,
                 new_value: Optional[int] = None, timestamp: int = 0, 
                 sender_id: int = 0, msg_id: int = 0):
        self.type = msg_type
        self.var_id = var_id
        self.process_id = process_id
        self.value = value
        self.old_value = old_value
        self.new_value = new_value
        self.timestamp = timestamp
        self.sender_id = sender_id
        self.msg_id = msg_id
    
    def __repr__(self):
        return (f"Message(type={self.type.value}, var_id={self.var_id}, "
                f"process_id={self.process_id}, value={self.value}, "
                f"timestamp={self.timestamp}, sender_id={self.sender_id})")

