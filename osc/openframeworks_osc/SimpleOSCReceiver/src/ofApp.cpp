#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    osc_receiver.setup(PORT);

}

//--------------------------------------------------------------
void ofApp::update()
{
    while (osc_receiver.hasWaitingMessages())
    {
        ofxOscMessage m;
        osc_receiver.getNextMessage(m);
        if (m.getAddress() == "/pose")
        {
          data1 = m.getArgAsFloat(0);
          //data2 = m.getArgAsFloat(1);

          std::cout << "data1 = " << data1 << std::endl;
          //std::cout << "data2 = " << data2 << std::endl;
        }
      }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    //ofDrawCircle(data1*ofGetWidth(), data2*ofGetHeight(), 10);

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
