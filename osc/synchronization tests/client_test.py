from pythonosc.osc_server import AsyncIOOSCUDPServer
from pythonosc.dispatcher import Dispatcher
from pythonosc.udp_client import SimpleUDPClient
import asyncio
from datetime import datetime, timedelta

CLIENT_NUMBER = 1
# CLIENT_NUMBER = 2

m = Model()

DEBUG=1

awake_time = datetime.timestamp(datetime.now())

# Function that answers to the areyouonline request
def ack():
    to_server.send_message( "/pyUtil/ack" , [CLIENT_NUMBER, awake_time] )

# Function that handles the areyouonline request and answers
def ping_handler(address, *args):
    if DEBUG==1:
        print("Ping Received, answering...")
    ack()

# Function that handles the areyouonline request and answers
def start_handler(address, *args):
    if DEBUG==1:
        print("Start received, beginning...")
    m.START = 1

# Default Handler
def default_handler(address, *args):
    print(f"DEFAULT {address}: {args}")

dispatcher = Dispatcher()
dispatcher.map("/pyUtil/ping", ping_handler)
dispatcher.map("/pyUtil/start", start_handler)
dispatcher.set_default_handler(default_handler)

server_ip = "192.168.207.225"
my_ip = "127.0.1.1"
# Server listens on 1255 and send back on 5511 for Computer1 and 5522 for Computer2
server_port = 1255
if CLIENT_NUMBER==1:
    listen_port = 5511
else:
    listen_port = 5522

print("Listening on port", listen_port)

to_server = SimpleUDPClient(server_ip, server_port)
to_me = SimpleUDPClient(my_ip, listen_port)

async def app_main():

    now = datetime.timestamp(datetime.now())

    while True:
        await asyncio.sleep(1)
        if m.START==1:
            print("LETS START BICCIS")
        else:
            print("No indication on when to start ")
        

async def init_main():
    server = AsyncIOOSCUDPServer(("0.0.0.0", listen_port), dispatcher, asyncio.get_event_loop())
    transport, protocol = await server.create_serve_endpoint()

    await app_main() 

    transport.close()


asyncio.run(init_main())
