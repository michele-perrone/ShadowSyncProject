#include "ofApp.h"
#define PL_XZ 400 //the xz plane dimension
#define PL_Y 300  //the y plane dimension 


//--------------------------------------------------------------
void ofApp::setup(){
    //sphere.set(30, 10);
    //sphere.move(100, 100,0);
    //mesh = sphere.getMesh(); //doesn't take the sphere's positioning

    //spherical mesh color 
    //for (int i = 0; i < mesh.getVertices().size(); i++)
    //{
    //    mesh.addColor(ofFloatColor(float(i) / mesh.getVertices().size(), 0, 1.0 - (float(i) / mesh.getVertices().size())));
    //}

    light.setup();
    light.setPosition(-50, 200, 200);

    ofEnableDepthTest(); //you can't see through objects (es. wall)


    cam.setPosition(ofVec3f(-180, 100, 250)); //camera positioning and heading configuration
    cam.lookAt(ofVec3f(0, 20, -20));
    
    mat.rotate(90, 0, 0, 1);    //with this two commands, we are combining two "high-abstraction level" matrix transformations
    mat.translate(0, 0, 1);    //in just one (otherwise, at a low level you can define manually a 4x4 mat that performs your transformation)
    
    plane_floor.set(PL_XZ, PL_XZ);
    plane_floor.rotateDeg(270, 1, 0, 0); //by default, it is orthogonal to x axis, centered in origin

    plane_wall.set(PL_XZ, PL_Y);

    //material def
    floor_material.setDiffuseColor(ofFloatColor::darkGray);
    floor_material.setShininess(0.01);
    wall_material.setDiffuseColor(ofFloatColor::white);
    wall_material.setShininess(0.01);

    body.setup();
    body.move(-100, 40, z_body); //moving com --> moves all body

    // OSC
    osc_receiver.setup(PORT);

    //Shadow 
    shadow.setup(0, 0);

    /*manual setup of a circle mesh NOT WORKING WELL the rendering for some triangles
    //circleMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    int radius = 10;
    int circle_pts = 200;
    float omega = 0;
    int zc = 0;
    circleMesh.addVertex(ofVec3f(0, 0, 0)); //origin
    for (int i = 0; i < circle_pts; i++)
    {
        circleMesh.addVertex(ofVec3f(cos(omega) * radius, sin(omega) * radius, zc)); //circonference vertex
        //std::cout << "cos, sin: " << cos(omega) << ", " << sin(omega) << std::endl;
        circleMesh.addColor(ofFloatColor(0, 0, 0));
        omega += (TWO_PI/circle_pts); 
    }

    for (int j = 1; j < circle_pts ; j++)
    {
        
        circleMesh.addIndex(j);
        circleMesh.addIndex(0);
        circleMesh.addIndex(j + 1);
    }

   
    circleMesh.addIndex(circle_pts);
    circleMesh.addIndex(0);
    circleMesh.addIndex(1);
    */

}

//--------------------------------------------------------------
void ofApp::update(){
    /* //quick guide on how to biuld a custom mesh
    //build the mesh of a square: 4 vertices and 6 indexes (min. conditions required by 2 triangles)
     
    mesh.addVertex(ofPoint(0, 0, 0)); // make a new vertex
    mesh.addColor(ofFloatColor(0, 0, 0));  // add a color at that vertex

    mesh.addVertex(ofPoint(width, 0, 0));
    mesh.addColor(ofFloatColor(0, 0, 0));  

    mesh.addVertex(ofPoint(width, height, 0));
    mesh.addColor(ofFloatColor(0, 0, 0)); 

    mesh.addVertex(ofPoint(0, height, 0));
    mesh.addColor(ofFloatColor(0, 0, 0)); 

    //first triangle composed of three vertices --> those at index [0,1,2] of the mesh
    mesh.addIndex(0);
    mesh.addIndex(1);
    mesh.addIndex(2);
    //second triangle composed of three vertices--> those at index [2,3,0] of the mesh
    mesh.addIndex(2);
    mesh.addIndex(3);
    mesh.addIndex(0);
    */

    //OSC
    while (osc_receiver.hasWaitingMessages())
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
    }

 
}

//--------------------------------------------------------------
void ofApp::draw(){

    //everything shown in 3D rendering goes btw cam.begin() and cam.end(), same for material
    cam.begin();

    ofBackground(142, 124, 200);

    floor_material.begin();
    plane_floor.draw();
    floor_material.end();

    

    wall_material.begin();
    plane_wall.draw();
    wall_material.end();


    //spherical mesh roto-translation
    //ofPushMatrix();
    //ofMultMatrix(mat);
    //mesh.draw();
    //ofPopMatrix();

    ofDrawGrid(20, 20, true, true, true, true); //3D grid
    
    body.draw();
    //circleMesh.draw(); //does not work well for some reason a triangle has rendering problems
    ofPushMatrix();
    ofMultMatrix(mat); //this matrix multiplication allow us to see the circle
    //ofSetColor(ofFloatColor::gold);
    //ofFill();
    //ofDrawCircle(0, 0, 10); //easiest solution for drawing a circle on the wall
    shadow.draw(); //dummy solution, otherwise not visible since its a 2D entity and gets covered by wall plane
    ofPopMatrix();

    
    cam.end();
    
    //show FPS on screen
    stringstream ss;
    ss << "FPS: " << ofToString(ofGetFrameRate(), 0) << endl << endl;
    ofDrawBitmapString(ss.str().c_str(), 20, 20);

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
