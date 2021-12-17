import netP5.*;
import oscP5.*;

OscP5 osc;
int lsX, lsY;
int rsX, rsY;
int lhX, lhY;
int rhX, rhY;
int bodyX, bodyY;

void setup(){
  osc = new OscP5(this, 1999);
  osc.plug(this, "pose_handler", "/pose");
  System.setProperty("jogl.disable.openglcore", "true");
  size(1000, 600, P3D);
  //noStroke();
}

void draw() {
    background(230);
    
    //fill(100);
    
    //beginShape();
    //vertex(2*lsX-800,2*lsY-500);
    //vertex(2*rsX-800,2*rsY-500);
    //vertex(2*rhX-800,2*rhY-500);
    //vertex(2*lhX-800,2*lhY-500);
    //vertex(2*lsX-800,2*lsY-500);
    //endShape();
    
    directionalLight(51, 102, 126, -1, 0, 0);
    
    fill(220);
    rect(0, 500, 1000, 100);
    
    beginShape();
    vertex(lsX,lsY);
    vertex(rsX,rsY);
    vertex(rhX,rhY);
    vertex(lhX,lhY);
    vertex(lsX,lsY);
    endShape();
    
    ellipse(bodyX, bodyY, 10, 10);
    

}

void pose_handler(float lsX,
                  float lsY,
                  float rsX,
                  float rsY,
                  float lhX,
                  float lhY,
                  float rhX,
                  float rhY,
                  float bodyX,
                  float bodyY
                  ) {
  this.lsX = int(lsX*1000);
  this.lsY = int(lsY*600);
  this.rsX = int(rsX*1000);
  this.rsY = int(rsY*600);
  this.lhX = int(lhX*1000);
  this.lhY = int(lhY*600);
  this.rhX = int(rhX*1000);
  this.rhY = int(rhY*600);
  this.bodyX = int(bodyX*1000);
  this.bodyY = int(bodyY*600);
}
