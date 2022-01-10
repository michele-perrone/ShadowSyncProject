from pythonosc.dispatcher import Dispatcher
from pythonosc.osc_server import BlockingOSCUDPServer

def print_handler(address, *args):
    address_bits = address[1:].split('/')
    print(f"{address}: {args}, {address_bits}")


def default_handler(address, *args):
    print(f"DEFAULT {address}: {args}")


dispatcher = Dispatcher()
dispatcher.map("/position/*", print_handler)
dispatcher.set_default_handler(default_handler)

ip = "localhost"
port = 5501

server = BlockingOSCUDPServer((ip, port), dispatcher)
server.serve_forever()  # Blocks forever

