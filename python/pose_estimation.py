import cv2  # image processing via openCv
import mediapipe as mp  # pose estimation via mediapipe
import json
import os
import platform


def init_pose_estimation():
    # array of all point
    # poseLandmarksArray = ["nose", "left_eye_inner", "left_eye", "left_eye_outer", "right_eye_inner", "right_eye", "right_eye_outer", "left_ear", "right_ear", "mouth_left", "mouth_right", "left_shoulder", "right_shoulder", "left_elbow", "right_elbow", "left_wrist", "right_wrist", "left_pinky", "right_pinky", "left_index", "right_index", "left_thumb", "right_thumb", "left_hip", "right_hip", "left_knee", "right_knee", "left_ankle", "right_ankle", "left_heel", "right_heel", "left_foot_index", "right_foot_index"]
    os_name = platform.system()
    # obtain video/webcam
    if "Windows" in os_name:
       cap = cv2.VideoCapture(os.path.join(os.path.curdir, "Videos", "cpac-video-test-2.mov"))
    else:
        cap = cv2.VideoCapture("./cpac-video-test-2.mov")
    # cap = cv2.VideoCapture(0)

    mpDraw = mp.solutions.drawing_utils
    mpPose = mp.solutions.pose
    pose_cv = mpPose.Pose()  # with default params for detection and tracking tolerance (until detection confidence is high enough, it keeps tracking)

    
    if "Windows" in os_name:
        data = open('python\data\landmark.json')
    else:
        data = open('landmark.json')

    # path = os.path.join(os.getcwd(), 'python', 'data', 'landmark.json')
    # data = open(path)
    pose = json.load(data)
    poseLandmarksArray = [x.upper() for x in pose]

    print(poseLandmarksArray)
    print(" ")
    print(pose)
    print(type(pose))

    if not cap.isOpened():
        print("Error opening video file")
    return cap, mpDraw, mpPose, pose_cv, pose, poseLandmarksArray


def get_body_position(img, mpDraw, mpPose, pose_cv, pose, poseLandmarksArray):
    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)  # conversion of the acquired img in RGB scale
    results = pose_cv.process(imgRGB)  # pose detection# print(results.pose_landmarks)

    if results.pose_landmarks:  # ==True, landmarks detected
        mpDraw.draw_landmarks(img, results.pose_landmarks, mpPose.POSE_CONNECTIONS)  # draw the landmarks
        for id, lm in enumerate(results.pose_landmarks.landmark):
            h, w, c = img.shape
            cx, cy = int(lm.x * w), int(lm.y * h)  # pixel coords in the frame of the landmarks
            cv2.circle(img, (cx, cy), 5, (155, 155, 0))  # superimpose a circle to the landmarks

        for i in pose:
            centroid = [0, 0, 0]
            length = len(pose[i]) - 3
            for j in pose[i]:
                if not j.startswith('_'):
                    upperJ = j.upper()
                    pose[i][j] = [results.pose_landmarks.landmark[mpPose.PoseLandmark[upperJ]].x,
                                  results.pose_landmarks.landmark[mpPose.PoseLandmark[upperJ]].y,
                                  results.pose_landmarks.landmark[mpPose.PoseLandmark[upperJ]].z]

                    if pose[i][j] == [0, 0, 0]:
                        break
                    centroid = [x + y for x, y in zip(centroid, pose[i][j])]

            centroid = [x / length for x in centroid]

            pose[i]['_centroid'] = centroid
            # print(i, pose[i]['_centroid'])

    if cv2.waitKey(10) & 0xFF == ord('q'):
        return

    return img, pose


def destroy_pose_estimator(cap):
    # When everything done, release
    # the video capture object
    cap.release()

    # Closes all the frames
    cv2.destroyAllWindows()
