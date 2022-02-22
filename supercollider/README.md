# SuperCollider

To start super collider:

1. `s.boot` to start server 
2. block of code `SynthDef(\sawpulse)` to define the type of sound
3. block of code `p=PmonoArtic` to start the sound
4. `v = s.volume` to save volume's control
5. `v.mute` to mute volume (in osc, if is play, the volume turn on)
6. block of code `OSCdef` to receive osc messages