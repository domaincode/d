import socket
import threading
import time

SERVER = "127.0.0.1"   # IRC server IP
PORT = 6667            # IRC server port
CHANNEL = "#stress"
NUM_CLIENTS = 50000     # number of fake clients
MESSAGE_INTERVAL = 1   # seconds between messages
NUM_MESSAGES = 4       # how many messages each client sends before QUIT
COMMAND_DELAY = 0.1    # sleep after each command except messages

def irc_client(id):
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((SERVER, PORT))
        nick = f"user{id}"
        user = f"user{id} 0 * :User {id}"

    # IRC handshake with delay
        s.sendall(f"PASS 123\r\n".encode())  
        time.sleep(COMMAND_DELAY)  
        s.sendall(f"NICK {nick}\r\n".encode())  
        time.sleep(COMMAND_DELAY)  
        s.sendall(f"USER {user}\r\n".encode())  
        time.sleep(COMMAND_DELAY)  
        s.sendall(f"JOIN {CHANNEL}\r\n".encode())  
        time.sleep(COMMAND_DELAY)  

        # Send some messages  
        for _ in range(NUM_MESSAGES):  
            msg = f"PRIVMSG {CHANNEL} :Hello from {nick}\r\n"  
            s.sendall(msg.encode())  
            time.sleep(MESSAGE_INTERVAL)  

        # Send QUIT and close with delay  
        s.sendall(f"QUIT :Bye from {nick}\r\n".encode())  
        time.sleep(COMMAND_DELAY)  
        s.close()  

    except Exception as e:  
        print(f"Client {id} error: {e}")

threads = []
for i in range(NUM_CLIENTS):
    t = threading.Thread(target=irc_client, args=(i,))
    t.daemon = True
    t.start()
    time.sleep(0.05)  # slight delay to avoid burst overload
    threads.append(t)


for t in threads:
    t.join()

