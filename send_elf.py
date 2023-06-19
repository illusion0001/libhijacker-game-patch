from pathlib import Path
import socket

def send_elf():
    host = 'PS5_IP'
    port = 9027

    data = Path('test_elf.elf').read_bytes()

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        s.setblocking(True)
        s.send(len(data).to_bytes(8, byteorder='little'))
        s.sendall(data)
        s.close()

if __name__ == '__main__':
    send_elf()
