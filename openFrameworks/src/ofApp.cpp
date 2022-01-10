#include "ofApp.h"
#define PL_XZ 200 //the xz plane dimension
#define PL_Y 100  //the y plane dimension
#define OSC_DEBUG 1 // Macro for verbose OSC reception

//--------------------------------------------------------------
void ofApp::setup()
{
    light.setup();
    light.setPosition(200, 100, 400);

    ofEnableDepthTest(); //you can't see through objects (es. wall)

    cam.setPosition(ofVec3f(157, 52, 363)); //camera positioning and heading configuration
    cam.lookAt(ofVec3f(PL_XZ/2, PL_Y/2, 0));
    //!definition of the transformation
    mat.rotate(90, 0, 0, 1);    //with this two commands, we are combining two "high-abstraction level" matrix transformations
    mat.translate(0, 0, 1);    //in just one (otherwise, at a low level you can define manually a 4x4 mat that performs your transformation)

    plane_floor.set(PL_XZ, PL_XZ);
    plane_floor.rotateDeg(270, 1, 0, 0); //by default, it is orthogonal to x axis, centered in origin
    plane_floor.move(PL_XZ/2, 0, PL_XZ/2);

    plane_wall.set(PL_XZ, PL_Y);
    plane_wall.move(PL_XZ/2, PL_Y/2, -1);

    //material def
    floor_material.setDiffuseColor(ofFloatColor::darkGray);
    floor_material.setShininess(0.01);
    wall_material.setDiffuseColor(ofFloatColor::white);
    wall_material.setShininess(0.01);

    //body, test particle system setup
    body.setup(&global_model.pose);

    // OSC
    osc_receiver.setup(PORT); // It is 5501 or 5502

    //Shadow
    //float shadow_origin[] = { 50, 50 };
    //glm::vec2 shadow_vOrigin = glm::make_vec2(shadow_origin);
    //shadow.setup(shadow_vOrigin);
    //shadow.move(50,50);
}

//--------------------------------------------------------------
void ofApp::update()
{
    //OSC
    while (osc_receiver.hasWaitingMessages())
    {
        ofxOscMessage m;
        osc_receiver.getNextMessage(m);
        if (OSC_DEBUG) cout << "Message Received : ";
        handle_address(&m); // Updates the global_model with the latest values arrived by osc
    }
    global_model.pose.print();
    body.move_centroid();
    body.move_junctions();

    //body.updateParticleSystems();
    //shadow.updateParticleSystems();
    //shadow.move(0.2, 0);
}

