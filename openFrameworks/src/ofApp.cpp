#include "ofApp.h"
#define PL_XZ 400 //xz plane dimension
#define PL_Y 300  //y plane dimension 


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
    light.setPosition(0, 200, 200);

    ofEnableDepthTest(); //you can't see through objects


    cam.setPosition(ofVec3f(-180, 100, 250)); //camera positioning and heading config
    cam.lookAt(ofVec3f(0, 20, -20));
    
    //mat.rotate(180, 0, 0, 1);    //with this two commands, we are combining two "high-abstraction level" matrix transformations
    //mat.translate(-150, 100, 0); //in just one (otherwise, you can define manually a 4x4 mat that performs your transformation)

    plane_floor.set(PL_XZ, PL_XZ);
    plane_floor.rotateDeg(270, 1, 0, 0); //by default, it is orthogonal to x axis, centered in origin

    plane_wall.set(PL_XZ, PL_Y);

    floor_material.setDiffuseColor(ofFloatColor::darkGray);
    floor_material.setShininess(0.01);
    wall_material.setDiffuseColor(ofFloatColor::white);
    wall_material.setShininess(0.01);

    body.setup();
    body.move(-100, 40, 120); //moving com --> moves all body
}

//--------------------------------------------------------------
void ofApp::update(){
    /* //how to biuld a custom mesh
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

    cam.end();
    
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
