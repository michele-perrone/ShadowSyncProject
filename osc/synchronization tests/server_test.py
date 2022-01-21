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
import keyboard

# STATUS BITS
m = Model()

# Print for status monitoring
def print_connection_status():
    # HOW TO IMPLEMENT END AND FLUSH?
    now = datetime.now()
    timestamp = datetime.timestamp(now)
    readable_timestamp = datetime.fromtimestamp(timestamp)
    print('[%s] Computer 1: ' % readable_timestamp, end='')
    print('\033[92mONLINE\033[0m' if m.computer_online[1]==1 else '\033[91mOFFLINE\033[0m', end='')
    print(' | Computer 2: ', end='')
    print('\033[92mONLINE\033[0m' if m.computer_online[2]==1 else '\033[91mOFFLINE\033[0m', flush=True, end='\r\r')


def debug_print(message):
    spaces = ' '
    for i in range(70-len(message)):
        spaces += ' '
        
    print(message, spaces)

# Function that handles the areyouonline request and answers
def ack_handler(address, *args):

    message = "ack received from " + str(args[0])

    debug_print(message)
    m.ack[args[0]] = 1
    m.computer_online[args[0]] = 1

# Default Handler
def default_handler(address, *args):
    debug_print(f"DEFAULT {address}: {args}")

dispatcher = Dispatcher()
dispatcher.map("/pyUtil/ack", ack_handler)
dispatcher.set_default_handler(default_handler)

computer1_ip = "192.168.207.2"
computer2_ip = "192.168.207.213"
# Server listens on 1255 and send back on 5511 for Computer1 and 5522 for Computer2
computer1_port = 5511
computer2_port = 5522
listen_port = 1255
to_computer1 = SimpleUDPClient(computer1_ip, computer1_port)
to_computer2 = SimpleUDPClient(computer2_ip, computer2_port)
to_me = SimpleUDPClient("127.0.1.1", listen_port)

async def app_main():

    for i in range(100):
        debug_print("Ping")

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

        print_connection_status()

        if keyboard.is_pressed('ctrl+w'):
            debug_print("Checking if all computers are online")
            if m.computer_online[1]==1 and m.computer_online[2]==1:
                debug_print("Sending starts!")
                to_computer1.send_message("/pyUtil/start", 0)
                to_computer2.send_message("/pyUtil/start", 0)
            else:
                debug_print("Not all computers are online")

        if keyboard.is_pressed('ctrl+q'):
            break

        await asyncio.sleep(1)

async def init_main():
    server = AsyncIOOSCUDPServer(("0.0.0.0", listen_port), dispatcher, asyncio.get_event_loop())
    transport, protocol = await server.create_serve_endpoint() 
    await app_main() 
    transport.close() 


asyncio.run(init_main())