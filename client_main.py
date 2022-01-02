from pythonosc.osc_server import AsyncIOOSCUDPServer
from pythonosc.dispatcher import Dispatcher
from pythonosc.udp_client import SimpleUDPClient
import asyncio

import cv2  # image processing via openCv
import mediapipe as mp  # pose estimation via mediapipe
import os  # platform agnostic paths

from pose_estimation import init_pose_estimation
from pose_estimation import get_body_position

DEBUG = 1

#CLIENT_NUMBER = 1
CLIENT_NUMBER = 2

# Function that answers to the areyouonline request
def ack():
    to_server.send_message("/pyUtil/ack", CLIENT_NUMBER)

# Pose sender
def pose_sender(client, pose):
    #client.send_message("/pose", [pose['left_shoulder'], pose['right_shoulder']])   # Send float message
    # client.send_message("/pose", [
    #     pose['left_shoulder'][0],
    #     pose['left_shoulder'][1],
    #     pose['right_shoulder'][0],
    #     pose['right_shoulder'][1],
    #     pose['left_hip'][0],
    #     pose['left_hip'][1],
    #     pose['right_hip'][0],
    #     pose['right_hip'][1],
    #     pose['body'][0],
    #     pose['body'][1],
    #     ])
    to_server.send_message("/pose", CLIENT_NUMBER, 2, 3, 4)

# Function that handles the areyouonline request and answers
def ping_handler(address, *args):
    if DEBUG==1:
        print("Ping Received, answering...")
    ack()

# Default Handler
def default_handler(address, *args):
    print(f"DEFAULT {address}: {args}")

dispatcher = Dispatcher()
dispatcher.map("/pyUtil/ping", ping_handler)
# dispatcher.map("/pose", pose_handler)
dispatcher.set_default_handler(default_handler)

if CLIENT_NUMBER==1:
    listen_port = 5511
elif CLIENT_NUMBER==2:
    listen_port = 5522

to_server = SimpleUDPClient("192.168.130.225", 1255)
to_me = SimpleUDPClient("127.0.1.1", listen_port)

async def app_main():

    cap, mpDraw, mpPose, pose_cv, pose, poseLandmarksArray = init_pose_estimation()
    while True:
        await asyncio.sleep(0.00001)
        ret, img = cap.read()
        img = cv2.flip(img, 1)
        if ret:
            img, pose = get_body_position(img, mpDraw, mpPose, pose_cv, pose, poseLandmarksArray)  # pose detection
            pose_sender(to_server, pose)
            pose_sender(to_me, pose)
            cv2.imshow('Image', img)
        else:
            return

async def init_main():
    server = AsyncIOOSCUDPServer(("127.0.1.1", listen_port), dispatcher, asyncio.get_event_loop())
    transport, protocol = await server.create_serve_endpoint()

    await app_main()

    transport.close()


asyncio.run(init_main())
