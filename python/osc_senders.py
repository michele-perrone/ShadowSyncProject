def pose_sender(client, pose):
    for i in pose:
        for j in pose[i]:
            address = "/pose/" + i + "/" + j #example: address = /pose/face/_centroid[ ... ]
            coordinates = []
            for index in range(len(pose[i][j])):
                coordinates.append(pose[i][j][index])
            print(coordinates)
            client.send_message(address, [
                coordinates
            ])


