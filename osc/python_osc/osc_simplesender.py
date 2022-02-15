from pythonosc.udp_client import SimpleUDPClient, SimpleTCPClient

ip = "localhost"
port = 1255

client = SimpleUDPClient(ip, port)  # Create client
client_tcp = SimpleTCPClient(ip, port)

address = ['fhu', 'fhduh', 'fhfhfhfhf']

client.send_message("/position", 123)   # Send float message
client.send_message("/some" + '/' + address[0] + '/' + address[2], [1, 2., "hello"])  # Send message with int, float and string