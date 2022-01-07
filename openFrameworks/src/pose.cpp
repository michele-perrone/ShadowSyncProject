#include "pose.h"

Pose::Pose()
{

}

void Pose::print()
{
    cout << "CURRENT POSE STATUS" << endl;
    if (!face_absent) {
        cout << "  > face " << (face_completely_detected ? "[completely_detected] @ " : "") << "[" << face_centroid[0] << ", " << face_centroid[1] << ", " << face_centroid[2] << "]" << endl;
    } else {
        cout << "  > face [absent]" << endl;
    }
    if (!body_absent) {
        cout << "  > body " << (body_completely_detected ? "[completely_detected] @ " : "") << "[" << body_centroid[0] << ", " << body_centroid[1] << ", " << body_centroid[2] << "]" << endl;
    } else {
        cout << "  > body [absent]" << endl;
    }
    if (!left_arm_absent) {
        cout << "  > left_arm " << (left_arm_completely_detected ? "[completely_detected] @ " : "") << "[" << left_arm_centroid[0] << ", " << left_arm_centroid[1] << ", " << left_arm_centroid[2] << "]" << endl;
    } else {
        cout << "  > left_arm [absent]" << endl;
    }
    if (!right_arm_absent) {
        cout << "  > right_arm " << (right_arm_completely_detected ? "[completely_detected] @ " : "") << "[" << right_arm_centroid[0] << ", " << right_arm_centroid[1] << ", " << right_arm_centroid[2] << "]" << endl;
    } else {
        cout << "  > right_arm [absent]" << endl;
    }
    if (!left_leg_absent) {
        cout << "  > left_leg " << (left_leg_completely_detected ? "[completely_detected] @ " : "") << "[" << left_leg_centroid[0] << ", " << left_leg_centroid[1] << ", " << left_leg_centroid[2] << "]" << endl;
    } else {
        cout << "  > left_leg [absent]" << endl;
    }
    if (!right_leg_absent) {
        cout << "  > right_leg " << (right_leg_completely_detected ? "[completely_detected] @ " : "") << "[" << right_leg_centroid[0] << ", " << right_leg_centroid[1] << ", " << right_leg_centroid[2] << "]" << endl;
    } else {
        cout << "  > right_leg [absent]" << endl;
    }
}
