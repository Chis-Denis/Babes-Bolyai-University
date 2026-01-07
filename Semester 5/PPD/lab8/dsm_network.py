import socket
import threading
import pickle
import time
from typing import Dict, List, Callable, Optional
from dsm_messages import Message


class NetworkManager:
    def __init__(self, process_id: int, num_processes: int, port: int, 
                 message_handler: Callable):
        self.process_id = process_id
        self.num_processes = num_processes
        self.port = port
        self.message_handler = message_handler
        
        # Network state
        self.socket: Optional[socket.socket] = None
        self.running = False
        self.receive_thread: Optional[threading.Thread] = None
        self.accept_thread: Optional[threading.Thread] = None
        
        # Connection management
        self.connections: Dict[int, socket.socket] = {}
        self.connection_lock = threading.Lock()
        self.pending_connections: List[socket.socket] = []
    
    def start(self):
        self.running = True
        
        # Create and bind server socket
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.socket.bind(('localhost', self.port))
        self.socket.listen(self.num_processes)
        
        # Start receiving thread
        self.receive_thread = threading.Thread(target=self._receive_loop, daemon=True)
        self.receive_thread.start()
        
        # Give other processes time to start, then connect
        time.sleep(0.5)
        self._connect_to_others()
    
    def _connect_to_others(self):
        base_port = 10000
        max_retries = 10
        
        for other_id in range(self.process_id + 1, self.num_processes):
            other_port = base_port + other_id
            for retry in range(max_retries):
                try:
                    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                    sock.connect(('localhost', other_port))
                    with self.connection_lock:
                        self.connections[other_id] = sock
                    break
                except (ConnectionRefusedError, OSError):
                    if retry < max_retries - 1:
                        time.sleep(0.2)
                    else:
                        print(f"Warning: Could not connect to process {other_id}")
    
    def _accept_connections(self):
        while self.running:
            try:
                self.socket.settimeout(1.0)
                conn, addr = self.socket.accept()
                # Store as pending - will be identified by first message
                with self.connection_lock:
                    self.pending_connections.append(conn)
            except socket.timeout:
                continue
            except Exception as e:
                if self.running:
                    print(f"Error accepting connection: {e}")
    
    def _receive_loop(self):
        # Start thread to accept new connections
        self.accept_thread = threading.Thread(target=self._accept_connections, daemon=True)
        self.accept_thread.start()
        
        # Handle messages from established and pending connections
        while self.running:
            # Check pending connections (new connections waiting for identification)
            with self.connection_lock:
                pending_to_check = list(self.pending_connections)
                connections_to_check = list(self.connections.items())
            
            # Process pending connections
            for conn in pending_to_check:
                try:
                    conn.settimeout(0.1)
                    data = conn.recv(4096)
                    if data:
                        msg = pickle.loads(data)
                        # Identify sender and move to established connections
                        with self.connection_lock:
                            if conn in self.pending_connections:
                                self.pending_connections.remove(conn)
                                self.connections[msg.sender_id] = conn
                        self.message_handler(msg, conn)
                except socket.timeout:
                    continue
                except Exception as e:
                    if self.running:
                        print(f"Error receiving from pending connection: {e}")
                    with self.connection_lock:
                        if conn in self.pending_connections:
                            self.pending_connections.remove(conn)
            
            # Process established connections
            for other_id, conn in connections_to_check:
                try:
                    conn.settimeout(0.1)
                    data = conn.recv(4096)
                    if data:
                        msg = pickle.loads(data)
                        self.message_handler(msg, conn)
                except socket.timeout:
                    continue
                except Exception as e:
                    if self.running:
                        print(f"Error receiving from {other_id}: {e}")
                    with self.connection_lock:
                        if other_id in self.connections:
                            del self.connections[other_id]
            
            time.sleep(0.01)
    
    def send_message(self, target_id: int, msg: Message):
        # Set sender ID
        msg.sender_id = self.process_id
        
        # Serialize message
        data = pickle.dumps(msg)
        
        # Handle local message (send to self)
        if target_id == self.process_id:
            self.message_handler(msg, None)
            return
        
        # Send to remote process
        with self.connection_lock:
            if target_id in self.connections:
                try:
                    self.connections[target_id].sendall(data)
                except Exception as e:
                    print(f"Error sending to {target_id}: {e}")
            else:
                # Try to connect if not already connected
                base_port = 10000
                try:
                    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                    sock.connect(('localhost', base_port + target_id))
                    sock.sendall(data)
                    self.connections[target_id] = sock
                except Exception as e:
                    print(f"Error connecting to {target_id}: {e}")
    
    def stop(self):
        self.running = False
        if self.socket:
            self.socket.close()
        with self.connection_lock:
            for conn in self.connections.values():
                conn.close()
            self.connections.clear()
            for conn in self.pending_connections:
                conn.close()
            self.pending_connections.clear()

