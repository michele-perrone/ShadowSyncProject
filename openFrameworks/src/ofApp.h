#pragma once

#include "ofMain.h"
#include "Body.h"
#include "ofxOsc.h"
#include "Shadow.h"
#include "Particle3D.h"

#define PORT 1998

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
		
	
		ofMatrix4x4 mat; //only way i found for moving around a mesh. In depth, every 3D transformation (rotation, translation, scaling) is performed by 4x4 matrices

		ofLight light;
		ofEasyCam cam;
		ofBoxPrimitive box;
		ofMaterial boxMaterial;

		ofPlanePrimitive plane_floor;
		ofPlanePrimitive plane_wall;

		ofMaterial floor_material;
		ofMaterial wall_material;

		Body body;
		float z_body = 120; //dummy value until we have 3D coords from osc
		
		//OSC
		ofxOscReceiver osc_receiver;
		float data1, data2;

		//2D shadow test
		Shadow shadow;
};
