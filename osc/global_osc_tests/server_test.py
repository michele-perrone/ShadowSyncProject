from pythonosc.osc_server import AsyncIOOSCUDPServer
from pythonosc.dispatcher import Dispatcher
from pythonosc.udp_client import SimpleUDPClient
import asyncio
import sys
import time
from datetime import datetime
import builtins as __builtin__
from model import Model
from threading import Thread

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
    sys.stdout.write('\033[92mONLINE\033[0m' if m.computer_online[1]==1 else '\033[91mOFFLINE\033[0m')
    sys.stdout.write(' | Computer 2: ')
    sys.stdout.write('\033[92mONLINE\033[0m' if m.computer_online[2]==1 else '\033[91mOFFLINE\033[0m')
    sys.stdout.write('\r')
    sys.stdout.flush()

# Function that handles the areyouonline request and answers
def yes_handler(address, *args):
    # __builtin__.print(args[0], "ping received")
    m.ack[args[0]] = 1
    turnedON_handler(address, *args)

def turnedON_handler(address, *args):
    m.computer_online[args[0]] = 1
    # __builtin__.print(args[0], "is now", m.computer_online[1], m.computer_online[2])

def turnedOFF_handler(address, *args):
    m.computer_online[args[0]] = 0
    #  __builtin__.print(args[0], "is now", m.computer_online[1], m.computer_online[2])

# Default Handler
def default_handler(address, *args):
    print(f"DEFAULT {address}: {args}")

dispatcher = Dispatcher()
dispatcher.map("/pyUtil/yes", yes_handler)
dispatcher.map("/pyUtil/turnedON", turnedON_handler)
dispatcher.map("/pyUtil/turnedOFF", turnedOFF_handler)
dispatcher.set_default_handler(default_handler)

computer1_ip = "192.168.130.2"
computer2_ip = "192.168.130.213"
# Server listens on 1255 and send back on 5511 for Computer1 and 5522 for Computer2
computer1_port = 5510
computer2_port = 5520
listen_port = 1255
to_computer1 = SimpleUDPClient(computer1_ip, computer1_port)
to_computer2 = SimpleUDPClient(computer2_ip, computer2_port)
to_me = SimpleUDPClient("127.0.1.1", listen_port)

def ping_loop():
    print("Ping")

    if m.ack[1]==0:
        m.computer_online[1] = 0
    else:
        m.ack[1] = 0

    if m.ack[2]==0:
        m.computer_online[2] = 0
    else:
        m.ack[2] = 0

    to_computer1.send_message("/pyUtil/areyouonline", 0)
    to_computer2.send_message("/pyUtil/areyouonline", 0)

    time.sleep(1)

async def app_main():

    for i in range(100):
        print("Starting ping thread")
        ping_1=Thread(target=ping_loop)
        ping_1.setDaemon(True)
        ping_1.start()

        # to_computer2.send_message("/pyUtil/areyouonline", 0)
        
        # print("Doing things")
        # areyouonline(1)
        # areyouonline(2)
        # if i==3:
        #     # print("Asking computer 1 if online")
        #     m.computer_1_online = 1
        # if i==52:
            # print("Asking computer 2 if online")
            # areyouonline(2)
        await asyncio.sleep(1)

async def init_main():
    server = AsyncIOOSCUDPServer(("127.0.1.1", listen_port), dispatcher, asyncio.get_event_loop())
    transport, protocol = await server.create_serve_endpoint() 
    await app_main() 
    transport.close() 


asyncio.run(init_main())