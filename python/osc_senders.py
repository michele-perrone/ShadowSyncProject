def pose_sender(client, pose):
    for i in pose:
        for j in pose[i]:
            address = "/pose/" + i + "/" + j #example: address = /pose/face/_centroid[ ... ]
            client.send_message(address, [
                pose[i][j][0],
                pose[i][j][1],
                pose[i][j][2]
            ])
