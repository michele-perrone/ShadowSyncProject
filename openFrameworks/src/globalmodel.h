#ifndef GLOBALMODEL_H
#define GLOBALMODEL_H

//using namespace std;
#include <map>
#include <string>
#include <iostream>
using namespace std;

#include <pose.h>

class GlobalModel
{
public:
    GlobalModel();
    float blend=0.0; // 0 -> Full pose | 1 -> Full other_pose
    Pose pose;
    Pose other_pose;
    void print();
};

#endif // GLOBALMODEL_H
