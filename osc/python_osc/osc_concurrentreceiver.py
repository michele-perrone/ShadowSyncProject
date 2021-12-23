from pythonosc.osc_server import AsyncIOOSCUDPServer
from pythonosc.dispatcher import Dispatcher
from pythonosc.udp_client import SimpleUDPClient
import asyncio

def specific_sender(a, b):
    client.send_message("/position", a)   # Send float message
    client.send_message("/position", b)

def ss2(a, b, c):
    client.send_message("/some/address", [a, 2., c, "hello"])  # Send message with int, float and string


def filter_handler(address, *args):
    print(f"{address}: {args}")

def default_handler(address, *args):
    print(f"DEFAULT {address}: {args}")


dispatcher = Dispatcher()
dispatcher.map("/filter", filter_handler)
dispatcher.set_default_handler(default_handler)

ip = "127.0.0.1"
port = 1255
client = SimpleUDPClient(ip, port)

async def loop():
    """Example main loop that only runs for 10 iterations before finishing"""
    for i in range(10):
        print(f"Loop {i}")
        await asyncio.sleep(1)


async def init_main():
    server = AsyncIOOSCUDPServer((ip, port), dispatcher, asyncio.get_event_loop())
    transport, protocol = await server.create_serve_endpoint()  # Create datagram endpoint and start serving

    await loop()  # Enter main loop of program

    transport.close()  # Clean up serve endpoint


asyncio.run(init_main())