rtsetparams(44100, 2)

load("./libSPECTRUM.so")

st = 0
dur = 10
amp = 10000
freq = 440
partials = 18
detune_table = maketable("curve", "nonorm", 1000, 0,100,-2, 10,0,2, 1000,10)
onset = maketable("curve", "nonorm", partials, 0,0,2, partials,1)
seed = 0.25
pan = 0.5
SPECTRUM(st, dur, amp, freq, partials, detune_table, onset, seed, pan)