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
    Pose pose;
    Pose other_pose;

    // 0 --> tutorial non ancora iniziato
    // "pose" copiata dentro "other_pose"
    // 1 --> tutorial fase 1
    // "other_pose" è bloccata all'ultima "pose"
    // 2 --> tutorial fase 2
    // "other_pose" è bloccata all'ultima "pose"
    // al momento in cui è stata impostata la fase 2
    // >3 --> no tutorial
    float installation_phase;
    string ip_server;
    int i_am;

    /**
     * @brief blend decides to which degree "other_pose"
     * should NOT mimick "pose". The range is [0,1].
     * 0 --> "other_pose" is a copy "pose".
     *       You (pose) fully control you shadow (other_pose)
     * 1 --> "other_pose" and "pose" are fully distinct
     *       The other person's shadow (other_pose) is you shadow
     */
    float blend = 0.0f; // By default, your shadow follows YOU (pose)

    GlobalModel();
    GlobalModel(Pose pose, Pose other_pose);

    /**
     * @brief Prints all the pose junctions for both poses
     */
    void print();

    /**
     * @brief Calculates the (dis)similarity between
     * the two poses in the XY plane
     * @param min Minimum value of the similarity factor
     * @param max Maximum value of the similarity factor
     * @return A similarity factor in the range [min, max],
     * where min --> poses are completely different,
     * and   max --> poses are completely identical
     */
    float get_pose_similarity_xy(float min, float max);

    /**
     * @brief Calculates the (dis)similarity between
     * the two poses in the YZ plane
     * @param min Minimum value of the similarity factor
     * @param max Maximum value of the similarity factor
     * @return A similarity factor in the range [min, max],
     * where min --> poses are completely different,
     * and   max --> poses are completely identical
     */
    float get_pose_similarity_yz(float min, float max);

    /**
     * @brief Detects whether "pose" and "other_pose"
     * are the same
     * @param min Minimum value of the similarity factor
     * @param max Maximum value of the similarity factor
     * @param threshold to decide if the poses are the same
     * @return
     */
    bool detect_same_pose(float min, float max, float threshold);

    /**
     * @brief Updates "pose" and "other_pose"
     * according to the current "blend_factor"
     */
    void update_blending();

private:
    float get_pose_similarity(float min, float max, int PLANE);
};

#endif // GLOBALMODEL_H
