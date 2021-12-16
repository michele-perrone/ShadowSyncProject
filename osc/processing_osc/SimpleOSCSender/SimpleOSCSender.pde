import netP5.*;
import oscP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;

void setup(){
  oscP5 = new OscP5(this, 1998);
  myRemoteLocation = new NetAddress("127.0.1.1",1998);
  frameRate(5);
}

void draw() {
  OscMessage myMessage = new OscMessage("/filter");
  myMessage.add(342);
  println("Sending");
  oscP5.send(myMessage, myRemoteLocation); 
  myMessage.print();
}
