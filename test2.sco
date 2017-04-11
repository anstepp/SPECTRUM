//To run this, cd to the directory in which it's living, then type:
// cmix < test2.sco

rtsetparams(44100, 2)
//you need the ./libSPECTRUM because this isn't part of the distribution yet
load("./libSPECTRUM.so")
load("GVERB")


//bus_config("./libSPECTRUM.so", "aux 0-1 out")
//bus_config("GVERB", "aux 0-1 in", "out 0-1")

//rtoutput("/Users/aaronstepp/Desktop/funny1.aif")

max_dur = 120
step_size = .125

for (st = 0; st < max_dur; st += step_size) {
	srand(trand(0, 9000))
	dur = step_size;
	amp = irand(1000, 2000) * maketable("curve", 1000, 0,0,2, 100,1,-2, 300,.75,0, 900,.75,2, 1000,0);
	freq = irand(880, 1760);

	partials = 200;

   //this table determines the detuning amount - it's in triples time,amount,curve slope
	table = maketable("curve", "nonorm", 1000, 0,irand(0,200),2, 1000,irand(0,200))
	//this table is for onset times of the partials
    onset = maketable("curve", "nonorm", partials, 0,0,2, partials,0)
   //vars: start, duration, amplitude, base frequency, number of partials, detuning curve, onset curve, seed
	SPECTRUM(st, dur, amp, freq, partials, table, onset, 0.5);
	step_size = irand(0.125, 1);
}

//GVERB(0, 0, max_dur, 0.9, 300, 100.0, 0.1, 1, 0.0, -11.0, -9.0, 20.0)