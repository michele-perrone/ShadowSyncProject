import netP5.*;
import oscP5.*;

OscP5 osc;

void setup(){
  osc = new OscP5(this, 1998);
}

void draw() {
}

void oscEvent(OscMessage m) {
  println(m.addrPattern() + " , " + m.typetag());
}
