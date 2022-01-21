from pythonosc.osc_server import AsyncIOOSCUDPServer
from pythonosc.dispatcher import Dispatcher
from pythonosc.udp_client import SimpleUDPClient
import asyncio
import sys
import time
import keyboard
from threading import Thread
from datetime import datetime, timedelta
import builtins as __builtin__
from model import Model

from pose_estimation import init_pose_estimation
from pose_estimation import get_body_position

DEBUG = 1

# STATUS
global_model = Model()

# Print for status monitoring
def print_connection_status(*args, sep = ' '):
    # HOW TO IMPLEMENT END AND FLUSH?
    now = datetime.now()
    timestamp = datetime.timestamp(now)
    readable_timestamp = datetime.fromtimestamp(timestamp)
    print('[%s] Computer 1: ' % readable_timestamp, end='')
    print('\033[92mONLINE\033[0m' if global_model.computer_online[1]==1 else '\033[91mOFFLINE\033[0m', end='')
    print(' | Computer 2: ', end='')
    print('\033[92mONLINE\033[0m' if global_model.computer_online[2]==1 else '\033[91mOFFLINE\033[0m', flush=True, end='\r\r')


def debug_print(message):
    spaces = ' '
    for i in range(70-len(message)):
        spaces += ' '
        
    print(message, spaces)

#################
# OSC RECEIVERS #
#################

def ack_handler(address, *args):
    if DEBUG==1:
        print(args[0], "Ack Received")
    global_model.ack[args[0]] = 1
    global_model.computer_online[args[0]] = 1

def pose_handler(address, *args):
    component = address[5:].split('/')
    if args[0]==1:
        # Pose comes from client 1 and has to be sent to 1 as pose and to 2 as other_pose
        args = args[1:]
        to_ofx1.send_message(address, args)
        to_ofx2.send_message('/other_pose' + address[5:], args)

    elif args[0]==2:
        # Pose comes from client 1 and has to be sent to 2 as pose and to 1 as other_pose
        args = args[1:]
        to_ofx1.send_message('/other_pose' + address[5:], args)
        to_ofx2.send_message(address, args)

def correlation_handler(address, *args):
    global_model.latest_correlation_value[args[0]] = args[1]
    # # FINAL
    # to_supercollider.send_message("/correlation", (global_model.latest_correlation_value[1]+global_model.latest_correlation_value[2])/2)
    # PLACEHOLDER
    to_supercollider.send_message("/correlation", [0, 0.5, 440*(global_model.latest_correlation_value[1]+global_model.latest_correlation_value[2])/2])

# Default Handler
def default_handler(address, *args):
    print(f"DEFAULT {address}: {args}")

dispatcher = Dispatcher()
dispatcher.map("/pyUtil/ack", ack_handler)
dispatcher.map("/pose/*", pose_handler)
dispatcher.map("/ofxUtil/correlation", correlation_handler)
dispatcher.set_default_handler(default_handler)

ip_1 = "192.168.207.2"
ip_2 = "192.168.207.213"

to_py1 = SimpleUDPClient(ip_1, 5511)
to_py2 = SimpleUDPClient(ip_2, 5522)
to_ofx1 = SimpleUDPClient(ip_1, 5501)
to_ofx2 = SimpleUDPClient(ip_2, 5502)

my_ip = "127.0.0.1"
listen_port = 1255
supercollider_port = 5555
to_me = SimpleUDPClient(my_ip, listen_port)
to_supercollider = SimpleUDPClient(my_ip, supercollider_port)

def update_installation_phase():
    if DEBUG:
        debug_print('Sending blends.')
    to_ofx1.send_message("/ofxUtil/blend", float(global_model.blend))
    to_ofx2.send_message("/ofxUtil/blend", float(global_model.blend))

def start_blend_sequence():
    global_model.blend = 0
    update_installation_phase()
    time.sleep(20)
    global_model.blend = 0.5
    update_installation_phase()
    time.sleep(20)
    global_model.blend = 1
    update_installation_phase()

async def app_main():

    cap, mpDraw, mpPose, pose_cv, pose, poseLandmarksArray = init_pose_estimation()
    while(True):
        if DEBUG==1:
            debug_print('Ping')

        if global_model.ack[1]==0:
            global_model.computer_online[1] = 0
        else:
            global_model.ack[1] = 0

        if global_model.ack[2]==0:
            global_model.computer_online[2] = 0
        else:
            global_model.ack[2] = 0

        to_py1.send_message("/pyUtil/ping", 0)
        to_py2.send_message("/pyUtil/ping", 0)

        print_connection_status()

        if global_model.has_started == 0 and keyboard.is_pressed('ctrl+w'):
            global_model.has_started = 1
            blend_sequence = Thread(target=start_blend_sequence, daemon=True)
            blend_sequence.start()
            
            debug_print("Checking if all computers are online in order to start.")
            if m.computer_online[1]==1 and m.computer_online[2]==1:
                debug_print("START!")
                to_computer1.send_message("/pyUtil/start", 0)
                to_computer2.send_message("/pyUtil/start", 0)
            else:
                debug_print("Not all computers are online!")

        if keyboard.is_pressed('ctrl+q'):
            break

        await asyncio.sleep(1)

async def init_main():
    server = AsyncIOOSCUDPServer(("0.0.0.0", listen_port), dispatcher, asyncio.get_event_loop())
    transport, protocol = await server.create_serve_endpoint()
    await app_main()
    transport.close()


asyncio.run(init_main())
