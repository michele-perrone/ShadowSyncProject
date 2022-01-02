# Just run this for a connection reminder

import socket

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.connect(("8.8.8.8", 80))
personal_ip = s.getsockname()[0]
s.close()

print('        | Client 1 :                      |                           | 5511 |')
print('If I am | Client 2 : Connect to me via ip | ', personal_ip, ' at port | 5522 |')
print('        |  Server  :                      |                           | 1255 |')