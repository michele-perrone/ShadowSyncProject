############################################################################################################
#THIS NEEDS TO BE THE MAIN OF OUR WHOLE PROJECT IN ORDER TO PROPERLY MAKE COMMUNICATION WORK
############################################################################################################

# 1 IMPORT OSC SERVER LIBRARIES HERE
from pythonosc.osc_server import AsyncIOOSCUDPServer # OSC server
from pythonosc.dispatcher import Dispatcher # OSC Sender
from pythonosc.udp_client import SimpleUDPClient # OSC client
import asyncio # Async rx
# 2 OTHER LIBRARIES HERE
import cv2 #image processing via openCv
import mediapipe as mp #pose estimation via mediapipe
import os # platform agnostic paths
# 3 EXTERNAL FUNCTIONS
from pose_estimation import init_pose_estimation
from pose_estimation import get_body_position 
# 4 IMPORT HANDLER FUNCTIONS HERE -> THEY BETTER ONLY MODIFY THE GLOBAL MODEL WITH THE RECEIVED PARAMETERS
from osc_receivers import default_handler
from osc_receivers import pose_handler
# 5 CREATE SENDER FUNCTIONS HERE -> THEY ARE ONLY NEEDED TO FORMAT COMPLEX OR MULTIPLE SENDS
from osc_senders import pose_sender

# 6 CREATE MODEL OBJECT HERE
# global_model = ...

# 7 CREATE DISPATCHER AND MAPPING OF OSC MESSAGES TO CORRESPONDING HANDLERS HERE. PLEASE KEEP DEFAULT HANDLER
dispatcher = Dispatcher()
dispatcher.map("/pose", pose_handler)
dispatcher.set_default_handler(default_handler)

# 8 DEFINE IP, PORT AND CLIENT FOR OSC COMMUNICATION HERE
ip = "127.0.1.1"
port = 1998
client = SimpleUDPClient(ip, port)  # Create client

# 9 PUT CODE FOR THE REST OF THE PROJECT IN app_main()
async def app_main():
    cap = init_pose_estimation()
    while(True):
        await asyncio.sleep(0.001)
        ret, img = cap.read()
        if ret == True:  
            img, pose = get_body_position(img) #pose detection
            cv2.imshow('Image', img)
        else: 
            return
    # for i in range(10):
    #     print(f"Loop {i}")
        # 9 PLEASE USE MY OSC CLIENT WRAPPER THIS WAY
        # subroutine_call(..., client, ...) in order to use the same client and at most define other functions into the subroutine
        # Simple sends
        # client.send_message('/position', 1)
        # Complex sends
        # specific_sender(22, 333)
        #await asyncio.sleep(1) # While sleeping the control is passed back to the osc message check function.

# 10 WRITE THE INITIALIZER MAIN FOR THE OSC SERVER AND RUN IT
async def init_main():
    server = AsyncIOOSCUDPServer((ip, port), dispatcher, asyncio.get_event_loop())
    transport, protocol = await server.create_serve_endpoint()  # Create datagram endpoint and start serving

    await app_main()  # Enter main loop of program -> MUST CONTAIN A asyncio.sleep(something) IN ORDER TO WORK

    transport.close()  # Clean up serve endpoint


asyncio.run(init_main())