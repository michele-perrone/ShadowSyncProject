import netP5.*;
import oscP5.*;

OscP5 osc;
MyPoint p;
float x, y, z;

void setup(){
  osc = new OscP5(this, 1998);
  osc.plug(this, "accelerometer_x_handler", "/accelerometer/x");
  osc.plug(this, "accelerometer_y_handler", "/accelerometer/y");
  osc.plug(this, "accelerometer_z_handler", "/accelerometer/z");
}

void draw() {
  println("( " + x + " , " + y + " , " + z + " )");
}

void accelerometer_x_handler(float value) {
  x = value;
}


void accelerometer_y_handler(float value) {
  y = value;
}


void accelerometer_z_handler(float value) {
  z = value;
}
