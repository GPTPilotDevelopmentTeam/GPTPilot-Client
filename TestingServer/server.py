import socket
import time
import json

DATAREF_OPTIONS = {
    "1": ("throttle", "Throttle (0.0 ~ 1.0)"),
    "2": ("flap", "Flap (0.0 ~ 1.0)"),
    "3": ("gear", "Gear (0 = up, 1 = down)"),
    "4": ("speedbrake", "Speedbrake (0.0 ~ 1.0)"),
    "5": ("handbrake", "Handbrake (0.0 ~ 1.0)"),
    "6": ("ap_on", "Autopilot On/Off (0 = off, 1 = on)"),
    "7": ("ap_heading", "AP Heading (degrees)"),
    "8": ("ap_altitude", "AP Altitude (feet)"),
    "9": ("ap_airspeed", "AP Airspeed (knots)"),
    "10": ("ap_heading_hold", "AP Heading Hold (0 = off, 1 = on)"),
    "11": ("ap_alt_hold", "AP Altitude Hold (0 = off, 1 = on)")
}

def start_server(host='127.0.0.1', port=87):
    global server_socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind((host, port))
    server_socket.listen(1)
    print(f"[SERVER] Listening on {host}:{port}...")

    global conn
    conn, addr = server_socket.accept()
    print(f"[SERVER] Connection established with {addr}")

def loop():
    while True:
        try:
            print("\n--- Select which datarefs to modify ---")
            for key, (_, desc) in DATAREF_OPTIONS.items():
                print(f"{key}. {desc}")
            print("Type multiple numbers separated by comma (e.g., 1,2,6,9)")

            selected = input("Your choice: ").strip().split(",")
            message_list = []

            for sel in selected:
                sel = sel.strip()
                if sel in DATAREF_OPTIONS:
                    operation, prompt = DATAREF_OPTIONS[sel]
                    value = input(f"Enter value for {prompt}: ").strip()
                    message_list.append({
                        "action": "operate_aircraft",
                        "operation": operation,
                        "target_value": float(value)
                    })
                else:
                    print(f"[WARN] Unknown selection: {sel}")

            json_message = json.dumps(message_list).replace(" ", "") + '\0'
            conn.send(json_message.encode())

            reply = conn.recv(2048)
            if not reply:
                print("[SERVER] Client disconnected.")
                break

            reply_str = reply.decode('utf-8').strip('\0')
            print(f"[SERVER] Received from client: {reply_str}")

            time.sleep(0.2)

        except Exception as e:
            print(f"[SERVER] Error: {e}")
            break

    conn.close()
    server_socket.close()
    print("[SERVER] Connection closed.")

if __name__ == '__main__':
    while True:
        start_server()
        loop()
