from pythonosc.osc_server import AsyncIOOSCUDPServer
from pythonosc.dispatcher import Dispatcher
from pythonosc.udp_client import SimpleUDPClient
import asyncio
import sys
import time
from datetime import datetime
import builtins as __builtin__
from model import Model

# STATUS BITS
m = Model()

# Custom print for status monitoring
def print(*args, sep = ' '):
    # HOW TO IMPLEMENT END AND FLUSH?
    now = datetime.now()
    timestamp = datetime.timestamp(now)
    readable_timestamp = datetime.fromtimestamp(timestamp)

    __builtin__.print(*args, sep=sep, end='', flush=True)
    for i in range(100):
        sys.stdout.write(' ')
    __builtin__.print('')
    
    sys.stdout.write('\r')
    sys.stdout.write('[%s] Computer 1: ' % readable_timestamp)
    sys.stdout.write('\033[92mONLINE\033[0m' if m.computer_1_online==1 else '\033[91mOFFLINE\033[0m')
    sys.stdout.write(' | Computer 2: ')
    sys.stdout.write('\033[92mONLINE\033[0m' if m.computer_2_online==1 else '\033[91mOFFLINE\033[0m')
    sys.stdout.write('\r')
    sys.stdout.flush()

# Function that answers to the areyouonline request
def areyouonline(computer_number):
    if computer_number==1:
        to_computer1.send_message("/pyUtil/areyouonline", 0)
    elif computer_number==2:
        to_computer2.send_message("/pyUtil/areyouonline", 0)

# Function that handles the areyouonline request and answers
def iamonline_handler(address, *args):
    # __builtin__.print(args[0], "ONLINE")
    turnedON_handler(address, *args)

def turnedON_handler(address, *args):
    # __builtin__.print(args[0], "turned on")
    if args[0]==1: 
        m.computer_1_online = 1
    elif args[0]==2:
        m.computer_2_online = 1

def turnedOFF_handler(address, *args):
    # print(args[0], "turned off")
    if args[0]==1: 
        m.computer_1_online = 0
    elif args[0]==2:
        m.computer_2_online = 0

# Default Handler
def default_handler(address, *args):
    print(f"DEFAULT {address}: {args}")

dispatcher = Dispatcher()
dispatcher.map("/pyUtil/iamonline", iamonline_handler)
dispatcher.map("/pyUtil/turnedON", turnedON_handler)
dispatcher.map("/pyUtil/turnedOFF", turnedOFF_handler)
dispatcher.set_default_handler(default_handler)

computer1_ip = "10.0.2.0"
computer2_ip = "10.0.2.0"
my_ip = "127.0.1.1"
# Server listens on 1255 and send back on 5511 for Computer1 and 5522 for Computer2
computer1_port = 5511
computer2_port = 5522
listen_port = 1255
to_computer1 = SimpleUDPClient(computer1_ip, computer1_port)
to_computer2 = SimpleUDPClient(computer2_ip, computer2_port)
to_me = SimpleUDPClient(my_ip, listen_port)

async def app_main():
    for i in range(100):
        print(m.computer_1_online, "Doing things...")
        areyouonline(2)
        # if i==3:
        #     # print("Asking computer 1 if online")
        #     m.computer_1_online = 1
        # if i==52:
            # print("Asking computer 2 if online")
            # areyouonline(2)
        await asyncio.sleep(1)

async def init_main():
    server = AsyncIOOSCUDPServer((my_ip, listen_port), dispatcher, asyncio.get_event_loop())
    transport, protocol = await server.create_serve_endpoint() 
    await app_main() 
    transport.close() 


asyncio.run(init_main())