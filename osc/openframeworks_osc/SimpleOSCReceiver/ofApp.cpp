#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    osc.setup(PORT);

}

//--------------------------------------------------------------
void ofApp::update()
{
    while (osc.hasWaitingMessages())
    {
        ofxOscMessage m;
        osc.getNextMessage(m);

        if (m.getAddress() == "/pose")
        {
          lsX = m.getArgAsFloat(0);
          lsY = m.getArgAsFloat(1);
          rsX = m.getArgAsFloat(2);
          rsY = m.getArgAsFloat(3);
          lhX = m.getArgAsFloat(4);
          lhY = m.getArgAsFloat(5);
          rhX = m.getArgAsFloat(6);
          rhY = m.getArgAsFloat(7);
          bodyX = m.getArgAsFloat(8);
          bodyY = m.getArgAsFloat(9);

          std::cout << "lsX " << lsX << std::endl;
          std::cout << "lsY " << lsY << std::endl;
          std::cout << "rsX " << rsX << std::endl;
          std::cout << "rsY " << rsY << std::endl;
          std::cout << "lhX " << lhX << std::endl;
          std::cout << "lhY " << lhY << std::endl;
          std::cout << "rhX " << rhX << std::endl;
          std::cout << "rhY " << rhY << std::endl;
          std::cout << "bodyX " << bodyX << std::endl;
          std::cout << "bodyY " << bodyY << std::endl;
        }
      }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofDrawCircle(lsX*ofGetWidth(), lsY*ofGetHeight(), 10);
    ofDrawCircle(rsX*ofGetWidth(), rsY*ofGetHeight(), 10);
    ofDrawCircle(lhX*ofGetWidth(), lhY*ofGetHeight(), 10);
    ofDrawCircle(rhX*ofGetWidth(), rhY*ofGetHeight(), 10);
    ofDrawCircle(bodyX*ofGetWidth(), bodyY*ofGetHeight(), 10);

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
