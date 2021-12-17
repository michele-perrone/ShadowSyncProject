def pose_sender(client, pose):
    client.send_message("/pose", pose['body'])   # Send float message