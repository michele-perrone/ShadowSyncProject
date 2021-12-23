from pythonosc.osc_server import AsyncIOOSCUDPServer
from pythonosc.dispatcher import Dispatcher
from pythonosc.udp_client import SimpleUDPClient
import asyncio

#CLIENT_NUMBER = 1
CLIENT_NUMBER = 2

# Function that answers to the areyouonline request
def yes():
    to_server.send_message("/pyUtil/yes", CLIENT_NUMBER)

# Function that handles the areyouonline request and answers
def areyouonline_handler(address, *args):
    print("RICEVUTO STRONZI")
    yes()

# Default Handler
def default_handler(address, *args):
    print(f"DEFAULT {address}: {args}")

dispatcher = Dispatcher()
dispatcher.map("/pyUtil/areyouonline", areyouonline_handler)
dispatcher.set_default_handler(default_handler)

server_ip = "192.168.130.225"
my_ip = "127.0.1.1"
# Server listens on 1255 and send back on 5511 for Computer1 and 5522 for Computer2
server_port = 1254
if CLIENT_NUMBER==1:
    listen_port = 5511
else:
    listen_port = 5522

print("Listening on port", listen_port)

to_server = SimpleUDPClient(server_ip, server_port)
to_me = SimpleUDPClient(my_ip, listen_port)

async def app_main():
    for i in range(10):
        print(f"Loop {i}")
        await asyncio.sleep(1)

async def init_main():
    server = AsyncIOOSCUDPServer((my_ip, listen_port), dispatcher, asyncio.get_event_loop())
    transport, protocol = await server.create_serve_endpoint()

    await app_main()

    transport.close()


asyncio.run(init_main())
