#ifndef GLOBALMODEL_H
#define GLOBALMODEL_H


#include <map>
#include <string>

#include "pose.h"

class GlobalModel
{
public:
    GlobalModel();
    float blend=0.0; // 0 -> Full pose | 1 -> Full other_pose
    Pose pose;
    Pose other_pose;
};

#endif // GLOBALMODEL_H
