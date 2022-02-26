#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "Shadow.h"
#include "Body.h"
#include "globalmodel.h"

#include <json/value.h>
#include <json/json.h>



#define PORT_RECEIVER_BASE 5500

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

		Body dummy_body;
		bool draw_dummy = true;
		float bd_ms_dummy = 0.45;
		float bd_mf_dummy = 0.45;

		//2D shadow 
		Shadow shadow;
		float sh_ms = 0.6; 
		float sh_mf = 0.9;

		//init
		GlobalModel init_model;
		void defineInitModel(Json::Value js);
};
