#ifndef POSE_H
#define POSE_H

#include <map>
#include <string>


class Pose
{
public:
    Pose();


    bool face_completely_detected = true;
    bool body_completely_detected = true;
    bool left_arm_completely_detected = true;
    bool right_arm_completely_detected = true;
    bool left_leg_completely_detected = true;
    bool right_leg_completely_detected = true;


    bool face_absent = true;
    bool body_absent = true;
    bool left_arm_absent = true;
    bool right_arm_absent = true;
    bool left_leg_absent = true;
    bool right_leg_absent = true;


    float face_centroid[3] = {0.0, 0.0, 0.0};
    float body_centroid[3] = {0.0, 0.0, 0.0};
    float left_arm_centroid[3] = {0.0, 0.0, 0.0};
    float right_arm_centroid[3] = {0.0, 0.0, 0.0};
    float left_leg_centroid[3] = {0.0, 0.0, 0.0};
    float right_leg_centroid[3] = {0.0, 0.0, 0.0};


    float nose[3] = {0.0, 0.0, 0.0};
    float left_eye_inner[3] = {0.0, 0.0, 0.0};
    float left_eye[3] = {0.0, 0.0, 0.0};
    float left_eye_outer[3] = {0.0, 0.0, 0.0};
    float right_eye_inner[3] = {0.0, 0.0, 0.0};
    float right_eye[3] = {0.0, 0.0, 0.0};
    float right_eye_outer[3] = {0.0, 0.0, 0.0};
    float left_ear[3] = {0.0, 0.0, 0.0};
    float right_ear[3] = {0.0, 0.0, 0.0};
    float mouth_left[3] = {0.0, 0.0, 0.0};
    float mouth_right[3] = {0.0, 0.0, 0.0};


    float left_shoulder[3] = {0.0, 0.0, 0.0};
    float right_shoulder[3] = {0.0, 0.0, 0.0};
    float left_hip[3] = {0.0, 0.0, 0.0};
    float right_hip[3] = {0.0, 0.0, 0.0};


    float left_pinky[3] = {0.0, 0.0, 0.0};
    float left_index[3] = {0.0, 0.0, 0.0};
    float left_thumb[3] = {0.0, 0.0, 0.0};
    float left_elbow[3] = {0.0, 0.0, 0.0};
    float left_wrist[3] = {0.0, 0.0, 0.0};


    float right_pinky[3] = {0.0, 0.0, 0.0};
    float right_index[3] = {0.0, 0.0, 0.0};
    float right_thumb[3] = {0.0, 0.0, 0.0};
    float right_elbow[3] = {0.0, 0.0, 0.0};
    float right_wrist[3] = {0.0, 0.0, 0.0};


    float left_heel[3] = {0.0, 0.0, 0.0};
    float left_foot_index[3] = {0.0, 0.0, 0.0};
    float left_knee[3] = {0.0, 0.0, 0.0};
    float left_ankle[3] = {0.0, 0.0, 0.0};


    float right_heel[3] = {0.0, 0.0, 0.0};
    float right_foot_index[3] = {0.0, 0.0, 0.0};
    float right_knee[3] = {0.0, 0.0, 0.0};
    float right_ankle[3] = {0.0, 0.0, 0.0};
};

#endif // POSE_H