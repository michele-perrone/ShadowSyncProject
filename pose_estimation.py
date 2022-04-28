import cv2  # image processing via openCv
import mediapipe as mp  # pose estimation via mediapipe
import json
import os
import platform



def init_pose_estimation():

    """
    This function activate the user's webcam and set all landmarks points form JSON.
    """

    os_name = platform.system()

    """
    if "Windows" in os_name:
        cap = cv2.VideoCapture(os.path.join(os.path.curdir, "Videos", "cpac-video-test-2.mov"))
    else:
        cap = cv2.VideoCapture("./cpac-video-test-2.mov")
    """

    # obtain video/webcam
    cap = cv2.VideoCapture(0)

    mpDraw = mp.solutions.drawing_utils
    mpPose = mp.solutions.pose
    pose_cv = mpPose.Pose()  # with default params for detection and tracking tolerance (until detection confidence is high enough, it keeps tracking)

    if "Windows" in os_name:
        data = open('python\data\landmark.json')
    else:
        data = open('landmark.json')

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

    """
    This function calculate all coordinates with for each landmark.
    """

    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)  # conversion of the acquired img in RGB scale
    results = pose_cv.process(imgRGB)  # pose detection

    if results.pose_landmarks:  # ==True, landmarks detected
        mpDraw.draw_landmarks(img, results.pose_landmarks, mpPose.POSE_CONNECTIONS)  # draw the landmarks
        for id, lm in enumerate(results.pose_landmarks.landmark):
            h, w, c = img.shape
            cx, cy = int(lm.x * w), int(lm.y * h)  # pixel coords in the frame of the landmarks
            cv2.circle(img, (cx, cy), 5, (155, 155, 0))  # superimpose a circle to the landmarks

        for i in pose: # for all element in pose
            old_pose = pose # save the old pose
            centroid = [0, 0, 0] # set 0 all centroid
            length = len(pose[i]) - 3
            for j in pose[i]: # for each group
                if not j.startswith('_'): # if it is a landmark from mediapipe
                    upperJ = j.upper() # transform it in uppercase
                    pose[i][j] = [results.pose_landmarks.landmark[mpPose.PoseLandmark[upperJ]].x,
                                  results.pose_landmarks.landmark[mpPose.PoseLandmark[upperJ]].y,
                                  results.pose_landmarks.landmark[mpPose.PoseLandmark[upperJ]].z] # extract all coordinates

                    if pose[i][j] == [0, 0, 0]: # if position in not calculated
                        pose[i][j] = old_pose[i][j] # take the previous position

                    centroid = [x + y for x, y in zip(centroid, pose[i][j])] # calculate centroid for each landmark

            centroid = [x / length for x in centroid] # calculate a global centroid for each group

            pose[i]['_centroid'] = centroid # save it

    if cv2.waitKey(10) & 0xFF == ord('q'):
        return

    return img, pose


def destroy_pose_estimator(cap):
    # When everything done, release
    # the video capture object
    cap.release()

    # Closes all the frames
    cv2.destroyAllWindows()
