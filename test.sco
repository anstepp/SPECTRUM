//To run this, cd to the directory in which it's living, then type:
// cmix < test.sco

rtsetparams(44100, 2)
//you need the ./libSPECTRUM because this isn't part of the distribution yet
load("./libSPECTRUM.so")

reset(44100);

rtoutput("/Users/aaronstepp/Desktop/funny1.aif")

for (st = 0; st < 60; st += 10) {
   dur = 10;
   amp = 10000 //* maketable("curve", 1000, 0,0,2, 100,1,-2, 300,.85,0, 900,.85,2, 1000,0);
   //you can give pitches if you want, this just generates a random pitch from A1 to A4
   freq = irand(110, 440);
   partials = 20;
   //this table determines the detuning amount - it's in triples time,amount,curve slope
   table = maketable("curve", "nonorm", 1000, 0,2,2, 100,0,2, 250,10,2, 400,0,2, 600,5,2, 750,20,4, 990,0,2, 1000,0);
   onset = maketable("curve", "nonorm", partials, 0,2,2, partials,0);
   SPECTRUM(st, dur, amp, freq, partials, table, onset, 1);
}