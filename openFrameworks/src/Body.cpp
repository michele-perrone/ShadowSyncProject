#include "Body.h"

Body::Body() {

}

void Body::setup() {
	material.setDiffuseColor(ofFloatColor::aqua);
	com.set(3, 10); //center of mass
	for (int i = 0; i < 5; i++) {
		//vector of junctions
		ofSpherePrimitive temp_junction;
		temp_junction.set(2, 10);
		temp_junction.setParent(com);
		body_junctions.push_back(temp_junction);

		//array of junctions
		//body_junctions[i].set(2, 10);
		//body_junctions[i].setParent(com);      //each junction is a child of com, if it moves, the whole body moves
		//body_junctions[i].move(20+20*i, 0, 0); //algined along x axis
	}
	//child nodes can move independetly
	body_junctions[0].move(0, 50, 0);	//head
	body_junctions[1].move(-30, 40, 0); //left shoulder
	body_junctions[2].move(30, 40, 0);  //right shoulder
	body_junctions[3].move(-20, -30, 0);//left hip (anca)
	body_junctions[4].move(20, -30, 0); //rigth hip
	
}

void Body::draw() {
	com.draw(); //not in same loop just for having a different (white, default) color for com

	material.begin(); //this will affect all the elements inside (like push/pop matrix)
	for (int i = 0; i < 5; i++)
		body_junctions[i].draw();
	material.end();

}

void Body::move(float x_dir, float y_dir, float z_dir)
{
	com.move(x_dir, y_dir, z_dir); //if a parent node moves --> all child nodes will follow
}