#include "ofApp.h"
#define PL_XZ 200 //the xz plane dimension
#define PL_Y 100  //the y plane dimension
#define OSC_DEBUG 0 // Macro for verbose OSC reception


#include <fstream>
#include <string>

//--------------------------------------------------------------
void ofApp::setup()
{
    background.load("futuristic_interior_background.jpg");

    light.setup();
    light.setPosition(200, 100, 400);

    ofEnableDepthTest(); //you can't see through objects (e.g. wall)

    cam.setPosition(ofVec3f(157, 52, 263)); //camera positioning and heading configuration
    cam.lookAt(ofVec3f(PL_XZ/2, PL_Y/2, 0));
    //!definition of the transformation
    mat.rotate(90, 0, 0, 1);    //with this two commands, we are combining two "high-abstraction level" matrix transformations
    mat.translate(0, 0, 1);    //in just one (otherwise, at a low level you can define manually a 4x4 mat that performs your transformation)

    plane_floor.set(PL_XZ, PL_XZ); //original setup
    plane_floor.rotateDeg(270, 1, 0, 0); //by default, it is orthogonal to x axis, centered in origin
    plane_floor.move(PL_XZ/2, 0, PL_XZ/2-1); //avoid misallignment with wall

    plane_wall.set(PL_XZ, PL_Y);
    plane_wall.move(PL_XZ/2, PL_Y/2, -1);

    // Material definition
    floor_material.setDiffuseColor(ofFloatColor::darkGray);
    floor_material.setShininess(0.01);
    wall_material.setDiffuseColor(ofFloatColor::white);
    wall_material.setShininess(0.01);

    // Body and Shadow setup
    body.setup(&global_model.pose);
    shadow.setup(&global_model.other_pose);

    // OSC - Receiver and Sender
    osc_receiver.setup(PORT_RECEIVER_BASE+global_model.i_am); // It is 5501 or 5502
    osc_sender.setup("192.168.1.31", PORT_SENDER); // 1255

    //INITAL STATE POSE -while not receiving osc messages yet-
    ifstream file("initialpose.json");
    Json::Value initPoseJson;
    Json::Reader reader;
    reader.parse(file, initPoseJson);
    //std::cout << "json data: " << initPoseJson << std::endl;
    dummy_body.setup(&init_model.pose);
    defineInitModel(initPoseJson);
    
    global_model.pose = init_model.pose; //when osc are received the body is generated where the dummy body was

    this->delay_timer = 0;
}

