//To run this, cd to the directory in which it's living, then type:
// cmix < test.sco

rtsetparams(44100, 2)
//you need the ./libSPECTRUM because this isn't part of the distribution yet
load("./libSPECTRUM.so")
load("GVERB")


bus_config("./libSPECTRUM.so", "aux 0-1 out")
bus_config("GVERB", "aux 0-1 in", "out 0-1")

//rtoutput("/Users/aaronstepp/Desktop/funny1.aif")

max_dur = 120
step_size = .125

for (st = 0; st < max_dur; st += step_size) {
   srand(trand(0, 9000))
   dur = step_size;
   amp = irand(1000, 2000);
   //you can give pitches if you want, this just generates a random pitch from A1 to A4
   freq = irand(880, 1760);
   //this table determines the detuning amount - it's in triples time,amount,curve slope
   table = maketable("curve", "nonorm", 1000, 0,irand(0,200),2, 100,irand(0,200),2, 250,irand(0,200),2, 400,irand(0,200),2, 600,5,2, 750,irand(0,200),4, 990,irand(0,200),2, 1000,irand(0,200));
   //vars: start, duration, amplitude, base frequency, number of partials, detuning curve, seed
   SPECTRUM(st, dur, amp, freq, 200, table, 2);
   step_size = irand(0.125, 0.6);
}

GVERB(0, 0, max_dur * 1.5, 0.9, 300, 7.0, 0.71, 1, -90.0, -11.0, -9.0, 20.0)