//--------------------------------------------------------------
void ofApp::draw()
{
    //everything shown in 3D rendering goes btw cam.begin() and cam.end(), same for material
    cam.begin();

    ofBackground(142, 124, 200);
    ofEnableAlphaBlending();

    floor_material.begin();
    plane_floor.draw();
    floor_material.end();

    wall_material.begin();
    plane_wall.draw();
    wall_material.end();

    ofDrawGrid(10, 20, true, true, true, true); //3D grid

    body.draw(); // The body takes now also care of the particle systems!

    //shadow.draw();


    //!application of the transformation
    // ofMultMatrix(mat); //this matrix multiplication allow us to see the circle
    //shadow.draw(); //dummy solution, otherwise not visible since its a 2D entity and gets covered by wall plane
    //ofPopMatrix();

    cam.end();



    //show FPS on screen
    stringstream ss;
    ss << "FPS: " << ofToString(ofGetFrameRate(), 0) << endl << endl;
    ofDrawBitmapString(ss.str().c_str(), 10, 30);

    //UTILITY cam coordinates on screen (just for set it up)
    //glm::vec3 cam_pos = cam.getPosition();
    //stringstream cp;
    //cp << "cam x, y, z coordinates: " << ofToString(cam_pos.x) << ", " << ofToString(cam_pos.y) << ", " << ofToString(cam_pos.z) << endl << endl;
    //ofDrawBitmapString(cp.str().c_str(), 50, 50);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

std::vector<std::string> ofApp::split(string address) {
    std::vector<std::string> address_component = {"00000","00000","00000","00000"};
    std::string delimiter = "/";
    int next = address.find(delimiter);
    if (next == -1 && address.length()>0) return address_component;
    else address = address.substr(1, address.npos);
    for (int i=0; i<4; i++) {
        next = address.find(delimiter);
        if (next != -1) {
            address_component[i] = address.substr(0, next);
//            if (OSC_DEBUG) cout << "Found component " << address_component[i] << " off adderess " << address << endl;
            address = address.substr(next+1, address.npos);
        } else {
            address_component[i] = address;
            address = "";
        }
    }

    return address_component;
}

void ofApp::handle_address(ofxOscMessage * m) {
    string address = m->getAddress();
    if (OSC_DEBUG) cout << "raw address: " << address << endl;
    std::vector<std::string> address_component = split(address);
    string type = address_component.at(0);
    string area = address_component.at(1);
    string component = address_component.at(2);
    //string sub_component = address_component.at(3);

//    if (OSC_DEBUG) cout << "Separated address: " << "-" << type << "-" << area << "-" << component << "-" << sub_component << "-" << endl;

    if(type == "pose"){
        if (OSC_DEBUG) cout << address << endl;
        if(area == "face") {
            if(component=="_completely_detected") {
                //global_model.pose.face_completely_detected = m->getArgAsBool(0);
            } else if (component=="_absent") {
                //global_model.pose.face_absent = m->getArgAsBool(0);
            } else if (component=="_centroid") {
                global_model.pose.face_centroid[0] = m->getArgAsFloat(0);
                global_model.pose.face_centroid[1] = m->getArgAsFloat(1);
                global_model.pose.face_centroid[2] = m->getArgAsFloat(2);
            } else if (component=="nose") {
                global_model.pose.nose[0] = m->getArgAsFloat(0);
                global_model.pose.nose[1] = m->getArgAsFloat(1);
                global_model.pose.nose[2] = m->getArgAsFloat(2);
            } else if (component=="left_eye_inner") {
                global_model.pose.left_eye_inner[0] = m->getArgAsFloat(0);
                global_model.pose.left_eye_inner[1] = m->getArgAsFloat(1);
                global_model.pose.left_eye_inner[2] = m->getArgAsFloat(2);
            } else if (component=="left_eye") {
                global_model.pose.left_eye[0] = m->getArgAsFloat(0);
                global_model.pose.left_eye[1] = m->getArgAsFloat(1);
                global_model.pose.left_eye[2] = m->getArgAsFloat(2);
            } else if (component=="left_eye_outer") {
                global_model.pose.left_eye_outer[0] = m->getArgAsFloat(0);
                global_model.pose.left_eye_outer[1] = m->getArgAsFloat(1);
                global_model.pose.left_eye_outer[2] = m->getArgAsFloat(2);
            } else if (component=="right_eye_inner") {
                global_model.pose.right_eye_inner[0] = m->getArgAsFloat(0);
                global_model.pose.right_eye_inner[1] = m->getArgAsFloat(1);
                global_model.pose.right_eye_inner[2] = m->getArgAsFloat(2);
            } else if (component=="right_eye") {
                global_model.pose.right_eye[0] = m->getArgAsFloat(0);
                global_model.pose.right_eye[1] = m->getArgAsFloat(1);
                global_model.pose.right_eye[2] = m->getArgAsFloat(2);
            } else if (component=="right_eye_outer") {
                global_model.pose.right_eye_outer[0] = m->getArgAsFloat(0);
                global_model.pose.right_eye_outer[1] = m->getArgAsFloat(1);
                global_model.pose.right_eye_outer[2] = m->getArgAsFloat(2);
            } else if (component=="left_ear") {
                global_model.pose.left_ear[0] = m->getArgAsFloat(0);
                global_model.pose.left_ear[1] = m->getArgAsFloat(1);
                global_model.pose.left_ear[2] = m->getArgAsFloat(2);
            } else if (component=="right_ear") {
                global_model.pose.right_ear[0] = m->getArgAsFloat(0);
                global_model.pose.right_ear[1] = m->getArgAsFloat(1);
                global_model.pose.right_ear[2] = m->getArgAsFloat(2);
            } else if (component=="mouth_left") {
                global_model.pose.mouth_left[0] = m->getArgAsFloat(0);
                global_model.pose.mouth_left[1] = m->getArgAsFloat(1);
                global_model.pose.mouth_left[2] = m->getArgAsFloat(2);
            } else if (component=="mouth_right") {
                global_model.pose.mouth_right[0] = m->getArgAsFloat(0);
                global_model.pose.mouth_right[1] = m->getArgAsFloat(1);
                global_model.pose.mouth_right[2] = m->getArgAsFloat(2);
            } else {
                if (OSC_DEBUG) cout << " not recognized" << endl;
            }
        } else if (area == "body") {
            if(component=="_completely_detected") {
                //global_model.pose.body_completely_detected = m->getArgAsBool(0);
            } else if (component=="_absent") {
                //global_model.pose.body_absent = m->getArgAsBool(0);
            } else if (component=="_centroid") {
                global_model.pose.body_centroid[0] = m->getArgAsFloat(0);
                global_model.pose.body_centroid[1] = m->getArgAsFloat(1);
                global_model.pose.body_centroid[2] = m->getArgAsFloat(2);
            } else if (component=="left_shoulder") {
                global_model.pose.left_shoulder[0] = m->getArgAsFloat(0);
                global_model.pose.left_shoulder[1] = m->getArgAsFloat(1);
                global_model.pose.left_shoulder[2] = m->getArgAsFloat(2);
            } else if (component=="right_shoulder") {
                global_model.pose.right_shoulder[0] = m->getArgAsFloat(0);
                global_model.pose.right_shoulder[1] = m->getArgAsFloat(1);
                global_model.pose.right_shoulder[2] = m->getArgAsFloat(2);
            } else if (component=="left_hip") {
                global_model.pose.left_hip[0] = m->getArgAsFloat(0);
                global_model.pose.left_hip[1] = m->getArgAsFloat(1);
                global_model.pose.left_hip[2] = m->getArgAsFloat(2);
            } else if (component=="right_hip") {
                global_model.pose.right_hip[0] = m->getArgAsFloat(0);
                global_model.pose.right_hip[1] = m->getArgAsFloat(1);
                global_model.pose.right_hip[2] = m->getArgAsFloat(2);
            } else {
                if (OSC_DEBUG) cout << "not recognized" << endl;
            }
        } else if (area == "left_arm") {
            if(component=="_completely_detected") {
                //global_model.pose.left_arm_completely_detected = m->getArgAsBool(0);
            } else if (component=="_absent") {
                //global_model.pose.left_arm_absent = m->getArgAsBool(0);
            } else if (component=="_centroid") {
                global_model.pose.left_arm_centroid[0] = m->getArgAsFloat(0);
                global_model.pose.left_arm_centroid[1] = m->getArgAsFloat(1);
                global_model.pose.left_arm_centroid[2] = m->getArgAsFloat(2);
            } else if (component=="elbow") {
                global_model.pose.left_elbow[0] = m->getArgAsFloat(0);
                global_model.pose.left_elbow[1] = m->getArgAsFloat(1);
                global_model.pose.left_elbow[2] = m->getArgAsFloat(2);
            } else if (component=="wrist") {
                global_model.pose.left_wrist[0] = m->getArgAsFloat(0);
                global_model.pose.left_wrist[1] = m->getArgAsFloat(1);
                global_model.pose.left_wrist[2] = m->getArgAsFloat(2);
            } else if (component=="pinky") {
                global_model.pose.left_pinky[0] = m->getArgAsFloat(0);
                global_model.pose.left_pinky[1] = m->getArgAsFloat(1);
                global_model.pose.left_pinky[2] = m->getArgAsFloat(2);
            } else if (component=="index") {
                global_model.pose.left_index[0] = m->getArgAsFloat(0);
                global_model.pose.left_index[1] = m->getArgAsFloat(1);
                global_model.pose.left_index[2] = m->getArgAsFloat(2);
            } else if (component=="thumb") {
                global_model.pose.left_thumb[0] = m->getArgAsFloat(0);
                global_model.pose.left_thumb[1] = m->getArgAsFloat(1);
                global_model.pose.left_thumb[2] = m->getArgAsFloat(2);
            } else {
                if (OSC_DEBUG) cout << "not recognized" << endl;
            }
        } else if (area == "right_arm") {
            if(component=="_completely_detected") {
                //global_model.pose.right_arm_completely_detected = m->getArgAsBool(0);
            } else if (component=="_absent") {
                //global_model.pose.right_arm_absent = m->getArgAsBool(0);
            } else if (component=="_centroid") {
                global_model.pose.right_arm_centroid[0] = m->getArgAsFloat(0);
                global_model.pose.right_arm_centroid[1] = m->getArgAsFloat(1);
                global_model.pose.right_arm_centroid[2] = m->getArgAsFloat(2);
            } else if (component=="elbow") {
                global_model.pose.right_elbow[0] = m->getArgAsFloat(0);
                global_model.pose.right_elbow[1] = m->getArgAsFloat(1);
                global_model.pose.right_elbow[2] = m->getArgAsFloat(2);
            } else if (component=="wrist") {
                global_model.pose.right_wrist[0] = m->getArgAsFloat(0);
                global_model.pose.right_wrist[1] = m->getArgAsFloat(1);
                global_model.pose.right_wrist[2] = m->getArgAsFloat(2);
            } else if (component=="pinky") {
                global_model.pose.right_pinky[0] = m->getArgAsFloat(0);
                global_model.pose.right_pinky[1] = m->getArgAsFloat(1);
                global_model.pose.right_pinky[2] = m->getArgAsFloat(2);
            } else if (component=="index") {
                global_model.pose.right_index[0] = m->getArgAsFloat(0);
                global_model.pose.right_index[1] = m->getArgAsFloat(1);
                global_model.pose.right_index[2] = m->getArgAsFloat(2);
            } else if (component=="thumb") {
                global_model.pose.right_thumb[0] = m->getArgAsFloat(0);
                global_model.pose.right_thumb[1] = m->getArgAsFloat(1);
                global_model.pose.right_thumb[2] = m->getArgAsFloat(2);
            } else {
                if (OSC_DEBUG) cout << "not recognized" << endl;
            }
        } else if (area == "left_leg") {
            if(component=="_completely_detected") {
                //global_model.pose.left_leg_completely_detected = m->getArgAsBool(0);
            } else if (component=="_absent") {
                //global_model.pose.left_leg_absent = m->getArgAsBool(0);
            } else if (component=="_centroid") {
                global_model.pose.left_leg_centroid[0] = m->getArgAsFloat(0);
                global_model.pose.left_leg_centroid[1] = m->getArgAsFloat(1);
                global_model.pose.left_leg_centroid[2] = m->getArgAsFloat(2);
            } else if (component=="knee") {
                global_model.pose.left_knee[0] = m->getArgAsFloat(0);
                global_model.pose.left_knee[1] = m->getArgAsFloat(1);
                global_model.pose.left_knee[2] = m->getArgAsFloat(2);
            } else if (component=="ankle") {
                global_model.pose.left_ankle[0] = m->getArgAsFloat(0);
                global_model.pose.left_ankle[1] = m->getArgAsFloat(1);
                global_model.pose.left_ankle[2] = m->getArgAsFloat(2);
            } else if (component=="heel") {
                global_model.pose.left_heel[0] = m->getArgAsFloat(0);
                global_model.pose.left_heel[1] = m->getArgAsFloat(1);
                global_model.pose.left_heel[2] = m->getArgAsFloat(2);
            } else if (component=="foot_index") {
                global_model.pose.left_foot_index[0] = m->getArgAsFloat(0);
                global_model.pose.left_foot_index[1] = m->getArgAsFloat(1);
                global_model.pose.left_foot_index[2] = m->getArgAsFloat(2);
            } else {
                if (OSC_DEBUG) cout << "not recognized" << endl;
            }
        } else if (area == "right_leg") {
            if(component=="_completely_detected") {
                //global_model.pose.right_leg_completely_detected = m->getArgAsBool(0);
            } else if (component=="_absent") {
                //global_model.pose.right_leg_absent = m->getArgAsBool(0);
            } else if (component=="_centroid") {
                global_model.pose.right_leg_centroid[0] = m->getArgAsFloat(0);
                global_model.pose.right_leg_centroid[1] = m->getArgAsFloat(1);
                global_model.pose.right_leg_centroid[2] = m->getArgAsFloat(2);
            } else if (component=="knee") {
                global_model.pose.right_knee[0] = m->getArgAsFloat(0);
                global_model.pose.right_knee[1] = m->getArgAsFloat(1);
                global_model.pose.right_knee[2] = m->getArgAsFloat(2);
            } else if (component=="ankle") {
                global_model.pose.right_ankle[0] = m->getArgAsFloat(0);
                global_model.pose.right_ankle[1] = m->getArgAsFloat(1);
                global_model.pose.right_ankle[2] = m->getArgAsFloat(2);
            } else if (component=="heel") {
                global_model.pose.right_heel[0] = m->getArgAsFloat(0);
                global_model.pose.right_heel[1] = m->getArgAsFloat(1);
                global_model.pose.right_heel[2] = m->getArgAsFloat(2);
            } else if (component=="foot_index") {
                global_model.pose.right_foot_index[0] = m->getArgAsFloat(0);
                global_model.pose.right_foot_index[1] = m->getArgAsFloat(1);
                global_model.pose.right_foot_index[2] = m->getArgAsFloat(2);
            } else {
                if (OSC_DEBUG) cout << "not recognized" << endl;
            }
        }
    } else if (type == "other_pose") {
        if (OSC_DEBUG) cout << address << endl;
        if(area == "face") {
            if(component=="_completely_detected") {
                global_model.other_pose.face_completely_detected = m->getArgAsBool(0);
            } else if (component=="_absent") {
                global_model.other_pose.face_absent = m->getArgAsBool(0);
            } else if (component=="_centroid") {
                global_model.other_pose.face_centroid[0] = m->getArgAsFloat(0);
                global_model.other_pose.face_centroid[1] = m->getArgAsFloat(1);
                global_model.other_pose.face_centroid[2] = m->getArgAsFloat(2);
            } else if (component=="nose") {
                global_model.other_pose.nose[0] = m->getArgAsFloat(0);
                global_model.other_pose.nose[1] = m->getArgAsFloat(1);
                global_model.other_pose.nose[2] = m->getArgAsFloat(2);
            } else if (component=="left_eye_inner") {
                global_model.other_pose.left_eye_inner[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_eye_inner[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_eye_inner[2] = m->getArgAsFloat(2);
            } else if (component=="left_eye") {
                global_model.other_pose.left_eye[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_eye[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_eye[2] = m->getArgAsFloat(2);
            } else if (component=="left_eye_outer") {
                global_model.other_pose.left_eye_outer[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_eye_outer[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_eye_outer[2] = m->getArgAsFloat(2);
            } else if (component=="right_eye_inner") {
                global_model.other_pose.right_eye_inner[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_eye_inner[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_eye_inner[2] = m->getArgAsFloat(2);
            } else if (component=="right_eye") {
                global_model.other_pose.right_eye[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_eye[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_eye[2] = m->getArgAsFloat(2);
            } else if (component=="right_eye_outer") {
                global_model.other_pose.right_eye_outer[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_eye_outer[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_eye_outer[2] = m->getArgAsFloat(2);
            } else if (component=="left_ear") {
                global_model.other_pose.left_ear[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_ear[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_ear[2] = m->getArgAsFloat(2);
            } else if (component=="right_ear") {
                global_model.other_pose.right_ear[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_ear[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_ear[2] = m->getArgAsFloat(2);
            } else if (component=="mouth_left") {
                global_model.other_pose.mouth_left[0] = m->getArgAsFloat(0);
                global_model.other_pose.mouth_left[1] = m->getArgAsFloat(1);
                global_model.other_pose.mouth_left[2] = m->getArgAsFloat(2);
            } else if (component=="mouth_right") {
                global_model.other_pose.mouth_right[0] = m->getArgAsFloat(0);
                global_model.other_pose.mouth_right[1] = m->getArgAsFloat(1);
                global_model.other_pose.mouth_right[2] = m->getArgAsFloat(2);
            }
        } else if (area == "body") {
            if(component=="_completely_detected") {
                global_model.other_pose.body_completely_detected = m->getArgAsBool(0);
            } else if (component=="_absent") {
                global_model.other_pose.body_absent = m->getArgAsBool(0);
            } else if (component=="_centroid") {
                global_model.other_pose.body_centroid[0] = m->getArgAsFloat(0);
                global_model.other_pose.body_centroid[1] = m->getArgAsFloat(1);
                global_model.other_pose.body_centroid[2] = m->getArgAsFloat(2);
            } else if (component=="left_shoulder") {
                global_model.other_pose.left_shoulder[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_shoulder[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_shoulder[2] = m->getArgAsFloat(2);
            } else if (component=="right_shoulder") {
                global_model.other_pose.right_shoulder[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_shoulder[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_shoulder[2] = m->getArgAsFloat(2);
            } else if (component=="left_hip") {
                global_model.other_pose.left_hip[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_hip[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_hip[2] = m->getArgAsFloat(2);
            } else if (component=="right_hip") {
                global_model.other_pose.right_hip[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_hip[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_hip[2] = m->getArgAsFloat(2);
            }
        } else if (area == "left_arm") {
            if(component=="_completely_detected") {
                global_model.other_pose.left_arm_completely_detected = m->getArgAsBool(0);
            } else if (component=="_absent") {
                global_model.other_pose.left_arm_absent = m->getArgAsBool(0);
            } else if (component=="_centroid") {
                global_model.other_pose.left_arm_centroid[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_arm_centroid[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_arm_centroid[2] = m->getArgAsFloat(2);
            } else if (component=="elbow") {
                global_model.other_pose.left_elbow[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_elbow[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_elbow[2] = m->getArgAsFloat(2);
            } else if (component=="wrist") {
                global_model.other_pose.left_wrist[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_wrist[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_wrist[2] = m->getArgAsFloat(2);
            } else if (component=="pinky") {
                global_model.other_pose.left_pinky[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_pinky[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_pinky[2] = m->getArgAsFloat(2);
            } else if (component=="index") {
                global_model.other_pose.left_index[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_index[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_index[2] = m->getArgAsFloat(2);
            } else if (component=="thumb") {
                global_model.other_pose.left_thumb[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_thumb[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_thumb[2] = m->getArgAsFloat(2);
            }
        } else if (area == "right_arm") {
            if(component=="_completely_detected") {
                global_model.other_pose.right_arm_completely_detected = m->getArgAsBool(0);
            } else if (component=="_absent") {
                global_model.other_pose.right_arm_absent = m->getArgAsBool(0);
            } else if (component=="_centroid") {
                global_model.other_pose.right_arm_centroid[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_arm_centroid[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_arm_centroid[2] = m->getArgAsFloat(2);
            } else if (component=="elbow") {
                global_model.other_pose.right_elbow[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_elbow[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_elbow[2] = m->getArgAsFloat(2);
            } else if (component=="wrist") {
                global_model.other_pose.right_wrist[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_wrist[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_wrist[2] = m->getArgAsFloat(2);
            } else if (component=="pinky") {
                global_model.other_pose.right_pinky[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_pinky[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_pinky[2] = m->getArgAsFloat(2);
            } else if (component=="index") {
                global_model.pose.right_index[0] = m->getArgAsFloat(0);
                global_model.pose.right_index[1] = m->getArgAsFloat(1);
                global_model.pose.right_index[2] = m->getArgAsFloat(2);
            } else if (component=="thumb") {
                global_model.other_pose.right_thumb[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_thumb[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_thumb[2] = m->getArgAsFloat(2);
            } else {

            }
        } else if (area == "left_leg") {
            if(component=="_completely_detected") {
                global_model.other_pose.left_leg_completely_detected = m->getArgAsBool(0);
            } else if (component=="_absent") {
                global_model.other_pose.left_leg_absent = m->getArgAsBool(0);
            } else if (component=="_centroid") {
                global_model.other_pose.left_leg_centroid[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_leg_centroid[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_leg_centroid[2] = m->getArgAsFloat(2);
            } else if (component=="knee") {
                global_model.other_pose.left_knee[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_knee[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_knee[2] = m->getArgAsFloat(2);
            } else if (component=="ankle") {
                global_model.other_pose.left_ankle[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_ankle[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_ankle[2] = m->getArgAsFloat(2);
            } else if (component=="heel") {
                global_model.other_pose.left_heel[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_heel[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_heel[2] = m->getArgAsFloat(2);
            } else if (component=="foot_index") {
                global_model.other_pose.left_foot_index[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_foot_index[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_foot_index[2] = m->getArgAsFloat(2);
            }
        } else if (area == "right_leg") {
            if(component=="_completely_detected") {
                global_model.other_pose.right_leg_completely_detected = m->getArgAsBool(0);
            } else if (component=="_absent") {
                global_model.other_pose.right_leg_absent = m->getArgAsBool(0);
            } else if (component=="_centroid") {
                global_model.other_pose.right_leg_centroid[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_leg_centroid[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_leg_centroid[2] = m->getArgAsFloat(2);
            } else if (component=="knee") {
                global_model.other_pose.right_knee[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_knee[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_knee[2] = m->getArgAsFloat(2);
            } else if (component=="ankle") {
                global_model.other_pose.right_ankle[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_ankle[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_ankle[2] = m->getArgAsFloat(2);
            } else if (component=="heel") {
                global_model.other_pose.right_heel[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_heel[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_heel[2] = m->getArgAsFloat(2);
            } else if (component=="foot_index") {
                global_model.other_pose.right_foot_index[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_foot_index[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_foot_index[2] = m->getArgAsFloat(2);
            }
        }
    } else if (type == "ofxUtil") {
        if (OSC_DEBUG) cout << address << endl;
        if(area == "blend") {
            // blend=0 is just shadow, blend=1 is all other pose
            global_model.blend = m->getArgAsFloat(0);
        }
    } else {
        if (OSC_DEBUG) cout << "not recognized" << endl;
    }

}
