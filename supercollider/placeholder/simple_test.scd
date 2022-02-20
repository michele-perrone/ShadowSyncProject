s.boot
thisProcess.openUDPPort(5555);
OSCFunc.trace(false);

(
SynthDef(\bass, { |out, freq = 440, gate = 1, amp = 0.5, slideTime = 0.17, ffreq = 1100, width = 0.15,
        detune = 1.005, preamp = 4|
    var sig, env;
    env = Env.adsr(0.01, 0.3, 0.4, 0.1);
    freq = Lag.kr(freq, slideTime);
    sig = Mix(VarSaw.ar([freq, freq * detune], 0, width, preamp)).distort * amp
        * EnvGen.kr(env, gate, doneAction: Done.freeSelf);
    sig = LPF.ar(sig, ffreq);
    Out.ar(out, sig ! 2)
}).add;
)

(
p = Pmono(
	\bass,
	\dur, 1,
	\freq, Pwhite(1,5) * 100,
	\legato, Pseq([1, 2.5], inf),
	\midinote, Pseq([60, 64, 57], 3),
	\amp, 0.5,
	\detune, 1.005
).play;
)

p.play;
p.stop;

(
OSCdef('OSCreceiver',
	{
		arg msg;
		var pyPlay, pyDur;


		[msg].postln;
		pyPlay = msg[1];
		pyDur = msg[2];


		// Parse message
		if(pyPlay == 1){
			p.stop;
			p = Pmono(
				\bass,
				\dur, pyDur,
				\freq, Pwhite(1,5) * 100,
				\legato, Pseq([1, 2.5], inf),
				\midinote, Pseq([60, 64, 57], 50),
				\amp, 0.5,
				\detune, 1.005
			).play;
		};

		if(pyPlay == 0){
			p.stop;
		};

	},
	"/correlation");
)



s.quit