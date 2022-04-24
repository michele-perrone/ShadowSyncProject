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
import json

from pose_estimation import init_pose_estimation
from pose_estimation import get_body_position

DEBUG = 1

# STATUS
global_model = Model()

data = open('info.json')
info = json.load(data)


# Print for status monitoring
def print_connection_status(*args, sep=' '):
    # HOW TO IMPLEMENT END AND FLUSH?
    now = datetime.now()
    timestamp = datetime.timestamp(now)
    readable_timestamp = datetime.fromtimestamp(timestamp)
    print('[%s] Computer 1: ' % readable_timestamp, end='')
    print('\033[92mONLINE\033[0m' if global_model.computer_online[1] == 1 else '\033[91mOFFLINE\033[0m', end='')
    print(' | Computer 2: ', end='')
    print('\033[92mONLINE\033[0m' if global_model.computer_online[2] == 1 else '\033[91mOFFLINE\033[0m', flush=True,
          end='\r\r')


def debug_print(message):
    spaces = ' '
    for i in range(70 - len(message)):
        spaces += ' '

    print(message, spaces)


#################
# OSC RECEIVERS #
#################

def ack_handler(address, *args):
    if DEBUG == 1:
        print(args[0], "Ack Received")
    global_model.ack[args[0]] = 1
    global_model.computer_online[args[0]] = 1


def pose_handler(address, *args):
    component = address[5:].split('/')
    # print("pose arrived", args[0])

    if args[0] == 1:
        # Pose comes from client 1 and has to be sent to 1 as pose and to 2 as other_pose
        args = args[1:]
        to_ofx1.send_message(address, args)
        # to_test_ofx.send_message(address, args)
        to_ofx2.send_message('/other_pose' + address[5:], args)

    elif args[0] == 2:
        # Pose comes from client 1 and has to be sent to 2 as pose and to 1 as other_pose
        args = args[1:]
        to_ofx1.send_message('/other_pose' + address[5:], args)
        # to_test_ofx.send_message('/other_pose' + address[5:], args)
        to_ofx2.send_message(address, args)


def correlation_controller(n, correlation):
    if correlation <= 0.2:
        value = [1, 0.05, global_model.oldDur[n], 2]
        global_model.dur[n] = 0.05
    elif 0.2 < correlation <= 0.4:
        value = [1, 0.1, global_model.oldDur[n], 2]
        global_model.dur[n] = 0.1
    elif 0.4 < correlation <= 0.6:
        value = [1, 0.5, global_model.oldDur[n], 1]
        global_model.dur[n] = 0.5
    elif 0.6 < correlation < 0.8:
        value = [1, 1, global_model.oldDur[n], 1]
        global_model.dur[n] = 1
    elif correlation>=0.8:
        value = [1, 3, global_model.oldDur[n], 1]
        global_model.dur[n] = 3
    global_model.oldDur[n] = global_model.dur[n]
    return value


def correlation_handler(address, *args):
    global_model.latest_correlation_value[args[0]] = args[1]

    correlation = global_model.latest_correlation_value[args[0]]

    # if args[0]==1:
    #     print("Correlation 1: ", correlation)
    # elif args[0]==2:
    #     print("Correlation 2: --------->", correlation)
    if global_model.installation_phase == 0:
        to_supercollider1.send_message("/correlation", 0)
        to_supercollider2.send_message("/correlation", 0)
    elif global_model.installation_phase == 1 or global_model.installation_phase == 2:
        value = correlation_controller(args[0], correlation**2)
        print(args[0], value, '-----> ', correlation**2)
        if args[0] == 1:       
            to_supercollider1.send_message("/correlation", value)
        if args[0] == 2:
            to_supercollider2.send_message("/correlation", value)
    elif global_model.installation_phase != 0 and global_model.installation_phase != 1 and global_model.installation_phase != 2:
        correlation = ((global_model.latest_correlation_value[1] + global_model.latest_correlation_value[2]) / 2)**2 
        # print("MEDIA CORRELATION ---------------------------->",  correlation)
        value = correlation_controller(args[0], correlation)
        to_supercollider1.send_message("/correlation", value)
        to_supercollider2.send_message("/correlation", value)

    # # FINAL
    # to_supercollider.send_message("/correlation", (global_model.latest_correlation_value[1]+global_model.latest_correlation_value[2])/2)
    # PLACEHOLDER


def tutorial_handler(address, *args):
    # if args[1] >= 2:
    #     if args[0] == 1:
    #         to_ofx1.send_message("/ofxUtil/startForReal", 0)
    #     elif args[0] == 2:
    #         to_ofx2.send_message("/ofxUtil/startForReal", 0)
    # elif args[1] <= 0:
    #     return
    # else:
    #     start_tutorial_phase(args[1] + 1)
    print("TUTORIAL PHASE", args[1], "OF CLIENT", args[0], 'COMPLETE')


# Default Handler
def default_handler(address, *args):
    print(f"DEFAULT {address}: {args}")


