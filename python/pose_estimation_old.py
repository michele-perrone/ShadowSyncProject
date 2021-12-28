
'''import cv2  # image processing via openCv
import mediapipe as mp  # pose estimation via mediapipe

import os


def init_pose_estimation():
    # Create a VideoCapture object and read from input file
    #print(os.path.join(os.path.curdir, "Videos", "robot_dance.mp4"))
    cap = cv2.VideoCapture(os.path.join(os.path.curdir, "../Videos", "robot_dance.mp4"))  # PC webcam
    #cap = cv2.VideoCapture(0)
    mpDraw = mp.solutions.drawing_utils
    mpPose = mp.solutions.pose
    pose_cv = mpPose.Pose()  # with default params for detection and tracking tolerance (until detection confidence is high enough, it keeps tracking)
    # Check if it is opened successfully
    pose = {
        'left_shoulder': "000",
        'right_shoulder': "000",
        'left_hip': "000",
        'right_hip': "000",
        'body': "000"
    }
    if not cap.isOpened():
        print("Error opening video file")
    return cap, mpDraw, mpPose, pose_cv, pose


def get_body_position(img, mpDraw, mpPose, pose_cv, pose):
    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)  # conversion of the acquired img in RGB scale
    results = pose_cv.process(imgRGB)  # pose detection# print(results.pose_landmarks)

    if results.pose_landmarks:  # ==True, landmarks detected
        mpDraw.draw_landmarks(img, results.pose_landmarks, mpPose.POSE_CONNECTIONS)  # draw the landmarks
        for id, lm in enumerate(results.pose_landmarks.landmark):
            h, w, c = img.shape
            cx, cy = int(lm.x * w), int(lm.y * h)  # pixel coords in the frame of the landmarks
            cv2.circle(img, (cx, cy), 5, (155, 155, 0))  # superimpose a circle to the landmarks
        left_shoulder = [results.pose_landmarks.landmark[mpPose.PoseLandmark.LEFT_SHOULDER].x,
                         results.pose_landmarks.landmark[mpPose.PoseLandmark.LEFT_SHOULDER].y,
                         results.pose_landmarks.landmark[mpPose.PoseLandmark.LEFT_SHOULDER].z]
        right_shoulder = [results.pose_landmarks.landmark[mpPose.PoseLandmark.RIGHT_SHOULDER].x,
                          results.pose_landmarks.landmark[mpPose.PoseLandmark.RIGHT_SHOULDER].y,
                          results.pose_landmarks.landmark[mpPose.PoseLandmark.RIGHT_SHOULDER].z]
        left_hip = [results.pose_landmarks.landmark[mpPose.PoseLandmark.LEFT_HIP].x,
                    results.pose_landmarks.landmark[mpPose.PoseLandmark.LEFT_HIP].y,
                    results.pose_landmarks.landmark[mpPose.PoseLandmark.LEFT_HIP].z]
        right_hip = [results.pose_landmarks.landmark[mpPose.PoseLandmark.RIGHT_HIP].x,
                     results.pose_landmarks.landmark[mpPose.PoseLandmark.RIGHT_HIP].y,
                     results.pose_landmarks.landmark[mpPose.PoseLandmark.RIGHT_HIP].z]
        centroid = [(left_shoulder[0] + right_shoulder[0] + left_hip[0] + right_hip[0]) / 4,
                    (left_shoulder[1] + right_shoulder[1] + left_hip[1] + right_hip[1]) / 4]

        pose = {
            'left_shoulder': left_shoulder,
            'right_shoulder': right_shoulder,
            'left_hip': left_hip,
            'right_hip': right_hip,
            'body': centroid,
        }

        cv2.circle(img, (int(centroid[0] * w), int(centroid[1] * h)), 8, (155, 155, 222),
                   cv2.FILLED)  # superimpose a circle to the landmarks

    if cv2.waitKey(1) & 0xFF == ord('q'):
        return

    return img, pose


def destroy_pose_estimator():
    # When everything done, release
    # the video capture object
    cap.release()

    # Closes all the frames
    cv2.destroyAllWindows()
'''