#pragma once

#include "ofMain.h"
#include "Body.h"

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
		
	// SSP setup	
		//ofMesh mesh;
		//int width = 100;
		//int height = 100;

		//ofSpherePrimitive sphere;
		//ofVboMesh mesh;

		//float moving_x_coord = 0;
		//int t = 0;

		//ofMatrix4x4 mat; //only way i found for moving around a mesh. In depth, every 3D transformation (rotation, translation, scaling) is performed by 4x4 matrices

		ofLight light;
		ofEasyCam cam;
		ofBoxPrimitive box;
		ofMaterial boxMaterial;

		ofPlanePrimitive plane_floor;
		ofPlanePrimitive plane_wall;

		ofMaterial floor_material;
		ofMaterial wall_material;

		Body body;

};
