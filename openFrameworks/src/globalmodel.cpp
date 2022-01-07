#include "globalmodel.h"

GlobalModel::GlobalModel()
{

}

void GlobalModel::print()
{
    cout << "#########################################################################################################" << endl;
    cout << "#                                      CURRENT GLOBAL MODEL STATE                                       #" << endl;
    cout << "#########################################################################################################" << endl;
    cout << "blend = " << blend << endl;
    cout << "Pose: " << endl;
    pose.print();
    cout << "Other Pose: " << endl;
    other_pose.print();
}