dispatcher = Dispatcher()
dispatcher.map("/pyUtil/ack", ack_handler)
dispatcher.map("/pose/*", pose_handler)
dispatcher.map("/ofxUtil/tutorialComplete", tutorial_handler)
dispatcher.map("/ofxUtil/correlation", correlation_handler)
dispatcher.set_default_handler(default_handler)

to_py1 = SimpleUDPClient(info["ip_client_1"], 5511)
to_py2 = SimpleUDPClient(info["ip_client_2"], 5522)
to_ofx1 = SimpleUDPClient(info["ip_client_1"], 5501)
to_ofx2 = SimpleUDPClient(info["ip_client_2"], 5502)

supercollider_port = 5555
to_supercollider1 = SimpleUDPClient(info["ip_client_1"], supercollider_port)
to_supercollider2 = SimpleUDPClient(info["ip_client_2"], supercollider_port)

# UNCOMMENT ONLY FOR SINGLE FRONTEND ON SERVER FOR TESTING
to_test_ofx = SimpleUDPClient("127.0.0.1", 5501)

my_ip = "127.0.0.1"
listen_port = 1255
to_me = SimpleUDPClient(my_ip, listen_port)


def start_tutorial_phase(n):
    print('STARTING PHASE ----------------------------------------------------------------> ', n)
    global_model.installation_phase = n
    to_ofx1.send_message("/ofxUtil/startTutorial", n)
    to_ofx2.send_message("/ofxUtil/startTutorial", n)


def update_installation_phase():
    if DEBUG:
        debug_print('Sending blends.')
    to_ofx1.send_message("/ofxUtil/blend", float(global_model.blend))
    to_ofx2.send_message("/ofxUtil/blend", float(global_model.blend))


def start_blend_sequence():  # INACTIVE
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
    started = 0

    # to_supercollider1.send_message("/correlation", "start")
    # to_supercollider2.send_message("/correlation", "start")

    while (True):

        if DEBUG == 1:
            debug_print('Ping')

        if global_model.ack[1] == 0:
            global_model.computer_online[1] = 0
        else:
            global_model.ack[1] = 0

        if global_model.ack[2] == 0:
            global_model.computer_online[2] = 0
        else:
            global_model.ack[2] = 0

        to_py1.send_message("/pyUtil/ping", 0)
        to_py2.send_message("/pyUtil/ping", 0)

        if started == 0:
            print_connection_status()

        # global_model.blend = 0
        # for i in range(24):
        #     update_installation_phase()

        if global_model.has_started == 0 and keyboard.is_pressed('alt+w'):
            # global_model.has_started = 1
            # blend_sequence = Thread(target=start_blend_sequence, daemon=True)
            # blend_sequence.start()
            started = 1
            debug_print("Checking if all computers are online in order to start.")
            if global_model.computer_online[1] == 1 and global_model.computer_online[2] == 1:
                debug_print("3...")
                await asyncio.sleep(1)
                debug_print("2...")
                await asyncio.sleep(1)
                debug_print("1...")
                await asyncio.sleep(1)
                debug_print("START!")
                to_py1.send_message("/pyUtil/start", 0)
                to_py2.send_message("/pyUtil/start", 0)
            else:
                debug_print("Not all computers are online!")
                # to_py1.send_message("/pyUtil/start", 0)
                # to_py2.send_message("/pyUtil/start", 0)

        # if keyboard.is_pressed('ctrl+t'):
        #     debug_print("Starting Automated Tutorial")
        #     start_tutorial_phase(1)

        if keyboard.is_pressed('alt+t+1'):
            debug_print("Starting Automated Tutorial")
            start_tutorial_phase(1)

        if keyboard.is_pressed('alt+t+2'):
            debug_print("Starting Automated Tutorial")
            start_tutorial_phase(2)

        if keyboard.is_pressed('alt+shift+w'):
            debug_print("Starting Automated Tutorial")
            global_model.installation_phase = 3
            to_ofx1.send_message("/ofxUtil/startForReal", 0)
            to_ofx2.send_message("/ofxUtil/startForReal", 0)
            global_model.blend = 1
            update_installation_phase()
            to_supercollider1.send_message("/correlation", "start")
            to_supercollider2.send_message("/correlation", "start")

        # if keyboard.is_pressed('alt+c'):

        if keyboard.is_pressed('alt+w+1'): # DA CAMBIARE
            debug_print("Starting Installation Full Regime")
            to_ofx1.send_message("/ofxUtil/startForReal", 0)

        if keyboard.is_pressed('alt+w+2'):
            debug_print("Starting Installation Full Regime")
            to_ofx2.send_message("/ofxUtil/startForReal", 0)

        if keyboard.is_pressed('alt+q'):
            to_py1.send_message("/pyUtil/stop", 0)
            to_py2.send_message("/pyUtil/stop", 0)
            break

        await asyncio.sleep(1)

    # to_supercollider1.send_message("/correlation", 0)
    # to_supercollider2.send_message("/correlation", 0)


async def init_main():
    server = AsyncIOOSCUDPServer(("0.0.0.0", listen_port), dispatcher, asyncio.get_event_loop())
    transport, protocol = await server.create_serve_endpoint()
    await app_main()
    transport.close()


asyncio.run(init_main())
