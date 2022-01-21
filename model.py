from datetime import datetime

class Model:

    def elapsed_time(self):
        return datetime.now() - self.start_time

    def __init__(self):
        self.start_time = datetime.now()
        self.computer_online = [0, 0, 0]
        self.ack = [0, 0, 0]
        self.blend = 0
        self.has_started = 0
        self.latest_correlation_value = [0, 0, 0]
        self.START = 0
        
