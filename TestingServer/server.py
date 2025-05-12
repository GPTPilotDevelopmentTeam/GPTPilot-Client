import socket
import time

def start_server(host='127.0.0.1', port=87):
    global server_socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((host, port))
    server_socket.listen(1)
    print(f"Server started, listening on {host}:{port}")

    global conn
    conn, addr = server_socket.accept()
    print(f"Connection established with {addr}")


def loop():
    while True:
        message = "hi"
        reply = ""
        message += '\0'
        try:
            conn.send(message.encode())
            reply = conn.recv(2048)

            time.sleep(1)

            reply_str = reply.decode('utf-8').strip('\0')
            print(f"Received from client: {reply_str}")
        except Exception as e:
            print(e)
            break

    conn.close()
    server_socket.close()

if __name__ == '__main__':
    while True:
        start_server()
        loop()