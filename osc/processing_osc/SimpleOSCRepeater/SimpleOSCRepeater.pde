import netP5.*;
import oscP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;

void setup(){
  oscP5 = new OscP5(this, 1254);
  myRemoteLocation = new NetAddress("127.0.1.1",1255);
}

void draw() {
}

void oscEvent(OscMessage m) {
  println("Received! " + m.addrPattern() + " , " + m.get(0).intValue());
  OscMessage new_m = new OscMessage(m.addrPattern());
  new_m.add(m.get(0).intValue());
  println("Sending...");
  oscP5.send(new_m, myRemoteLocation); 
  //myMessage.print();
}
