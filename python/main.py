############################################################################################################
# THIS NEEDS TO BE THE MAIN OF OUR WHOLE PROJECT IN ORDER TO PROPERLY MAKE COMMUNICATION WORK
############################################################################################################

# 1 IMPORT OSC SERVER LIBRARIES HERE
from pythonosc.osc_server import AsyncIOOSCUDPServer  # OSC server
from pythonosc.dispatcher import Dispatcher  # OSC Sender
from pythonosc.udp_client import SimpleUDPClient  # OSC client
import asyncio  # Async rx
# 2 OTHER LIBRARIES HERE
import cv2  # image processing via openCv
import mediapipe as mp  # pose estimation via mediapipe
import os  # platform agnostic paths
# 3 EXTERNAL FUNCTIONS
from pose_estimation import init_pose_estimation
from pose_estimation import get_body_position
# 4 IMPORT HANDLER FUNCTIONS HERE -> THEY BETTER ONLY MODIFY THE GLOBAL MODEL WITH THE RECEIVED PARAMETERS
from osc_receivers import default_handler
from osc_receivers import pose_handler
# 5 CREATE SENDER FUNCTIONS HERE -> THEY ARE ONLY NEEDED TO FORMAT COMPLEX OR MULTIPLE SENDS
from osc_senders import pose_sender
import socket

# 6 CREATE MODEL OBJECT HERE
# global_model = ...

# 7 CREATE DISPATCHER AND MAPPING OF OSC MESSAGES TO CORRESPONDING HANDLERS HERE. PLEASE KEEP DEFAULT HANDLER
dispatcher = Dispatcher()
dispatcher.map("/pose", pose_handler)
dispatcher.set_default_handler(default_handler)

# 8 DEFINE IP, PORT AND CLIENT FOR OSC COMMUNICATION HERE
# take personal ip
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.connect(("8.8.8.8", 80))
personal_ip = s.getsockname()[0]
s.close()

send_ip_1 = personal_ip
# send_ip_2 = "192.168.168.127"
my_ip = "127.0.0.1"
send_port = 5511
listen_port = 1999
client_1 = SimpleUDPClient(send_ip_1, send_port)  # Create client
# client_2 = SimpleUDPClient(send_ip_2, send_port)
client_self = SimpleUDPClient(my_ip, send_port)


# 9 PUT CODE FOR THE REST OF THE PROJECT IN app_main()
async def app_main():
    cap, mpDraw, mpPose, pose_cv, pose, poseLandmarksArray = init_pose_estimation()
    while True:
        await asyncio.sleep(0.00001)
        ret, img = cap.read()
        if ret:
            img, pose = get_body_position(img, mpDraw, mpPose, pose_cv, pose, poseLandmarksArray)  # pose detection
            pose_sender(client_1, pose)
            # pose_sender(client_2, pose)
            pose_sender(client_self, pose)
            cv2.imshow('Image', img)
        else:
            return
    # for i in range(10):
    #     print(f"Loop {i}")
    # 9 PLEASE USE MY OSC CLIENT WRAPPER THIS WAY
    # subroutine_call(..., client, ...) in order to use the same client and at most
    # define other functions into the subroutine
    # Simple sends
    # client.send_message('/position', 1)
    # Complex sends
    # specific_sender(22, 333)
    # await asyncio.sleep(1) # While sleeping the control is passed back to the osc message check function.


# 10 WRITE THE INITIALIZER MAIN FOR THE OSC SERVER AND RUN IT
async def init_main():
    server = AsyncIOOSCUDPServer((my_ip, listen_port), dispatcher, asyncio.get_event_loop())
    transport, protocol = await server.create_serve_endpoint()  # Create datagram endpoint and start serving

    await app_main()  # Enter main loop of program -> MUST CONTAIN A asyncio.sleep(something) IN ORDER TO WORK

    transport.close()  # Clean up serve endpoint


asyncio.run(init_main())
