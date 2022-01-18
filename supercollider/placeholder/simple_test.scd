(
SynthDef(\dubstep, { arg freq = 56;
	Out.ar(0, SinOsc.ar(freq, 0, 0.5))
}).add;
)

x = Synth(\dubstep,[\freq,22]);
x.set(\freq,30);
x.set(\freq,60);

// Receiver
NetAddr("127.0.0.1",1255);
NetAddr.langPort;
NetAddr.localAddr;
OSCFunc.trace(true);
OSCFunc.trace(false);
thisProcess.openUDPPort(5555);

(

x = Synth(\dubstep);

OSCdef('OSCreceiver',
	{
		arg msg;
		var freq = msg[0];
		x.set(\freq,freq);
		[msg].postln;

	},
	"/correlation");
)

s.scope
