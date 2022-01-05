def pose_sender(client, pose):
    #client.send_message("/pose", [pose['left_shoulder'], pose['right_shoulder']])   # Send float message
    client.send_message("/pose", [
        pose['body']['left_shoulder'][0],
        pose['body']['left_shoulder'][1],
        pose['body']['right_shoulder'][0],
        pose['body']['right_shoulder'][1],
        pose['body']['left_hip'][0],
        pose['body']['left_hip'][1],
        pose['body']['right_hip'][0],
        pose['body']['right_hip'][1],
        ])