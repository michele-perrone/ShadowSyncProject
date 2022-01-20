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

def pose_sender(client, pose):
    for i in pose:
        for j in pose[i]:
            address = "/other_pose/" + i + "/" + j  # example: address = /pose/face/_centroid[ ... ]
            coordinates = []
            for index in range(len(pose[i][j])):
                coordinates.append(pose[i][j][index])
            if (j == "right_shoulder"):
                print(coordinates)
            client.send_message(address,
                                coordinates
                                )

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

to_server = SimpleUDPClient("79.53.59.224", 5501)
to_me = SimpleUDPClient("127.0.1.1", listen_port)

async def app_main():

    cap, mpDraw, mpPose, pose_cv, pose, poseLandmarksArray = init_pose_estimation()
    while True:
        await asyncio.sleep(0.01)
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
    server = AsyncIOOSCUDPServer(("0.0.0.0", listen_port), dispatcher, asyncio.get_event_loop())
    transport, protocol = await server.create_serve_endpoint()

    await app_main()

    transport.close()


asyncio.run(init_main())
