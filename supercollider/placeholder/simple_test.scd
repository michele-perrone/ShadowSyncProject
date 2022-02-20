s.boot
thisProcess.openUDPPort(5555);
OSCFunc.trace(false);

(
SynthDef(\bass, { |out, freq = 440, gate = 1, amp = 0.5, slideTime = 0.17, ffreq = 1100, width = 0.15,
        detune = 1.005, preamp = 4, noise = 0|
    var sig, env;
    env = Env.adsr(0.01, 0.3, 0.4, 0.1);
    freq = Lag.kr(freq, slideTime);
    sig = Mix(VarSaw.ar([freq, freq * detune], 0, width, preamp)).distort * amp
        * EnvGen.kr(env, gate, doneAction: Done.freeSelf);
    sig = LPF.ar(sig, ffreq);
	if(noise == 1){
		sig = BPF.ar(
		sig,
		{LFNoise1.kr(
			LFNoise1.kr(4).exprange(cfhzmin,cfhzmax)
		).exprange(cfmin, cfmax)}!2,
		{LFNoise1.kr(0.1).exprange(rqmin, rqmax)}!2
		);
	}

    Out.ar(out, sig ! 2)
}).add;
)

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
OSCdef('OSCreceiver',
	{
		arg msg;
		var pyPlay, pyDur, pySync;


		[msg].postln;
		pyPlay = msg[1];
		pyDur = msg[2];
		pySync = msg[3];


		// Parse message
		if(pyPlay == 1){
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
					\freq, Pwhite(1,8) * 300,
					\legato, Pwrand(#[0.5, 1.0], #[0.1, 0.9], inf)
				).play;
			};

		};

		if(pyPlay == 0){
			p.stop;
		};

	},
	"/correlation");
)



s.quit