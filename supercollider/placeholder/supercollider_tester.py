from pythonosc.osc_server import AsyncIOOSCUDPServer
from pythonosc.dispatcher import Dispatcher
from pythonosc.udp_client import SimpleUDPClient
import asyncio
import sys
import time
from datetime import datetime
import builtins as __builtin__
from threading import Thread

# Default Handler
def default_handler(address, *args):
    print(f"DEFAULT {address}: {args}")

dispatcher = Dispatcher()
dispatcher.set_default_handler(default_handler)

my_ip = "127.0.0.1"
listen_port = 1255
supercollider_port = 5555
to_me = SimpleUDPClient(my_ip, listen_port)
to_supercollider = SimpleUDPClient(my_ip, supercollider_port)

def send(correlation_value):
    to_supercollider.send_message("/correlation", 100*correlation_value)


async def app_main():

    value = 0

    for i in range(100):
        value += 0.07
        if value>1:
            value = value-1
        print('Sending', value)
        await asyncio.sleep(1)
        send(value)

async def init_main():
    server = AsyncIOOSCUDPServer(("0.0.0.0", listen_port), dispatcher, asyncio.get_event_loop())
    transport, protocol = await server.create_serve_endpoint()
    await app_main()
    transport.close()


asyncio.run(init_main())
