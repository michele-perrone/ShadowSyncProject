s.boot
thisProcess.openUDPPort(5555);
OSCFunc.trace(false);

(
SynthDef(\sawpulse, { |out, freq = 440, gate = 0.5, plfofreq = 6, mw = 0, ffreq = 2000, rq = 0.3, freqlag = 0.05, amp = 1|
    var sig, plfo, fcurve;
    plfo = SinOsc.kr(plfofreq, mul:mw, add:1);
    freq = Lag.kr(freq, freqlag) * plfo;
    fcurve = EnvGen.kr(Env.adsr(0, 0.3, 0.1, 20), gate);
    fcurve = (fcurve - 1).madd(0.7, 1) * ffreq;
    sig = Mix.ar([Pulse.ar(freq, 0.9), Saw.ar(freq*1.007)]);
    sig = RLPF.ar(sig, fcurve, rq)
        * EnvGen.kr(Env.adsr(0.04, 0.2, 0.6, 0.1), gate, doneAction: Done.freeSelf)
        * amp;
    Out.ar(out, sig ! 2)
}).add;
)

p.play;
p.stop;
(
p = PmonoArtic(
	\sawpulse,
	\dur, 2,
	\freq, Pwhite(1,5) * 80,
	\legato, Pwrand(#[0.5, 1.0], #[0.1, 0.9], inf)
).play;
)

v = s.volume;
v.mute;


(
OSCdef('OSCreceiver',
	{
		arg msg;
		var pyPlay, pyDur, pySync, oldPyDur;


		[msg].postln;
		pyPlay = msg[1];
		pyDur = msg[2];
		oldPyDur = msg[3];
		pySync = msg[4];


		// Parse message
		if(pyPlay == 1){
			v.unmute;
			if(oldPyDur != pyDur){
				p.stop;
				if(pySync == 1){
					p = PmonoArtic(
						\sawpulse,
						\dur, pyDur,
						\freq, Pwhite(1,5) * 100,
						\legato, Pwrand(#[0.5, 1.0], #[0.1, 0.9], inf)
					).play;
				};
				if(pySync == 2){
					p = PmonoArtic(
						\sawpulse,
						\dur, pyDur,
						\freq, Pwhite(5,8) * 300,
						\legato, Pwrand(#[0.5, 1.0], #[0.1, 0.9], inf)
					).play;
				}
			};
			oldPyDur = pyDur;

		};

		if(pyPlay == 0){
			p.stop;
		};

	},
	"/correlation");
)



s.quit