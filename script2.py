import socket
import threading
import time

SERVER = "127.0.0.1"   # your IRC server IP
PORT = 6667            # your IRC server port
CHANNEL = "#stress"
NUM_CLIENTS = 10000     # how many fake clients to create
COMMAND_DELAY = 0.05   # delay after each IRC command
CONNECT_DELAY = 0.01   # delay between creating clients
NUM_MESSAGES = 4      # <-- each client will send exactly 4 messages
MESSAGE_INTERVAL = 2   # seconds between each message
KEEP_ALIVE_INTERVAL = 60  # seconds, loop sleep after sending messages

# track sockets so we can close them cleanly
sockets = []
sockets_lock = threading.Lock()

def irc_client(id):
    s = None
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(10)
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

        with sockets_lock:
            sockets.append(s)

        print(f"✅ {nick} connected and joined {CHANNEL}")

        # --- Send exactly NUM_MESSAGES then keep connection alive ---
        for i in range(NUM_MESSAGES):
            msg = f"PRIVMSG {CHANNEL} :Hello ({i+1}/{NUM_MESSAGES}) from {nick}\r\n"
            try:
                s.sendall(msg.encode())
                print(f"💬 {nick} -> {CHANNEL} ({i+1}/{NUM_MESSAGES})")
            except Exception as e:
                print(f"❌ {nick} send error: {e}")
                # remove socket from list if broken
                with sockets_lock:
                    if s in sockets:
                        sockets.remove(s)
                s.close()
                return
            time.sleep(MESSAGE_INTERVAL)

        # Now the client stays connected but stops sending messages
        while True:
            time.sleep(KEEP_ALIVE_INTERVAL)

    except Exception as e:
        print(f"❌ Client {id} error: {e}")
        if s:
            try:
                s.close()
            except:
                pass
    finally:
        # if socket still in list and closed, remove it
        with sockets_lock:
            if s in sockets:
                # keep it there while alive; if it's closed earlier remove it
                pass

# start threads
threads = []
for i in range(NUM_CLIENTS):
    t = threading.Thread(target=irc_client, args=(i,))
    t.start()
    threads.append(t)
    time.sleep(CONNECT_DELAY)

# main: wait and gracefully close on Ctrl+C
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print("\nStopping — closing all sockets...")
    with sockets_lock:
        for s in list(sockets):
            try:
                s.close()
            except:
                pass
        sockets.clear()
    # optionally join threads (they'll exit when socket closed)
    for t in threads:
        t.join(timeout=0.1)
    print("All sockets closed. Exiting.")
