from pythonosc.osc_server import AsyncIOOSCUDPServer
from pythonosc.dispatcher import Dispatcher
from pythonosc.udp_client import SimpleUDPClient
import asyncio

DEBUG = 1

#CLIENT_NUMBER = 1
CLIENT_NUMBER = 2

# Function that answers to the areyouonline request
def ack():
    to_server.send_message("/pyUtil/ack", CLIENT_NUMBER)

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
dispatcher.set_default_handler(default_handler)

if CLIENT_NUMBER==1:
    listen_port = 5511
elif CLIENT_NUMBER==2:
    listen_port = 5522

to_server = SimpleUDPClient("192.168.130.225", 1255)
to_me = SimpleUDPClient("127.0.1.1", listen_port)

async def app_main():
  
    cap, mpDraw, mpPose, pose_cv = init_pose_estimation()
    while(True):

        await asyncio.sleep(0.00001)
        ret, img = cap.read()
        if ret == True:  
            img, pose = get_body_position(img, mpDraw, mpPose, pose_cv) #pose detection
            pose_sender(client_1, pose)
            pose_sender(client_2, pose)
            pose_sender(client_self, pose)
            cv2.imshow('Image', img)
        else: 
            return

async def init_main():
    server = AsyncIOOSCUDPServer((my_ip, listen_port), dispatcher, asyncio.get_event_loop())
    transport, protocol = await server.create_serve_endpoint()

    await app_main()

    transport.close()


asyncio.run(init_main())
