from pythonosc.udp_client import SimpleUDPClient

ip = "127.0.1.1"
port = 1998

client = SimpleUDPClient(ip, port)  # Create client

client.send_message("/position", 123)   # Send float message
client.send_message("/some/address", [1, 2., "hello"])  # Send message with int, float and string