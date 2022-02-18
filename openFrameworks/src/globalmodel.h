#ifndef GLOBALMODEL_H
#define GLOBALMODEL_H

#include <map>
#include <string>
#include <iostream>
#include "pose.h"

#define XY_PLANE 0
#define YZ_PLANE 1

class GlobalModel
{
public:
    float blend=0.0; // 0 -> Full pose | 1 -> Full other_pose
    Pose pose;
    Pose other_pose;

    GlobalModel();
    GlobalModel(Pose pose, Pose other_pose);

    /**
     * @brief Prints all the pose junctions for both poses
     */
    void print();

    /**
     * @brief Calculates the (dis)similarity between
     * the two poses in the XY plane
     * @param Minimum value of the similarity factor
     * @param Maximum value of the similarity factor
     * @return A similarity factor in the range [min, max]
     */
    float get_pose_similarity_xy(float min, float max);

    /**
     * @brief Calculates the (dis)similarity between
     * the two poses in the YZ plane
     * @param Minimum value of the similarity factor
     * @param Maximum value of the similarity factor
     * @return A similarity factor in the range [min, max]
     */
    float get_pose_similarity_yz(float min, float max);

private:
    float get_pose_similarity(float min, float max, int PLANE);
};

#endif // GLOBALMODEL_H
