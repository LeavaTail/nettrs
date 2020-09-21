import os
import socket
import pickle

server_ip = "127.0.0.1"
port = 5000
arr = [0,0,0,0,0]

if __name__ == "__main__":
  with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((server_ip,port))
    while True:
      s.sendall(pickle.dumps(arr))
      data = s.recv(1024)
      arr2 = pickle.loads(data)
      arr = [i % 2 for i in arr2]
      print(arr)
