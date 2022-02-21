#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "Shadow.h"
#include "Body.h"
#include "globalmodel.h"

//#define CLIENT_NUMBER 1
//#define PORT_RECEIVER 5501

#define CLIENT_NUMBER 2
#define PORT_RECEIVER 5502

#define PORT_SENDER 1255


class ofApp : public ofBaseApp{
	// default openFrameworks setup
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
	
        ofMatrix4x4 mat; // Only way i found for moving around a mesh.
                         // In depth, every 3D transformation (rotation, translation, scaling)
                         // is performed by 4x4 matrices

		ofLight light;
		ofEasyCam cam;
		ofBoxPrimitive box;
		ofMaterial boxMaterial;

		ofPlanePrimitive plane_floor;
		ofPlanePrimitive plane_wall;

		ofMaterial floor_material;
        ofMaterial wall_material;
		
		//OSC
		ofxOscReceiver osc_receiver;
        ofxOscSender   osc_sender;
        GlobalModel global_model;
        std::vector<std::string> split(string address);
        void handle_address(ofxOscMessage * m);

		//3D body
		Body body;
		float bd_ms = 0.5;
		float bd_mf = 1.5;

		//2D shadow 
		Shadow shadow;
		float sh_ms = 0.054; 
		float sh_mf = 0.046;
};
