//To run this, cd to the directory in which it's living, then type:
// cmix < test.sco

rtsetparams(44100, 2)
//you need the ./libSPECTRUM because this isn't part of the distribution yet
load("./libSPECTRUM.so")

rtoutput("/Users/aaronstepp/Desktop/funny1.aif")

step = 90

for (st = 0; st < 200; st += step) {
   dur = step;
   amp = 10000
   //you can give pitches if you want, this just generates a random pitch from A1 to A4
   env = maketable("curve", 1000, 0,1,2, 1000,0.5)
   freq = irand(880, 1270);
   partials = trand(100,200);
   //this table determines the detuning amount - it's in triples time,amount,curve slope
   table = maketable("curve", "nonorm", 1000, 
      0,irand(0,200),2, irand(1,150),irand(0,50),2, irand(125, 400),irand(100,300),2, 
      irand(600,700),0,2, irand(800,900),irand(100,400),2, 
      901,200,2, irand(975,990),0,0, 1000,0);
   //table = maketable("line", 1000, 0,0, 1000,0)
   onset = maketable("curve", "nonorm", partials, 0,irand(0,4),2, 
                     partials/2,0,2, partials,irand(0,4));
   seed = 0.5
   pan = maketable("line", 1000, 0,0.5, 500,1, 1000,0)
   SPECTRUM(st, dur, amp * env, freq, partials, table, onset, seed, pan)
   step = irand(60, 90)
}