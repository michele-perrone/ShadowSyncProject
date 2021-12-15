import cv2 #image processing via openCv
import mediapipe as mp #pose estimation via mediapipe

mpDraw = mp.solutions.drawing_utils
mpPose = mp.solutions.pose
pose = mpPose.Pose() #with default params for detection and tracking tolerance (until detection confidence is high enough, it keeps tracking)
# Create a VideoCapture object and read from input file
cap = cv2.VideoCapture(r".\ComputerVisionPython\Videos\robot_dance.mp4") #PC webcam
#cap = cv2.VideoCapture(0)
# Check if it is opened successfully
if (cap.isOpened()== False):
  print("Error opening video file")

# Read until video is completed
while(True):
	# Capture frame-by-frame
  ret, img = cap.read()
  if ret == True:
    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB) #conversion of the acquired img in RGB scale
    results = pose.process(imgRGB) #pose detection
   # print(results.pose_landmarks)
    if results.pose_landmarks: #==True, landmarks detected
      mpDraw.draw_landmarks(img, results.pose_landmarks, mpPose.POSE_CONNECTIONS) #draw the landmarks
    for id, lm in enumerate(results.pose_landmarks.landmark):
        h, w, c = img.shape
        cx, cy = int(lm.x*w), int(lm.y*h) #pixel coords in the frame of the landmarks
        cv2.circle(img, (cx,cy), 5, (155,155,0)) #superimpose a circle to the landmarks''' 
    left_shoulder = [results.pose_landmarks.landmark[mpPose.PoseLandmark.LEFT_SHOULDER].x*w , results.pose_landmarks.landmark[mpPose.PoseLandmark.LEFT_SHOULDER].y*h]
    right_shoulder = [results.pose_landmarks.landmark[mpPose.PoseLandmark.RIGHT_SHOULDER].x*w , results.pose_landmarks.landmark[mpPose.PoseLandmark.RIGHT_SHOULDER].y*h]
    left_hip = [results.pose_landmarks.landmark[mpPose.PoseLandmark.LEFT_HIP].x*w , results.pose_landmarks.landmark[mpPose.PoseLandmark.LEFT_HIP].y*h]
    right_hip = [results.pose_landmarks.landmark[mpPose.PoseLandmark.RIGHT_HIP].x*w , results.pose_landmarks.landmark[mpPose.PoseLandmark.RIGHT_HIP].y*h]
    centroid = [int((left_shoulder[0] + right_shoulder[0] + left_hip[0] + right_hip[0])/4) , int((left_shoulder[1] + right_shoulder[1] + left_hip[1] + right_hip[1])/4) ]
    cv2.circle(img, (centroid[0], centroid[1]), 8, (155,155,222), cv2.FILLED) #superimpose a circle to the landmarks''' 
    if cv2.waitKey(1) & 0xFF == ord('q'):
      break
    cv2.imshow('Image', img)
  else:
    break

  # When everything done, release
  # the video capture object
cap.release()

  # Closes all the frames
cv2.destroyAllWindows()

