from socket import *

IP = "192.168.141.226"
port = 1234

s = socket(AF_INET, SOCK_DGRAM)
s.bind((IP, port))

while True:
    d = s.recv(1024)
    print(d)