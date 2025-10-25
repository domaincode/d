import socket
import threading
import time
import random

SERVER = "127.0.0.1"   # IRC server IP
PORT = 6667            # IRC server port
CHANNEL = "#stress"
NUM_CLIENTS = 1000     # number of fake clients
COMMAND_DELAY = 0.05   # delay after each IRC command
CONNECT_DELAY = 0.01   # delay between creating clients
MESSAGE_INTERVAL = 10  # seconds between each message

def irc_client(id):
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(5)
        s.connect((SERVER, PORT))

        nick = f"user{id}"
        user = f"user{id} 0 * :User {id}"

        # --- IRC handshake ---
        s.sendall(f"PASS 123\r\n".encode())
        time.sleep(COMMAND_DELAY)
        s.sendall(f"NICK {nick}\r\n".encode())
        time.sleep(COMMAND_DELAY)
        s.sendall(f"USER {user}\r\n".encode())
        time.sleep(COMMAND_DELAY)
        s.sendall(f"JOIN {CHANNEL}\r\n".encode())
        time.sleep(COMMAND_DELAY)

        print(f"✅ {nick} connected and joined {CHANNEL}")

        # --- Stay alive and send messages periodically ---
        while True:
            msg = f"PRIVMSG {CHANNEL} :Hello from {nick}\r\n"
            try:
                s.sendall(msg.encode())
                print(f"💬 {nick} -> {CHANNEL}")
            except Exception as e:
                print(f"❌ {nick} send error: {e}")
                break  # connection likely closed, stop this client

            # wait before next message
            time.sleep(MESSAGE_INTERVAL)

    except Exception as e:
        print(f"❌ Client {id} error: {e}")

threads = []

for i in range(NUM_CLIENTS):
    t = threading.Thread(target=irc_client, args=(i,))
    t.daemon = True
    t.start()
    threads.append(t)
    time.sleep(CONNECT_DELAY)

# Keep the main thread alive
try:
    while True:
        time.sleep(60)
except KeyboardInterrupt:
    print("Stopped by user.")
