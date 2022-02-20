#include "globalmodel.h"
#include <ofVec2f.h>
#include <ofVec3f.h>

GlobalModel::GlobalModel()
{}

GlobalModel::GlobalModel(Pose pose, Pose other_pose)
{
    this->pose = pose;
    this->other_pose = other_pose;
}

void GlobalModel::print()
{
    std::cout << "####################################################################################" << std::endl;
    std::cout << "#                          CURRENT GLOBAL MODEL STATE                              #" << std::endl;
    std::cout << "####################################################################################" << std::endl;
    std::cout << "blend = " << blend << std::endl;
    std::cout << "Pose: " << std::endl;
    pose.print();
    std::cout << "Other Pose: " << std::endl;
    other_pose.print();
}

float GlobalModel::get_blend()
{
    return this->blend;
}

void GlobalModel::set_blend(float blend)
{
    // Check the range. Should be [0,1]
    if(blend < 0 || blend > 1)
    {
        std::cout << "The received value of blend (" << blend << ") is out of range!" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        this->blend = blend;

    // 0 --> "other_pose" is a copy "pose".
    // In other words: you (pose) are FULLY controlling your own shadow (other_pose).

    // 1 --> "other_pose" and "pose" are fully distinct.
    // In other words: you (pose) are NOT controlling you shadow (other_pose),
    // because the other person's shadow is your shadow instead.

    // The variable "blend" can be seen as a weighted-average coefficient
    // between "pose" and "other_pose".
    enum {X=0, Y=1, Z=2};
    for(int i = X; i <= Z; i++)
    {
        other_pose.face_centroid[i] = (blend)*other_pose.face_centroid[i] + (1-blend)*pose.face_centroid[i];
        other_pose.body_centroid[i] = (blend)*other_pose.body_centroid[i] + (1-blend)*pose.body_centroid[i];
        other_pose.left_arm_centroid[i] = (blend)*other_pose.left_arm_centroid[i] + (1-blend)*pose.left_arm_centroid[i];
        other_pose.right_arm_centroid[i] = (blend)*other_pose.right_arm_centroid[i] + (1-blend)*pose.right_arm_centroid[i];
        other_pose.left_leg_centroid[i] = (blend)*other_pose.left_leg_centroid[i] + (1-blend)*pose.left_leg_centroid[i];
        other_pose.right_leg_centroid[i] = (blend)*other_pose.right_leg_centroid[i] + (1-blend)*pose.right_leg_centroid[i];

        other_pose.nose[i] = (blend)*other_pose.nose[i] + (1-blend)*pose.nose[i];
        other_pose.left_eye_inner[i] = (blend)*other_pose.left_eye_inner[i] + (1-blend)*pose.left_eye_inner[i];
        other_pose.left_eye_outer[i] = (blend)*other_pose.left_eye_outer[i] + (1-blend)*pose.left_eye_outer[i];
        other_pose.right_eye_inner[i] = (blend)*other_pose.right_eye_inner[i] + (1-blend)*pose.right_eye_inner[i];
        other_pose.right_eye[i] = (blend)*other_pose.right_eye[i] + (1-blend)*pose.right_eye[i];
        other_pose.right_eye_outer[i] = (blend)*other_pose.right_eye_outer[i] + (1-blend)*pose.right_eye_outer[i];
        other_pose.left_ear[i] = (blend)*other_pose.left_ear[i] + (1-blend)*pose.left_ear[i];
        other_pose.right_ear[i] = (blend)*other_pose.right_ear[i] + (1-blend)*pose.right_ear[i];
        other_pose.mouth_left[i] = (blend)*other_pose.mouth_left[i] + (1-blend)*pose.mouth_left[i];
        other_pose.mouth_right[i] = (blend)*other_pose.mouth_right[i] + (1-blend)*pose.mouth_right[i];

        other_pose.left_shoulder[i] = (blend)*other_pose.left_shoulder[i] + (1-blend)*pose.left_shoulder[i];
        other_pose.right_shoulder[i] = (blend)*other_pose.right_shoulder[i] + (1-blend)*pose.right_shoulder[i];
        other_pose.left_hip[i] = (blend)*other_pose.left_hip[i] + (1-blend)*pose.left_hip[i];
        other_pose.right_hip[i] = (blend)*other_pose.right_hip[i] + (1-blend)*pose.right_hip[i];

        other_pose.left_pinky[i] = (blend)*other_pose.left_pinky[i] + (1-blend)*pose.left_pinky[i];
        other_pose.left_index[i] = (blend)*other_pose.left_index[i] + (1-blend)*pose.left_index[i];
        other_pose.left_thumb[i] = (blend)*other_pose.left_thumb[i] + (1-blend)*pose.left_thumb[i];
        other_pose.left_elbow[i] = (blend)*other_pose.left_elbow[i] + (1-blend)*pose.left_elbow[i];
        other_pose.left_wrist[i] = (blend)*other_pose.left_wrist[i] + (1-blend)*pose.left_wrist[i];

        other_pose.right_pinky[i] = (blend)*other_pose.right_pinky[i] + (1-blend)*pose.right_pinky[i];
        other_pose.right_index[i] = (blend)*other_pose.right_index[i] + (1-blend)*pose.right_index[i];
        other_pose.right_thumb[i] = (blend)*other_pose.right_thumb[i] + (1-blend)*pose.right_thumb[i];
        other_pose.right_elbow[i] = (blend)*other_pose.right_elbow[i] + (1-blend)*pose.right_elbow[i];
        other_pose.right_wrist[i] = (blend)*other_pose.right_wrist[i] + (1-blend)*pose.right_wrist[i];

        other_pose.left_heel[i] = (blend)*other_pose.left_heel[i] + (1-blend)*pose.left_heel[i];
        other_pose.left_foot_index[i] = (blend)*other_pose.left_foot_index[i] + (1-blend)*pose.left_foot_index[i];
        other_pose.left_knee[i] = (blend)*other_pose.left_knee[i] + (1-blend)*pose.left_knee[i];
        other_pose.left_ankle[i] = (blend)*other_pose.left_ankle[i] + (1-blend)*pose.left_ankle[i];

        other_pose.right_heel[i] = (blend)*other_pose.right_heel[i] + (1-blend)*pose.right_heel[i];
        other_pose.right_foot_index[i] = (blend)*other_pose.right_foot_index[i] + (1-blend)*pose.right_foot_index[i];
        other_pose.right_knee[i] = (blend)*other_pose.right_knee[i] + (1-blend)*pose.right_knee[i];
        other_pose.right_ankle[i] = (blend)*other_pose.right_ankle[i] + (1-blend)*pose.right_ankle[i];
    }
}

float GlobalModel::get_pose_similarity_xy(float min, float max)
{
    return this->get_pose_similarity(min, max, XY_PLANE);
}

float GlobalModel::get_pose_similarity_yz(float min, float max)
{
    return this->get_pose_similarity(min, max, YZ_PLANE);
}

float GlobalModel::get_pose_similarity(float min, float max, int PLANE)
{
    // Coordinates of the centroid.
    ofVec3f pose_centroid_coordinates(pose.body_centroid[0],
                                      pose.body_centroid[1],
                                      pose.body_centroid[2]);

    // We will subtract them from the vectors of the
    // left arm, face, right arm, right leg, and left leg.
    // This is equivalent to moving the centroid into the origin (0,0).
    ofVec2f pose_left_arm;
    ofVec2f pose_face;
    ofVec2f pose_right_arm;
    ofVec2f pose_right_leg;
    ofVec2f pose_left_leg;

    if(PLANE == XY_PLANE)
    {
        pose_left_arm = ofVec2f(pose.left_arm_centroid[0]-pose_centroid_coordinates.x,
                              pose.left_arm_centroid[1]-pose_centroid_coordinates.y);
        pose_face = ofVec2f(pose.face_centroid[0]-pose_centroid_coordinates.x,
                          pose.face_centroid[1]-pose_centroid_coordinates.y);
        pose_right_arm = ofVec2f(pose.right_arm_centroid[0]-pose_centroid_coordinates.x,
                               pose.right_arm_centroid[1]-pose_centroid_coordinates.y);
        pose_right_leg = ofVec2f(pose.right_leg_centroid[0]-pose_centroid_coordinates.x,
                               pose.right_leg_centroid[1]-pose_centroid_coordinates.y);
        pose_left_leg = ofVec2f(pose.left_leg_centroid[0]-pose_centroid_coordinates.x,
                              pose.left_leg_centroid[1]-pose_centroid_coordinates.y);
    }
    else if(PLANE == YZ_PLANE)
    {
        pose_left_arm = ofVec2f(pose.left_arm_centroid[1]-pose_centroid_coordinates.y,
                              pose.left_arm_centroid[2]-pose_centroid_coordinates.z);
        pose_face = ofVec2f(pose.face_centroid[1]-pose_centroid_coordinates.y,
                          pose.face_centroid[2]-pose_centroid_coordinates.z);
        pose_right_arm = ofVec2f(pose.right_arm_centroid[1]-pose_centroid_coordinates.y,
                               pose.right_arm_centroid[2]-pose_centroid_coordinates.z);
        pose_right_leg = ofVec2f(pose.right_leg_centroid[1]-pose_centroid_coordinates.y,
                               pose.right_leg_centroid[2]-pose_centroid_coordinates.z);
        pose_left_leg = ofVec2f(pose.left_leg_centroid[1]-pose_centroid_coordinates.y,
                              pose.left_leg_centroid[2]-pose_centroid_coordinates.z);
    }
    else
    {
        std::cout << "You specified an incorrect plane" << std::endl;
        exit(EXIT_FAILURE);
    }

    // After doing this, we can safely calculate the angles between:
    // - left arm and face (alpha)
    // - face and right arm (beta)
    // - right arm and right leg (gamma)
    // - right leg and left leg (delta)
    // - left leg and left arm (theta)
    float pose_alpha = pose_left_arm.angle(pose_face);
    float pose_beta = pose_face.angle(pose_right_arm);
    float pose_gamma = pose_right_arm.angle(pose_right_leg);
    float pose_delta = pose_right_leg.angle(pose_left_leg);
    float pose_theta = pose_left_leg.angle(pose_left_arm);

    // We do the same thing for other_pose
    ofVec2f other_pose_centroid_coordinates(pose.body_centroid[0], pose.body_centroid[1]);

    ofVec2f other_pose_left_arm(pose.left_arm_centroid[0]-other_pose_centroid_coordinates.x,
                          pose.left_arm_centroid[1]-other_pose_centroid_coordinates.y);
    ofVec2f other_pose_face(pose.face_centroid[0]-other_pose_centroid_coordinates.x,
                      pose.face_centroid[1]-other_pose_centroid_coordinates.y);
    ofVec2f other_pose_right_arm(pose.right_arm_centroid[0]-other_pose_centroid_coordinates.x,
                           pose.right_arm_centroid[1]-other_pose_centroid_coordinates.y);
    ofVec2f other_pose_right_leg(pose.right_leg_centroid[0]-other_pose_centroid_coordinates.x,
                           pose.right_leg_centroid[1]-other_pose_centroid_coordinates.y);
    ofVec2f other_pose_left_leg(pose.left_leg_centroid[0]-other_pose_centroid_coordinates.x,
                          pose.left_leg_centroid[1]-other_pose_centroid_coordinates.y);

    float other_pose_alpha = other_pose_left_arm.angle(other_pose_face);
    float other_pose_beta = other_pose_face.angle(other_pose_right_arm);
    float other_pose_gamma = other_pose_right_arm.angle(other_pose_right_leg);
    float other_pose_delta = other_pose_right_leg.angle(other_pose_left_leg);
    float other_pose_theta = other_pose_left_leg.angle(other_pose_left_arm);

    // We then compare all the angles between pose and other_pose and spit out the magic number.
    // When the pose is identical, we expect the sum of the differences between the angles of
    // pose and other_pose to be zero.
    float alpha_diff = abs(pose_alpha-other_pose_alpha);
    float beta_diff = abs(pose_beta-other_pose_beta);
    float gamma_diff = abs(pose_gamma-other_pose_gamma);
    float delta_diff = abs(pose_delta-other_pose_delta);
    float theta_diff = abs(pose_theta-other_pose_theta);

    // It is quite obvious that the maximum absolute difference between two angles
    // can be 180 degrees, since ofVec2f::angle return values in the range [-180, +180].
    // So the sum of the five differences can be in the range [0, 5*180] --> [0, 900].
    float total_diff = alpha_diff + beta_diff + gamma_diff + delta_diff + theta_diff;
    float desired_range = 0; // If we want something else instead of [0, 900]
    if(min < max)
    {
        desired_range = max-min;
    }
    else
    {
        std::swap(max, min);
        std::cout << "You entered a maximum value that's smaller than the minimum value..."
                     "Sure ya know what'ya doin'?" << std::endl;
    }

    // To rescale into the desired range [min, max], we simply divide by "desired_range"
    // and then subtract "min".
    float super_magic_number_that_tells_you_how_similar_the_poses_are = (total_diff/desired_range)-min;

    return super_magic_number_that_tells_you_how_similar_the_poses_are;
}