//--------------------------------------------------------------
void ofApp::update()
{
    if (osc_receiver.hasWaitingMessages() == false)
    {
        dummy_body.moveJunctions();
        dummy_body.moveCentroids();
        dummy_body.updateParticleSystems();
        dummy_body.updateSysMaxVals(bd_ms_dummy, bd_mf_dummy);
    }
    // OSC - Receive the messages
    while (osc_receiver.hasWaitingMessages())
    {
        ofxOscMessage m;
        osc_receiver.getNextMessage(m);
        if (OSC_DEBUG) cout << "Message Received : ";
        handle_address(&m); // Updates the global_model with the latest values arrived by osc
        //std::cout << "receiving osc msg" << std::endl;
        draw_dummy = false;
    }

    // OSC - Send the messages
    ofxOscMessage message_to_send;
    if(global_model.installation_phase > 0)
    {
        message_to_send.clear();
        message_to_send.setAddress("/ofxUtil/correlation");
        message_to_send.addIntArg(global_model.i_am);
        float correlation = global_model.get_pose_similarity_xy(0, 1);
        std::cout << "CORRELATION: " << correlation << std::endl;
        message_to_send.addFloatArg(correlation);
        osc_sender.sendMessage(message_to_send);
    }

    // This is where we send the OSC message when the "pose" and "other_pose"
    // reach a decent level of similarity.
    if(this->delay_timer >= 90)
    {
        if( (global_model.installation_phase == 1 || global_model.installation_phase == 2)
                && global_model.detect_same_pose(0, 1, 0.8))
        {
            message_to_send.clear();
            message_to_send.setAddress("/ofxUtil/tutorialComplete");
            message_to_send.addIntArg(global_model.i_am);
            message_to_send.addIntArg(global_model.installation_phase);
            osc_sender.sendMessage(message_to_send);
            std::cout << "******************************" << std::endl;
            std::cout << "******************************" << std::endl;
            std::cout << "TUTORIAL COMPLETE MESSAGE SENT" << std::endl;
            std::cout << "******************************" << std::endl;
            std::cout << "******************************" << std::endl;
        }
    }

    //3D Body
    //body.getCentroidsPositions(); //not updated centroids positions
    body.savePastCentroidsPositions();
    body.moveJunctions();
    body.moveCentroids();
    body.updateParticleSystems();
    body.updateSysMaxVals(bd_ms, bd_mf);

    
    //2D Shadow
    //shadow.getCentroidsPositions(); //not updated centroids position
    shadow.moveJunctions();
    shadow.moveCentroids();
    shadow.updateParticleSystems();
    shadow.updateSysMaxVals(sh_ms, sh_mf);

    // Update the blending
    // CAUTION: updating the blending is not something we want to do
    //          at every update cycle, especially during installation
    //          phases 1 and 2!
    if(global_model.continuous_blending)
        global_model.update_blending();

    //check if the pose is moving
    //std::cout << "Is face centroid moving? " << shadow.isCentroidMoving(0) << std::endl;
    // 
    //global_model.pose.isInFrontOfCam();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    // The comparison is done when ofApp::delay_timer reaches a certain value.
    if(this->delay_timer < 90)
    {
        this->delay_timer++;
    }

    //changing background
    ofEnableLighting();
    ofDisableDepthTest();
    ///draw the backdrop
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);
    ofSetColor(255); //will be black if this is not included 
    background.getTexture().bind();
    backdrop.resizeToTexture(background.getTexture());
    backdrop.set(ofGetWidth(), ofGetHeight());
    backdrop.draw();
    background.getTexture().unbind();
    ofPopMatrix();
    ofEnableDepthTest();
    //end of changing background
    
    //everything shown in 3D rendering goes btw cam.begin() and cam.end(), same for material
    cam.begin();

    //ofBackground(142, 124, 200);
    ofEnableAlphaBlending();

    ofPushStyle();
    ofSetColor(255, 255, 255, 127);
    //floor_material.begin();
    plane_floor.draw();
    //floor_material.end();

    ofSetColor(125, 125, 125, 127);
    //wall_material.begin();
    plane_wall.draw();
    //wall_material.end();

    ofPopStyle();

    //ofDrawGrid(10, 20, true, true, true, true); //3D grid
    if(draw_dummy)
        dummy_body.draw();

    body.draw();
    shadow.draw();

    //!application of the transformation
    // ofMultMatrix(mat); //this matrix multiplication allow us to see the circle
    //shadow.draw(); //dummy solution, otherwise not visible since its a 2D entity and gets covered by wall plane
    //ofPopMatrix();

    cam.end();



    //show FPS on screen
    stringstream ss;
    ss << "FPS: " << ofToString(ofGetFrameRate(), 0) << endl << endl;
    //ofDrawBitmapString(ss.str().c_str(), 10, 30);

    //UTILITY cam coordinates on screen (just for set it up)
    //glm::vec3 cam_pos = cam.getPosition();
    //stringstream cp;
    //cp << "cam x, y, z coordinates: " << ofToString(cam_pos.x) << ", " << ofToString(cam_pos.y) << ", " << ofToString(cam_pos.z) << endl << endl;
    //ofDrawBitmapString(cp.str().c_str(), 50, 50);

    ss << "BODY MAX_SPEED: " << bd_ms << ", MAX_FORCE: " << bd_mf << endl;
    ss << "SHADOW MAX_SPEED: "<< sh_ms << ", MAX_FORCE: "<< sh_mf<< endl;
    ofDrawBitmapString(ss.str().c_str(), 10, 30);


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key)
    {
    //shadow max force, speed
    case 'a':
        sh_mf -= 0.001;
        break;
    case 'd':
        sh_mf += 0.001;
        break;
    case 's':
        sh_ms -= 0.001;
        break;
    case 'w':
        sh_ms += 0.001;
        break;

    case 'A':
        sh_mf -= 0.05;
        break;
    case 'D':
        sh_mf += 0.05;
        break;
    case 'S':
        sh_ms -= 0.05;
        break;
    case 'W':
        sh_ms += 0.05;
        break;
    //body max force, speed
    case 'j':
        bd_mf -= 0.001;
        break;
    case 'l':
        bd_mf += 0.001;
        break;
    case 'k':
        bd_ms -= 0.001;
        break;
    case 'i':
        bd_ms += 0.001;
        break;

    case 'J':
        bd_mf -= 0.05;
        break;
    case 'L':
        bd_mf += 0.05;
        break;
    case 'K':
        bd_ms -= 0.05;
        break;
    case 'I':
        bd_ms += 0.05;
        break;
    }

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

    if(type == "pose")
    {
        if (OSC_DEBUG) cout << address << endl;
        if(area == "face") {
            if(component=="_completely_detected") {
                global_model.pose.face_completely_detected = m->getArgAsBool(0);
            } else if (component=="_absent") {
                global_model.pose.face_absent = m->getArgAsBool(0);
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
                global_model.pose.body_completely_detected = m->getArgAsBool(0);
            } else if (component=="_absent") {
                global_model.pose.body_absent = m->getArgAsBool(0);
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
                global_model.pose.left_arm_completely_detected = m->getArgAsBool(0);
            } else if (component=="_absent") {
                global_model.pose.left_arm_absent = m->getArgAsBool(0);
            } else if (component=="_centroid") {
                global_model.pose.left_arm_centroid[0] = m->getArgAsFloat(0);
                global_model.pose.left_arm_centroid[1] = m->getArgAsFloat(1);
                global_model.pose.left_arm_centroid[2] = m->getArgAsFloat(2);
            } else if (component=="left_elbow") {
                global_model.pose.left_elbow[0] = m->getArgAsFloat(0);
                global_model.pose.left_elbow[1] = m->getArgAsFloat(1);
                global_model.pose.left_elbow[2] = m->getArgAsFloat(2);
            } else if (component=="left_wrist") {
                global_model.pose.left_wrist[0] = m->getArgAsFloat(0);
                global_model.pose.left_wrist[1] = m->getArgAsFloat(1);
                global_model.pose.left_wrist[2] = m->getArgAsFloat(2);
            } else if (component=="left_pinky") {
                global_model.pose.left_pinky[0] = m->getArgAsFloat(0);
                global_model.pose.left_pinky[1] = m->getArgAsFloat(1);
                global_model.pose.left_pinky[2] = m->getArgAsFloat(2);
            } else if (component=="left_index") {
                global_model.pose.left_index[0] = m->getArgAsFloat(0);
                global_model.pose.left_index[1] = m->getArgAsFloat(1);
                global_model.pose.left_index[2] = m->getArgAsFloat(2);
            } else if (component=="left_thumb") {
                global_model.pose.left_thumb[0] = m->getArgAsFloat(0);
                global_model.pose.left_thumb[1] = m->getArgAsFloat(1);
                global_model.pose.left_thumb[2] = m->getArgAsFloat(2);
            } else {
                if (OSC_DEBUG) cout << "not recognized" << endl;
            }
        } else if (area == "right_arm") {
            if(component=="_completely_detected") {
                global_model.pose.right_arm_completely_detected = m->getArgAsBool(0);
            } else if (component=="_absent") {
                global_model.pose.right_arm_absent = m->getArgAsBool(0);
            } else if (component=="_centroid") {
                global_model.pose.right_arm_centroid[0] = m->getArgAsFloat(0);
                global_model.pose.right_arm_centroid[1] = m->getArgAsFloat(1);
                global_model.pose.right_arm_centroid[2] = m->getArgAsFloat(2);
            } else if (component=="right_elbow") {
                global_model.pose.right_elbow[0] = m->getArgAsFloat(0);
                global_model.pose.right_elbow[1] = m->getArgAsFloat(1);
                global_model.pose.right_elbow[2] = m->getArgAsFloat(2);
            } else if (component=="right_wrist") {
                global_model.pose.right_wrist[0] = m->getArgAsFloat(0);
                global_model.pose.right_wrist[1] = m->getArgAsFloat(1);
                global_model.pose.right_wrist[2] = m->getArgAsFloat(2);
            } else if (component=="right_pinky") {
                global_model.pose.right_pinky[0] = m->getArgAsFloat(0);
                global_model.pose.right_pinky[1] = m->getArgAsFloat(1);
                global_model.pose.right_pinky[2] = m->getArgAsFloat(2);
            } else if (component=="right_index") {
                global_model.pose.right_index[0] = m->getArgAsFloat(0);
                global_model.pose.right_index[1] = m->getArgAsFloat(1);
                global_model.pose.right_index[2] = m->getArgAsFloat(2);
            } else if (component=="right_thumb") {
                global_model.pose.right_thumb[0] = m->getArgAsFloat(0);
                global_model.pose.right_thumb[1] = m->getArgAsFloat(1);
                global_model.pose.right_thumb[2] = m->getArgAsFloat(2);
            } else {
                if (OSC_DEBUG) cout << "not recognized" << endl;
            }
        } else if (area == "left_leg") {
            if(component=="_completely_detected") {
                global_model.pose.left_leg_completely_detected = m->getArgAsBool(0);
            } else if (component=="_absent") {
                global_model.pose.left_leg_absent = m->getArgAsBool(0);
            } else if (component=="_centroid") {
                global_model.pose.left_leg_centroid[0] = m->getArgAsFloat(0);
                global_model.pose.left_leg_centroid[1] = m->getArgAsFloat(1);
                global_model.pose.left_leg_centroid[2] = m->getArgAsFloat(2);
            } else if (component=="left_knee") {
                global_model.pose.left_knee[0] = m->getArgAsFloat(0);
                global_model.pose.left_knee[1] = m->getArgAsFloat(1);
                global_model.pose.left_knee[2] = m->getArgAsFloat(2);
            } else if (component=="left_ankle") {
                global_model.pose.left_ankle[0] = m->getArgAsFloat(0);
                global_model.pose.left_ankle[1] = m->getArgAsFloat(1);
                global_model.pose.left_ankle[2] = m->getArgAsFloat(2);
            } else if (component=="left_heel") {
                global_model.pose.left_heel[0] = m->getArgAsFloat(0);
                global_model.pose.left_heel[1] = m->getArgAsFloat(1);
                global_model.pose.left_heel[2] = m->getArgAsFloat(2);
            } else if (component=="left_foot_index") {
                global_model.pose.left_foot_index[0] = m->getArgAsFloat(0);
                global_model.pose.left_foot_index[1] = m->getArgAsFloat(1);
                global_model.pose.left_foot_index[2] = m->getArgAsFloat(2);
            } else {
                if (OSC_DEBUG) cout << "not recognized" << endl;
            }
        } else if (area == "right_leg") {
            if(component=="_completely_detected") {
                global_model.pose.right_leg_completely_detected = m->getArgAsBool(0);
            } else if (component=="_absent") {
                global_model.pose.right_leg_absent = m->getArgAsBool(0);
            } else if (component=="_centroid") {
                global_model.pose.right_leg_centroid[0] = m->getArgAsFloat(0);
                global_model.pose.right_leg_centroid[1] = m->getArgAsFloat(1);
                global_model.pose.right_leg_centroid[2] = m->getArgAsFloat(2);
            } else if (component=="right_knee") {
                global_model.pose.right_knee[0] = m->getArgAsFloat(0);
                global_model.pose.right_knee[1] = m->getArgAsFloat(1);
                global_model.pose.right_knee[2] = m->getArgAsFloat(2);
            } else if (component=="right_ankle") {
                global_model.pose.right_ankle[0] = m->getArgAsFloat(0);
                global_model.pose.right_ankle[1] = m->getArgAsFloat(1);
                global_model.pose.right_ankle[2] = m->getArgAsFloat(2);
            } else if (component=="right_heel") {
                global_model.pose.right_heel[0] = m->getArgAsFloat(0);
                global_model.pose.right_heel[1] = m->getArgAsFloat(1);
                global_model.pose.right_heel[2] = m->getArgAsFloat(2);
            } else if (component=="right_foot_index") {
                global_model.pose.right_foot_index[0] = m->getArgAsFloat(0);
                global_model.pose.right_foot_index[1] = m->getArgAsFloat(1);
                global_model.pose.right_foot_index[2] = m->getArgAsFloat(2);
            } else {
                if (OSC_DEBUG) cout << "not recognized" << endl;
            }
        }
    }

    // We don't want to receive "other_pose" during the tutorial (installation phases 0, 1, and 2)
    else if (type == "other_pose" && global_model.installation_phase >= 3)
    {
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
            } else if (component=="left_elbow") {
                global_model.other_pose.left_elbow[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_elbow[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_elbow[2] = m->getArgAsFloat(2);
            } else if (component=="left_wrist") {
                global_model.other_pose.left_wrist[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_wrist[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_wrist[2] = m->getArgAsFloat(2);
            } else if (component=="left_pinky") {
                global_model.other_pose.left_pinky[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_pinky[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_pinky[2] = m->getArgAsFloat(2);
            } else if (component=="left_index") {
                global_model.other_pose.left_index[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_index[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_index[2] = m->getArgAsFloat(2);
            } else if (component=="left_thumb") {
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
            } else if (component=="right_elbow") {
                global_model.other_pose.right_elbow[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_elbow[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_elbow[2] = m->getArgAsFloat(2);
            } else if (component=="right_wrist") {
                global_model.other_pose.right_wrist[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_wrist[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_wrist[2] = m->getArgAsFloat(2);
            } else if (component=="right_pinky") {
                global_model.other_pose.right_pinky[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_pinky[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_pinky[2] = m->getArgAsFloat(2);
            } else if (component=="right_index") {
                global_model.pose.right_index[0] = m->getArgAsFloat(0);
                global_model.pose.right_index[1] = m->getArgAsFloat(1);
                global_model.pose.right_index[2] = m->getArgAsFloat(2);
            } else if (component=="right_thumb") {
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
            } else if (component=="left_knee") {
                global_model.other_pose.left_knee[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_knee[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_knee[2] = m->getArgAsFloat(2);
            } else if (component=="left_ankle") {
                global_model.other_pose.left_ankle[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_ankle[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_ankle[2] = m->getArgAsFloat(2);
            } else if (component=="left_heel") {
                global_model.other_pose.left_heel[0] = m->getArgAsFloat(0);
                global_model.other_pose.left_heel[1] = m->getArgAsFloat(1);
                global_model.other_pose.left_heel[2] = m->getArgAsFloat(2);
            } else if (component=="left_foot_index") {
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
            } else if (component=="right_knee") {
                global_model.other_pose.right_knee[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_knee[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_knee[2] = m->getArgAsFloat(2);
            } else if (component=="right_ankle") {
                global_model.other_pose.right_ankle[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_ankle[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_ankle[2] = m->getArgAsFloat(2);
            } else if (component=="right_heel") {
                global_model.other_pose.right_heel[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_heel[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_heel[2] = m->getArgAsFloat(2);
            } else if (component=="right_foot_index") {
                global_model.other_pose.right_foot_index[0] = m->getArgAsFloat(0);
                global_model.other_pose.right_foot_index[1] = m->getArgAsFloat(1);
                global_model.other_pose.right_foot_index[2] = m->getArgAsFloat(2);
            }
        }
    }

    else if (type == "ofxUtil")
    {
        if (OSC_DEBUG) cout << address << endl;
        // We don't want to receive "blend" during the tutorial (installation phases 0, 1, and 2)
        if(area == "blend" && global_model.installation_phase >= 3)
        {
            // blend=0 is just shadow, blend=1 is all other pose
            global_model.blend = m->getArgAsFloat(0);
        }
        else if (area == "startTutorial")
        {
            cout << area << endl;
            // NOTE: the "installation_phase" value is received "una tantum" (i.e. not continuously)
            //       when the installation switches from one phase to another.
            global_model.installation_phase = m->getArgAsFloat(0);

            if (global_model.installation_phase == 0)
            {
                cout << "Phase 0: tutorial hasn't started yet" << endl;

                // "other_pose" must mimick "pose", CONTINUOUSLY.
                // This can be achieved by setting "blend" to zero,
                // and by updating constantly the blending.
                // This way, we guarantee that "pose" is continuously
                // copied into "other_pose".
                global_model.blend = 0;
                global_model.continuous_blending = true;

                // (Remember that in this phase,
                //  the "blend" parameter is not received)
            }
            else if (global_model.installation_phase == 1)
            {
                cout << "Phase 1: tutorial pt. 1 started" << endl;

                // In this phase, "other_pose" is not being received.
                // We copy the CURRENT "pose" into "other_pose". This is done ONCE.
                // Then, "other_pose" remains LOCKED.
                // We remain in this phase until "pose" and "other pose" reach enough similarity.

                // Last received "pose" is copied into "other_pose"
                global_model.blend = 0;
                global_model.update_blending();

                // By setting "continuous_blending" to FALSE, we ensure that the above step
                // is performed only ONCE.
                // (GlobalModel::update_blending won't be called in ofApp::update)
                global_model.continuous_blending = false;

                // "pose" is compared with "other_pose" and the result is sent via OSC.
                // This is done at every cycle ofApp::update.
                // Reset the delay timer to zero
                this->delay_timer = 0;
            }
            else if (global_model.installation_phase == 2)
            {
                cout << "Phase 2: tutorial pt. 2 started" << endl;

                // In this phase, "other_pose" is not being received.
                // We copy the CURRENT "pose" into "other_pose". This is done ONCE.
                // Then, "other_pose" remains LOCKED.
                // We remain in this phase until "pose" and "other pose" reach enough similarity.

                // Last received "pose" is copied into "other_pose"
                global_model.blend = 0;
                global_model.update_blending();

                // By setting "continuous_blending" to FALSE, we ensure that the above step
                // is performed only ONCE.
                // (GlobalModel::update_blending won't be called in ofApp::update)
                global_model.continuous_blending = false;

                // "pose" is compared with "other_pose" and the result is sent via OSC.
                // This is done at every cycle ofApp::update.
                // Reset the delay timer to zero
                this->delay_timer = 0;
            }
        }
        else if (area == "startForReal")
        {
            cout << area << endl;

            // By setting "installation_phase" to 3, we start receiving and updating:
            // -- "other_pose"
            // -- "blend"
            global_model.installation_phase = 3;
            global_model.continuous_blending = true;
        }
    }
    else
    {
        if (OSC_DEBUG) cout << "not recognized" << endl;
    }

}

void ofApp::defineInitModel(Json::Value initPoseJson)
{
    string main_components[6] = { "face", "body", "left_arm", "right_arm", "left_leg", "right_leg" };
    string type = "pose";
    for (int i = 0; i < 6; i++)
    {
        string area = main_components[i];

        Json::Value sub_components = initPoseJson[main_components[i]];

        if (area == "face")
        {
            init_model.pose.face_centroid[0] = (sub_components["_centroid"])[0].asFloat();
            init_model.pose.face_centroid[1] = (sub_components["_centroid"])[1].asFloat();
            init_model.pose.face_centroid[2] = (sub_components["_centroid"])[2].asFloat();

            init_model.pose.nose[0] = (sub_components["nose"])[0].asFloat();
            init_model.pose.nose[1] = (sub_components["nose"])[1].asFloat();
            init_model.pose.nose[2] = (sub_components["nose"])[2].asFloat();

            init_model.pose.left_eye_inner[0] = (sub_components["left_eye_inner"])[0].asFloat();
            init_model.pose.left_eye_inner[1] = (sub_components["left_eye_inner"])[1].asFloat();
            init_model.pose.left_eye_inner[2] = (sub_components["left_eye_inner"])[2].asFloat();

            init_model.pose.left_eye[0] = (sub_components["left_eye"])[0].asFloat();
            init_model.pose.left_eye[1] = (sub_components["left_eye"])[1].asFloat();
            init_model.pose.left_eye[2] = (sub_components["left_eye"])[2].asFloat();

            init_model.pose.left_eye_outer[0] = (sub_components["left_eye_outer"])[0].asFloat();
            init_model.pose.left_eye_outer[1] = (sub_components["left_eye_outer"])[1].asFloat();
            init_model.pose.left_eye_outer[2] = (sub_components["left_eye_outer"])[2].asFloat();

            init_model.pose.right_eye_inner[0] = (sub_components["right_eye_inner"])[0].asFloat();
            init_model.pose.right_eye_inner[1] = (sub_components["right_eye_inner"])[1].asFloat();
            init_model.pose.right_eye_inner[2] = (sub_components["right_eye_inner"])[2].asFloat();

            init_model.pose.right_eye[0] = (sub_components["right_eye"])[0].asFloat();
            init_model.pose.right_eye[1] = (sub_components["right_eye"])[1].asFloat();
            init_model.pose.right_eye[2] = (sub_components["right_eye"])[2].asFloat();

            init_model.pose.right_eye_outer[0] = (sub_components["right_eye_outer"])[0].asFloat();
            init_model.pose.right_eye_outer[1] = (sub_components["right_eye_outer"])[1].asFloat();
            init_model.pose.right_eye_outer[2] = (sub_components["right_eye_outer"])[2].asFloat();

            init_model.pose.left_ear[0] = (sub_components["left_ear"])[0].asFloat();
            init_model.pose.left_ear[1] = (sub_components["left_ear"])[1].asFloat();
            init_model.pose.left_ear[2] = (sub_components["left_ear"])[2].asFloat();

            init_model.pose.right_ear[0] = (sub_components["right_ear"])[0].asFloat();
            init_model.pose.right_ear[1] = (sub_components["right_ear"])[1].asFloat();
            init_model.pose.right_ear[2] = (sub_components["right_ear"])[2].asFloat();

            init_model.pose.mouth_left[0] = (sub_components["mouth_left"])[0].asFloat();
            init_model.pose.mouth_left[1] = (sub_components["mouth_left"])[1].asFloat();
            init_model.pose.mouth_left[2] = (sub_components["mouth_left"])[2].asFloat();

            init_model.pose.mouth_right[0] = (sub_components["mouth_right"])[0].asFloat();
            init_model.pose.mouth_right[1] = (sub_components["mouth_right"])[1].asFloat();
            init_model.pose.mouth_right[2] = (sub_components["mouth_right"])[2].asFloat();

        }

        else if (area == "body")
        {
            init_model.pose.body_centroid[0] = (sub_components["_centroid"])[0].asFloat();
            init_model.pose.body_centroid[1] = (sub_components["_centroid"])[1].asFloat();
            init_model.pose.body_centroid[2] = (sub_components["_centroid"])[2].asFloat();

            init_model.pose.left_shoulder[0] = (sub_components["left_shoulder"])[0].asFloat();
            init_model.pose.left_shoulder[1] = (sub_components["left_shoulder"])[1].asFloat();
            init_model.pose.left_shoulder[2] = (sub_components["left_shoulder"])[2].asFloat();

            init_model.pose.right_shoulder[0] = (sub_components["right_shoulder"])[0].asFloat();
            init_model.pose.right_shoulder[1] = (sub_components["right_shoulder"])[1].asFloat();
            init_model.pose.right_shoulder[2] = (sub_components["right_shoulder"])[2].asFloat();

            init_model.pose.left_hip[0] = (sub_components["left_hip"])[0].asFloat();
            init_model.pose.left_hip[1] = (sub_components["left_hip"])[1].asFloat();
            init_model.pose.left_hip[2] = (sub_components["left_hip"])[2].asFloat();

            init_model.pose.right_hip[0] = (sub_components["right_hip"])[0].asFloat();
            init_model.pose.right_hip[1] = (sub_components["right_hip"])[1].asFloat();
            init_model.pose.right_hip[2] = (sub_components["right_hip"])[2].asFloat();

        }

        else if (area == "left_arm")
        {
            init_model.pose.left_arm_centroid[0] = (sub_components["_centroid"])[0].asFloat();
            init_model.pose.left_arm_centroid[1] = (sub_components["_centroid"])[1].asFloat();
            init_model.pose.left_arm_centroid[2] = (sub_components["_centroid"])[2].asFloat();

            init_model.pose.left_elbow[0] = (sub_components["left_elbow"])[0].asFloat();
            init_model.pose.left_elbow[1] = (sub_components["left_elbow"])[1].asFloat();
            init_model.pose.left_elbow[2] = (sub_components["left_elbow"])[2].asFloat();

            init_model.pose.left_wrist[0] = (sub_components["left_wrist"])[0].asFloat();
            init_model.pose.left_wrist[1] = (sub_components["left_wrist"])[1].asFloat();
            init_model.pose.left_wrist[2] = (sub_components["left_wrist"])[2].asFloat();

            init_model.pose.left_pinky[0] = (sub_components["left_pinky"])[0].asFloat();
            init_model.pose.left_pinky[1] = (sub_components["left_pinky"])[1].asFloat();
            init_model.pose.left_pinky[2] = (sub_components["left_pinky"])[2].asFloat();

            init_model.pose.left_index[0] = (sub_components["left_index"])[0].asFloat();
            init_model.pose.left_index[1] = (sub_components["left_index"])[1].asFloat();
            init_model.pose.left_index[2] = (sub_components["left_index"])[2].asFloat();

            init_model.pose.left_thumb[0] = (sub_components["left_thumb"])[0].asFloat();
            init_model.pose.left_thumb[1] = (sub_components["left_thumb"])[1].asFloat();
            init_model.pose.left_thumb[2] = (sub_components["left_thumb"])[2].asFloat();
        }

        else if (area == "right_arm")
        {
            init_model.pose.right_arm_centroid[0] = (sub_components["_centroid"])[0].asFloat();
            init_model.pose.right_arm_centroid[1] = (sub_components["_centroid"])[1].asFloat();
            init_model.pose.right_arm_centroid[2] = (sub_components["_centroid"])[2].asFloat();

            init_model.pose.right_elbow[0] = (sub_components["right_elbow"])[0].asFloat();
            init_model.pose.right_elbow[1] = (sub_components["right_elbow"])[1].asFloat();
            init_model.pose.right_elbow[2] = (sub_components["right_elbow"])[2].asFloat();

            init_model.pose.right_wrist[0] = (sub_components["right_wrist"])[0].asFloat();
            init_model.pose.right_wrist[1] = (sub_components["right_wrist"])[1].asFloat();
            init_model.pose.right_wrist[2] = (sub_components["right_wrist"])[2].asFloat();

            init_model.pose.right_pinky[0] = (sub_components["right_pinky"])[0].asFloat();
            init_model.pose.right_pinky[1] = (sub_components["right_pinky"])[1].asFloat();
            init_model.pose.right_pinky[2] = (sub_components["right_pinky"])[2].asFloat();

            init_model.pose.right_index[0] = (sub_components["right_index"])[0].asFloat();
            init_model.pose.right_index[1] = (sub_components["right_index"])[1].asFloat();
            init_model.pose.right_index[2] = (sub_components["right_index"])[2].asFloat();

            init_model.pose.right_thumb[0] = (sub_components["right_thumb"])[0].asFloat();
            init_model.pose.right_thumb[1] = (sub_components["right_thumb"])[1].asFloat();
            init_model.pose.right_thumb[2] = (sub_components["right_thumb"])[2].asFloat();
        }

        else if (area == "left_leg")
        {
            init_model.pose.left_leg_centroid[0] = (sub_components["_centroid"])[0].asFloat();
            init_model.pose.left_leg_centroid[1] = (sub_components["_centroid"])[1].asFloat();
            init_model.pose.left_leg_centroid[2] = (sub_components["_centroid"])[2].asFloat();

            init_model.pose.left_knee[0] = (sub_components["left_knee"])[0].asFloat();
            init_model.pose.left_knee[1] = (sub_components["left_knee"])[1].asFloat();
            init_model.pose.left_knee[2] = (sub_components["left_knee"])[2].asFloat();

            init_model.pose.left_ankle[0] = (sub_components["left_ankle"])[0].asFloat();
            init_model.pose.left_ankle[1] = (sub_components["left_ankle"])[1].asFloat();
            init_model.pose.left_ankle[2] = (sub_components["left_ankle"])[2].asFloat();

            init_model.pose.left_heel[0] = (sub_components["left_heel"])[0].asFloat();
            init_model.pose.left_heel[1] = (sub_components["left_heel"])[1].asFloat();
            init_model.pose.left_heel[2] = (sub_components["left_heel"])[2].asFloat();

            init_model.pose.left_foot_index[0] = (sub_components["left_foot_index"])[0].asFloat();
            init_model.pose.left_foot_index[1] = (sub_components["left_foot_index"])[1].asFloat();
            init_model.pose.left_foot_index[2] = (sub_components["left_foot_index"])[2].asFloat();

        }

        else if (area == "right_leg")
        {
            init_model.pose.right_leg_centroid[0] = (sub_components["_centroid"])[0].asFloat();
            init_model.pose.right_leg_centroid[1] = (sub_components["_centroid"])[1].asFloat();
            init_model.pose.right_leg_centroid[2] = (sub_components["_centroid"])[2].asFloat();

            init_model.pose.right_knee[0] = (sub_components["right_knee"])[0].asFloat();
            init_model.pose.right_knee[1] = (sub_components["right_knee"])[1].asFloat();
            init_model.pose.right_knee[2] = (sub_components["right_knee"])[2].asFloat();

            init_model.pose.right_ankle[0] = (sub_components["right_ankle"])[0].asFloat();
            init_model.pose.right_ankle[1] = (sub_components["right_ankle"])[1].asFloat();
            init_model.pose.right_ankle[2] = (sub_components["right_ankle"])[2].asFloat();

            init_model.pose.right_heel[0] = (sub_components["right_heel"])[0].asFloat();
            init_model.pose.right_heel[1] = (sub_components["right_heel"])[1].asFloat();
            init_model.pose.right_heel[2] = (sub_components["right_heel"])[2].asFloat();

            init_model.pose.right_foot_index[0] = (sub_components["right_foot_index"])[0].asFloat();
            init_model.pose.right_foot_index[1] = (sub_components["right_foot_index"])[1].asFloat();
            init_model.pose.right_foot_index[2] = (sub_components["right_foot_index"])[2].asFloat();
        }

        //std::cout << "centroid of each sub component" << (sub_components["_centroid"])[0].asFloat() << std::endl;

        //std::cout << sub_components << std::endl;

        //std::cout << initPoseJson[main_components[i]] << std::endl;

    }
    //init_model.print();

}
