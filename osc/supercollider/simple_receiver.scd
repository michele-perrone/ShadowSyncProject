NetAddr.langPort;
NetAddr.localAddr;
OSCFunc.trace(true);

thisProcess.openUDPPort(5555);

n = NetAddr.new("127.0.0.1", 1255);    // create the NetAddr
// create the OSCFunc
o = OSCFunc({ arg msg, time, addr, recvPort; [msg, time, addr, recvPort].postln; }, '/correlation', n);
o.free;