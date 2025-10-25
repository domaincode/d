import socket
import threading
import time

SERVER = "127.0.0.1"   # your IRC server IP
PORT = 6667            # your IRC server port
CHANNEL = "#stress"
NUM_CLIENTS = 50000     # number of fake clients to create
COMMAND_DELAY = 0.05   # delay after each IRC command (seconds)
CONNECT_DELAY = 0.01   # delay between starting each client (seconds)
KEEP_ALIVE_INTERVAL = 60  # seconds, how often each client wakes up (keeps thread alive)

def irc_connect_and_hold(id):
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(5)
        s.connect((SERVER, PORT))

        nick = f"user{id}"
        # USER format: "<username> <mode> <unused> :<realname>"
        user = f"user{id} 0 * :User {id}"

        # IRC handshake (no further messages after JOIN)
        s.sendall(f"PASS 123\r\n".encode())
        time.sleep(COMMAND_DELAY)
        s.sendall(f"NICK {nick}\r\n".encode())
        time.sleep(COMMAND_DELAY)
        s.sendall(f"USER {user}\r\n".encode())
        time.sleep(COMMAND_DELAY)
        s.sendall(f"JOIN {CHANNEL}\r\n".encode())
        time.sleep(COMMAND_DELAY)

        print(f"✅ Client {id} connected & joined as {nick}")

        # Keep the socket open indefinitely. Optionally, we could send periodic PING/NOOP to mimic activity.
        while True:
            time.sleep(KEEP_ALIVE_INTERVAL)

    except Exception as e:
        print(f"❌ Client {id} error: {e}")

threads = []

for i in range(NUM_CLIENTS):
    t = threading.Thread(target=irc_connect_and_hold, args=(i,))
    t.daemon = True
    t.start()
    threads.append(t)
    time.sleep(CONNECT_DELAY)

# Keep the main thread alive so daemon threads persist
try:
    while True:
        time.sleep(60)
except KeyboardInterrupt:
    print("Stopped by user.")
