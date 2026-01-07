import threading
import time
from typing import Dict, List, Callable, Optional
from collections import deque

from dsm_messages import Message, MessageType
from dsm_network import NetworkManager


class DSM:
    def __init__(self, process_id: int, num_processes: int, port: int, 
                 subscriptions: Dict[int, List[int]], callback: Callable):
        self.process_id = process_id
        self.num_processes = num_processes
        self.port = port
        self.subscriptions = subscriptions
        self.callback = callback
        
        # Local variable storage: var_id -> value
        self.variables: Dict[int, int] = {}
        
        # Initialize variables this process subscribes to
        for var_id, subscribers in subscriptions.items():
            if process_id in subscribers:
                self.variables[var_id] = 0  # Initialize to 0
        
        # Logical clock for total ordering (Lamport timestamps)
        self.logical_clock = 0
        self._next_expected_timestamp = 0
        
        # Message queue for total ordering (sorted by timestamp)
        self.pending_messages: deque = deque()
        self.pending_lock = threading.Lock()
        
        # Message ID counter for unique message identification
        self.msg_counter = 0
        self.msg_counter_lock = threading.Lock()
        
        # CAS operation tracking: msg_id -> operation info
        self.cas_operations: Dict[int, Dict] = {}
        self.cas_lock = threading.Lock()
        
        # Write operation tracking for total ordering: msg_id -> ack info
        self.write_acks: Dict[int, Dict] = {}
        self.write_lock = threading.Lock()
        
        # Initialize network manager
        self.network = NetworkManager(process_id, num_processes, port, 
                                     self._handle_message)
    
    def _get_next_msg_id(self) -> int:
        with self.msg_counter_lock:
            self.msg_counter += 1
            return self.msg_counter
    
    def _increment_clock(self) -> int:
        self.logical_clock += 1
        return self.logical_clock
    
    def _update_clock(self, received_timestamp: int):
        self.logical_clock = max(self.logical_clock, received_timestamp) + 1
    
    def start(self):
        self.network.start()
    
    def stop(self):
        self.network.stop()
    
    # ==================== Message Handling ====================
    
    def _handle_message(self, msg: Message, conn: Optional[object]):
        # Update logical clock based on received timestamp
        self._update_clock(msg.timestamp)
        
        # Route message to appropriate handler
        if msg.type == MessageType.WRITE:
            self._handle_write(msg)
        elif msg.type == MessageType.WRITE_ACK:
            self._handle_write_ack(msg)
        elif msg.type == MessageType.WRITE_COMMIT:
            self._handle_write_commit(msg)
        elif msg.type == MessageType.CAS:
            self._handle_cas(msg)
        elif msg.type == MessageType.CAS_ACK:
            self._handle_cas_ack(msg)
        elif msg.type == MessageType.CAS_COMMIT:
            self._handle_cas_commit(msg)
        elif msg.type == MessageType.CAS_ABORT:
            self._handle_cas_abort(msg)
    
    # ==================== Write Operation Handlers ====================
    
    def _handle_write(self, msg: Message):
        var_id = msg.var_id
        value = msg.value
        
        # Check if we're subscribed to this variable
        if var_id not in self.subscriptions or self.process_id not in self.subscriptions[var_id]:
            return
        
        # Update variable locally
        self.variables[var_id] = value
        
        # Send ACK with our logical clock timestamp
        ack = Message(MessageType.WRITE_ACK, var_id, msg.process_id, 
                     timestamp=self._increment_clock(), msg_id=msg.msg_id)
        self.network.send_message(msg.sender_id, ack)
    
    def _handle_write_ack(self, msg: Message):
        with self.write_lock:
            if msg.msg_id not in self.write_acks:
                return
            
            ack_info = self.write_acks[msg.msg_id]
            ack_info['subscribers'].add(msg.sender_id)
            ack_info['timestamps'].append(msg.timestamp)
            
            var_id = ack_info['var_id']
            subscribers = self.subscriptions.get(var_id, [])
            
            # Check if we have all acks
            if len(ack_info['subscribers']) == len(subscribers):
                # All acks received - determine final timestamp for total ordering
                # Use max of all timestamps + 1 to ensure ordering
                final_timestamp = max(ack_info['timestamps']) + 1
                value = ack_info['value']
                
                # Broadcast commit to all subscribers with agreed timestamp
                commit_msg = Message(MessageType.WRITE_COMMIT, var_id, self.process_id,
                                   value=value, timestamp=final_timestamp, 
                                   msg_id=msg.msg_id)
                for sub_id in subscribers:
                    self.network.send_message(sub_id, commit_msg)
                
                # Trigger callback locally with agreed timestamp
                self._trigger_callback_ordered(var_id, value, final_timestamp, 
                                              msg.msg_id, self.process_id)
                
                del self.write_acks[msg.msg_id]
    
    def _handle_write_commit(self, msg: Message):
        var_id = msg.var_id
        value = msg.value
        
        # Trigger callback with the agreed timestamp for total ordering
        self._trigger_callback_ordered(var_id, value, msg.timestamp, 
                                      msg.msg_id, msg.sender_id)
    
    # ==================== CAS Operation Handlers ====================
    
    def _handle_cas(self, msg: Message):
        var_id = msg.var_id
        old_val = msg.old_value
        new_val = msg.new_value
        
        # Check if we're subscribed to this variable
        if var_id not in self.subscriptions or self.process_id not in self.subscriptions[var_id]:
            return
        
        # Check if current value matches expected value
        current_value = self.variables.get(var_id, 0)
        success = (current_value == old_val)
        
        # Send ACK with result and our logical clock
        ack = Message(MessageType.CAS_ACK, var_id, msg.process_id,
                     value=1 if success else 0,
                     timestamp=self._increment_clock(), msg_id=msg.msg_id)
        self.network.send_message(msg.sender_id, ack)
    
    def _handle_cas_ack(self, msg: Message):
        with self.cas_lock:
            if msg.msg_id not in self.cas_operations:
                return
            
            op = self.cas_operations[msg.msg_id]
            op['acks'].add(msg.sender_id)
            op['results'][msg.sender_id] = (msg.value == 1)
            op['timestamps'].append(msg.timestamp)
            
            var_id = op['var_id']
            subscribers = self.subscriptions.get(var_id, [])
            
            # Check if we have all acks
            if len(op['acks']) == len(subscribers):
                # All acks received - check if all succeeded
                all_success = all(op['results'].values())
                
                # Determine final timestamp for total ordering
                if op.get('timestamps'):
                    final_timestamp = max(op['timestamps']) + 1
                else:
                    final_timestamp = self._increment_clock()
                
                if all_success:
                    # All checks passed - commit the CAS operation
                    commit_msg = Message(MessageType.CAS_COMMIT, var_id, op['process_id'],
                                       old_value=op['old_value'], new_value=op['new_value'],
                                       timestamp=final_timestamp, msg_id=msg.msg_id)
                    for sub_id in subscribers:
                        self.network.send_message(sub_id, commit_msg)
                else:
                    # At least one check failed - abort
                    abort_msg = Message(MessageType.CAS_ABORT, var_id, op['process_id'],
                                      timestamp=final_timestamp, msg_id=msg.msg_id)
                    for sub_id in subscribers:
                        self.network.send_message(sub_id, abort_msg)
                
                del self.cas_operations[msg.msg_id]
    
    def _handle_cas_commit(self, msg: Message):
        var_id = msg.var_id
        new_value = msg.new_value
        
        # Update variable
        self.variables[var_id] = new_value
        
        # Trigger callback with total ordering
        self._trigger_callback_ordered(var_id, new_value, msg.timestamp, 
                                      msg.msg_id, msg.sender_id)
    
    def _handle_cas_abort(self, msg: Message):
        # CAS failed, no variable update, no callback
        pass
    
    # ==================== Callback Ordering ====================
    
    def _trigger_callback_ordered(self, var_id: int, value: int, timestamp: int, 
                                  msg_id: int, sender_id: int):
        with self.pending_lock:
            # Add to pending queue
            self.pending_messages.append({
                'var_id': var_id,
                'value': value,
                'timestamp': timestamp,
                'msg_id': msg_id,
                'sender_id': sender_id
            })
            
            # Sort by timestamp, then sender_id, then msg_id for deterministic ordering
            sorted_list = sorted(self.pending_messages, 
                                key=lambda x: (x['timestamp'], x['sender_id'], x['msg_id']))
            self.pending_messages = deque(sorted_list)
            
            # Process messages in order
            expected_timestamp = getattr(self, '_next_expected_timestamp', 0)
            
            while self.pending_messages:
                msg_data = self.pending_messages[0]
                # Process if timestamp is acceptable
                if msg_data['timestamp'] >= expected_timestamp:
                    self.pending_messages.popleft()
                    self._next_expected_timestamp = msg_data['timestamp'] + 1
                    self.callback(msg_data['var_id'], msg_data['value'])
                else:
                    # Wait for earlier messages
                    break
    
    # ==================== Public API ====================
    
    def write(self, var_id: int, value: int) -> bool:
        # Check subscription
        if var_id not in self.subscriptions or self.process_id not in self.subscriptions[var_id]:
            return False
        
        subscribers = self.subscriptions[var_id]
        msg_id = self._get_next_msg_id()
        timestamp = self._increment_clock()
        
        # Initialize ack tracking
        with self.write_lock:
            self.write_acks[msg_id] = {
                'subscribers': set(),
                'timestamps': [],
                'var_id': var_id,
                'value': value
            }
        
        # Update local variable optimistically
        if self.process_id in subscribers:
            self.variables[var_id] = value
        
        # Send write request to all subscribers
        for sub_id in subscribers:
            msg = Message(MessageType.WRITE, var_id, self.process_id, value=value,
                         timestamp=timestamp, msg_id=msg_id)
            self.network.send_message(sub_id, msg)
        
        return True
    
    def compare_and_swap(self, var_id: int, old_value: int, new_value: int) -> bool:
        # Check subscription
        if var_id not in self.subscriptions or self.process_id not in self.subscriptions[var_id]:
            return False
        
        subscribers = self.subscriptions[var_id]
        msg_id = self._get_next_msg_id()
        timestamp = self._increment_clock()
        
        # Initialize CAS operation tracking
        with self.cas_lock:
            self.cas_operations[msg_id] = {
                'var_id': var_id,
                'old_value': old_value,
                'new_value': new_value,
                'process_id': self.process_id,
                'acks': set(),
                'results': {},
                'timestamps': []
            }
        
        # Send CAS request to all subscribers
        for sub_id in subscribers:
            msg = Message(MessageType.CAS, var_id, self.process_id,
                         old_value=old_value, new_value=new_value,
                         timestamp=timestamp, msg_id=msg_id)
            self.network.send_message(sub_id, msg)
        
        # Wait for all acks and decision
        # In a real implementation, this would use condition variables
        # For simplicity, we'll poll with timeout
        max_wait = 5.0  # 5 seconds timeout
        start_time = time.time()
        while time.time() - start_time < max_wait:
            with self.cas_lock:
                if msg_id not in self.cas_operations:
                    # Operation completed - check if successful
                    return self.variables.get(var_id, 0) == new_value
            time.sleep(0.01)
        
        return False
    
    def get_value(self, var_id: int) -> Optional[int]:
        if var_id in self.variables:
            return self.variables[var_id]
        return None

