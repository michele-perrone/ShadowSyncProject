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

DEBUG = 1

# STATUS
global_model = Model()

# Print for status monitoring
def print_connection_status(*args, sep = ' '):
    # HOW TO IMPLEMENT END AND FLUSH?
    now = datetime.now()
    timestamp = datetime.timestamp(now)
    readable_timestamp = datetime.fromtimestamp(timestamp)
    print('\r')
    print('[%s] Computer 1: ' % readable_timestamp, end='')
    print('\033[92mONLINE\033[0m' if m.computer_online[1]==1 else '\033[91mOFFLINE\033[0m', end='')
    print(' | Computer 2: ', end='')
    print('\033[92mONLINE\033[0m' if m.computer_online[2]==1 else '\033[91mOFFLINE\033[0m', flush=True)


#################
# OSC RECEIVERS #
#################

def ack_handler(address, *args):
    if DEBUG==1:
        print(args[0], "Ack Received")
    m.ack[args[0]] = 1
    turnedON_handler(address, *args)

def turnedON_handler(address, *args):
    m.computer_online[args[0]] = 1
    if DEBUG==1:
        print(args[0], "is now", m.computer_online[1], m.computer_online[2])

def turnedOFF_handler(address, *args):
    m.computer_online[args[0]] = 0
    if DEBUG==1:
        print(args[0], "is now", m.computer_online[1], m.computer_online[2])

# Default Handler
def default_handler(address, *args):
    print(f"DEFAULT {address}: {args}")

dispatcher = Dispatcher()
dispatcher.map("/pyUtil/ack", ack_handler)
dispatcher.map("/pyUtil/turnedON", turnedON_handler)
dispatcher.map("/pyUtil/turnedOFF", turnedOFF_handler)
dispatcher.set_default_handler(default_handler)

to_computer1 = SimpleUDPClient("192.168.130.2", 5510)
to_computer2 = SimpleUDPClient("192.168.130.213", 5520)

listen_port = 1255
to_me = SimpleUDPClient("127.0.0.1", listen_port)

def ping_loop():
    if DEBUG==1:
        print("Ping")

    if m.ack[1]==0:
        m.computer_online[1] = 0
    else:
        m.ack[1] = 0

    if m.ack[2]==0:
        m.computer_online[2] = 0
    else:
        m.ack[2] = 0

    to_computer1.send_message("/pyUtil/ping", 0)
    to_computer2.send_message("/pyUtil/ping", 0)

    time.sleep(1)

async def app_main():
  
    cap, mpDraw, mpPose, pose_cv = init_pose_estimation()
    while(True):
        if DEBUG==1:
            print("Starting ping thread")
        ping=Thread(target=ping_loop)
        ping.setDaemon(True)
        ping.start()

        await asyncio.sleep(1)

async def init_main():
    server = AsyncIOOSCUDPServer(("127.0.1.1", listen_port), dispatcher, asyncio.get_event_loop())
    transport, protocol = await server.create_serve_endpoint() 
    await app_main() 
    transport.close() 


asyncio.run(init_main())