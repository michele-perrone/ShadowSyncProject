#include "ofApp.h"
#define PL_XZ 200 //the xz plane dimension
#define PL_Y 100  //the y plane dimension 


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
    //body.setup();
    //body.move(100, 40, z_body); //moving com --> moves all body

    // OSC
    //osc_receiver.setup(PORT);

    //Shadow 
    float shadow_origin[] = { 50, 50 };
    glm::vec2 shadow_vOrigin = glm::make_vec2(shadow_origin);
    shadow.setup(shadow_vOrigin);
    //shadow.move(50,50);
}

//--------------------------------------------------------------
void ofApp::update()
{
    //OSC
    /*while (osc_receiver.hasWaitingMessages())
    {
        ofxOscMessage m;
        osc_receiver.getNextMessage(m);
        if (m.getAddress() == "/pose")
        {
            data1 = m.getArgAsFloat(8); // x centroid
            data2 = m.getArgAsFloat(9); // y centroid

            //shadow and body update, still testing 
            shadow.move(data1, data2); //actually it doesn't seem to move as the centroid, but at least it moves
            body.move(0.5-data1, 0.5-data2, 0); // both x, y dir nomalized btw -0.5, 0.5 (otherwise it will go far away along + x,y dir)
            std::cout << "data1 = " << data1 << std::endl;
            std::cout << "data2 = " << data2 << std::endl;
        }
    }*/  
   // body.move(0.5,0.5,0.1);
    //body.updateParticleSystems();
    //body.move(0.1, 0.1, 0.1);
    shadow.updateParticleSystems();
    shadow.move(0.2, 0);
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

    //body.draw(); // The body takes now also care of the particle systems!
   
    shadow.draw();
 

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
