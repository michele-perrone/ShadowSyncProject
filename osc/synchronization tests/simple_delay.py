from datetime import datetime, timedelta

now = datetime.now()
print(now)
timestamp = datetime.timestamp(now)
print(timestamp)
readable_timestamp = datetime.fromtimestamp(timestamp+10)
print(readable_timestamp)

# EXECUTE SOMETHING 10 SECONDS AFTER START

while True:
    if datetime.timestamp(datetime.now()) >= (timestamp+10):
        print("Printed after 10 seconds.")
        break