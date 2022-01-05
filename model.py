from datetime import datetime

class Model:

    def elapsed_time(self):
        return datetime.now() - self.start_time

    def __init__(self):
        self.start_time = datetime.now()
        self.computer_online = [0, 0, 0]
        self.ack = [0, 0, 0]
        self.pose1 = {
            "face": {
                "_completely_detected": "1",
                "_centroid": "000", 
                "nose": "000",
                "left_eye_inner": "000",
                "left_eye": "000",
                "left_eye_outer": "000",
                "right_eye_inner": "000",
                "right_eye": "000",
                "right_eye_outer": "000",
                "left_ear": "000",
                "right_ear": "000",
                "mouth_left": "000",
                "mouth_right": "000"
            },
            "body": {
                "_completely_detected": "1",
                "_centroid": "000", 
                "left_shoulder": "000",
                "right_shoulder": "000",
                "left_hip": "000",
                "right_hip": "000"
            },
            "left_arm": {
                "_completely_detected": "1",
                "_centroid": "000", 
                "left_elbow": "000",
                "left_wrist": "000",
                "left_hand": {
                    "_completely_detected": "1",
                    "_centroid": "000", 
                    "left_pinky": "000",
                    "left_index": "000",
                    "left_thumb": "000"
                }
            },
            "right_arm": {
                "_completely_detected": "1",
                "_centroid": "000", 
                "right_elbow": "000",
                "right_wrist": "000",
                "right_hand": {
                    "_completely_detected": "1",
                    "_centroid": "000", 
                    "right_pinky": "000",
                    "right_index": "000",
                    "right_thumb": "000"
                }
            },
            "left_leg": {
                "_completely_detected": "1",
                "_centroid": "000", 
                "left_knee": "000",
                "left_ankle": "000",
                "left_foot": {
                    "_completely_detected": "1",
                    "_centroid": "000", 
                    "left_heel": "000",
                    "left_foot_index": "000"
                }
            },
            "right_leg": {
                "_completely_detected": "1",
                "_centroid": "000", 
                "right_knee": "000",
                "right_ankle": "000",
                "right_foot": {
                    "_completely_detected": "1",
                    "_centroid": "000", 
                    "right_heel": "000",
                    "right_foot_index": "000"
                }
            }
        }

        self.pose2 =  {
            "face": {
                "_completely_detected": "1",
                "_centroid": "000", 
                "nose": "000",
                "left_eye_inner": "000",
                "left_eye": "000",
                "left_eye_outer": "000",
                "right_eye_inner": "000",
                "right_eye": "000",
                "right_eye_outer": "000",
                "left_ear": "000",
                "right_ear": "000",
                "mouth_left": "000",
                "mouth_right": "000"
            },
            "body": {
                "_completely_detected": "1",
                "_centroid": "000", 
                "left_shoulder": "000",
                "right_shoulder": "000",
                "left_hip": "000",
                "right_hip": "000"
            },
            "left_arm": {
                "_completely_detected": "1",
                "_centroid": "000", 
                "left_elbow": "000",
                "left_wrist": "000",
                "left_hand": {
                    "_completely_detected": "1",
                    "_centroid": "000", 
                    "left_pinky": "000",
                    "left_index": "000",
                    "left_thumb": "000"
                }
            },
            "right_arm": {
                "_completely_detected": "1",
                "_centroid": "000", 
                "right_elbow": "000",
                "right_wrist": "000",
                "right_hand": {
                    "_completely_detected": "1",
                    "_centroid": "000", 
                    "right_pinky": "000",
                    "right_index": "000",
                    "right_thumb": "000"
                }
            },
            "left_leg": {
                "_completely_detected": "1",
                "_centroid": "000", 
                "left_knee": "000",
                "left_ankle": "000",
                "left_foot": {
                    "_completely_detected": "1",
                    "_centroid": "000", 
                    "left_heel": "000",
                    "left_foot_index": "000"
                }
            },
            "right_leg": {
                "_completely_detected": "1",
                "_centroid": "000", 
                "right_knee": "000",
                "right_ankle": "000",
                "right_foot": {
                    "_completely_detected": "1",
                    "_centroid": "000", 
                    "right_heel": "000",
                    "right_foot_index": "000"
                }
            }
        }