import socket
import threading
import time

SERVER = "127.0.0.1"   # your IRC server IP
PORT = 6667            # your IRC server port
NUM_CLIENTS = 5000   # number of connection attempts
CONNECT_DELAY = 0.01   # delay between each connection (seconds)

def connect_only(id):
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(5)
        s.connect((SERVER, PORT))
        print(f"✅ Client {id} connected (fd open)")
        # keep socket open
        time.sleep(9999)
        s.close()
    except Exception as e:
        print(f"❌ Client {id} error: {e}")
        

threads = []

for i in range(NUM_CLIENTS):
    t = threading.Thread(target=connect_only, args=(i,))
    t.daemon = True
    t.start()
    time.sleep(CONNECT_DELAY)
    threads.append(t)

for t in threads:
    t.join()

try:
    while True:
        time.sleep(60)
except KeyboardInterrupt:
    print("Stopped.")
