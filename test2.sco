//To run this, cd to the directory in which it's living, then type:
// cmix < test2.sco

rtsetparams(44100, 2)
//you need the ./libSPECTRUM because this isn't part of the distribution yet
load("./libSPECTRUM.so")

max_dur = 120
step_size = .125

for (st = 0; st < max_dur; st += step_size) {
	srand(trand(0, 9000))
	dur = step_size * 2;
	amp = irand(1000, 2000) 
	env = maketable("curve", 1000, 0,0,2, 100,1,-2, 300,.75,0, 900,.75,2, 1000,0);
	freq = irand(st ^ 2 * step_size, st ^ 3 * step_size);

	partials = 200;

	//this table determines the detuning amount - it's in triples time,amount,curve slope
	table = maketable("curve", "nonorm", 1000, 0,irand(0,200),2, 1000,irand(0,200))
	//this table is for onset times of the partials
    onset = maketable("curve", "nonorm", partials, 0,0,2, partials,0)
	//vars: start, duration, amplitude, base frequency, number of partials, detuning curve, set curve, seed
    seed = 0.1
    pan = maketable("line", 1000, 0,0, 1000,1)
	SPECTRUM(st, dur, amp * env, freq, partials, table, onset, seed, pan)
	step_size = irand(0.0125, 0.1);
}
