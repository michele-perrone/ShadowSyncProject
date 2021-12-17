############################################################################################################
#THIS NEEDS TO BE THE MAIN OF OUR WHOLE PROJECT IN ORDER TO PROPERLY MAKE COMMUNICATION WORK
############################################################################################################

# 1 IMPORT OSC SERVER LIBRARIES HERE
from pythonosc.osc_server import AsyncIOOSCUDPServer
from pythonosc.dispatcher import Dispatcher
from pythonosc.udp_client import SimpleUDPClient
import asyncio
# 2 OTHER LIBRARIES HERE

# 3 CREATE SENDER FUNCTIONS HERE -> THEY ARE ONLY NEEDED TO FORMAT COMPLEX OR MULTIPLE SENDS

def specific_sender(a, b):
    client.send_message("/position", a)   # Send float message
    client.send_message("/position", b)

def ss2(a, b, c):
    client.send_message("/some/address", [a, 2., c, "hello"])  # Send message with int, float and string

# 4 CREATE HANDLER FUNCTIONS HERE -> THEY BETTER ONLY MODIFY THE GLOBAL MODEL WITH THE RECEIVED PARAMETERS
def filter_handler(address, *args):
    # global_model.parameter...
    print(f"{address}: {args}")

def default_handler(address, *args):
    print(f"DEFAULT {address}: {args}")


# 5 CREATE MODEL OBJECT HERE
# global_model = ...

# 6 CREATE DISPATCHER AND MAPPING OF OSC MESSAGES TO CORRESPONDING HANDLERS HERE. PLEASE KEEP DEFAULT HANDLER
dispatcher = Dispatcher()
dispatcher.map("/filter", filter_handler)
dispatcher.set_default_handler(default_handler)

# 7 DEFINE IP, PORT AND CLIENT FOR OSC COMMUNICATION HERE
ip = "127.0.1.1"
port = 1998
client = SimpleUDPClient(ip, port)  # Create client

# 8 PUT CODE FOR THE REST OF THE PROJECT IN app_main()
async def app_main():
    """Example main loop that only runs for 10 iterations before finishing"""
    for i in range(10):
        print(f"Loop {i}")
        # 9 PLEASE USE MY OSC CLIENT WRAPPER THIS WAY
        # subroutine_call(..., client, ...) in order to use the same client and at most define other functions into the subroutine
        # Simple sends
        client.send_message('/position', 1)
        # Complex sends
        specific_sender(22, 333)
        await asyncio.sleep(1) # While sleeping the control is passed back to the osc message check function.

# 10 WRITE THE INITIALIZER MAIN FOR THE OSC SERVER AND RUN IT
async def init_main():
    server = AsyncIOOSCUDPServer((ip, port), dispatcher, asyncio.get_event_loop())
    transport, protocol = await server.create_serve_endpoint()  # Create datagram endpoint and start serving

    await app_main()  # Enter main loop of program -> MUST CONTAIN A asyncio.sleep(something) IN ORDER TO WORK

    transport.close()  # Clean up serve endpoint


asyncio.run(init_main())