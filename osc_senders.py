def pose_sender(client, pose):
    #client.send_message("/pose", [pose['left_shoulder'], pose['right_shoulder']])   # Send float message
    client.send_message("/pose", [
        pose['left_shoulder'][0],
        pose['left_shoulder'][1],
        pose['right_shoulder'][0],
        pose['right_shoulder'][1],
        pose['left_hip'][0],
        pose['left_hip'][1],
        pose['right_hip'][0],
        pose['right_hip'][1],
        pose['body'][0],
        pose['body'][1],
        ])