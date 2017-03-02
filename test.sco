rtsetparams(44100, 2)
load("./libSPECTRUM.so")

reset(44100);

rtoutput("/Users/aaronstepp/Desktop/funny1.aif")

for (st = 0; st < 60; st += 10) {
   dur = 10.1;
   amp = 10000;
   freq = irand(110, 440);
   table = maketable("curve", "nonorm", 1000, 0,0,2, 500,50,2, 1000,0);
   SPECTRUM(st, dur, amp, freq, 10, table, 1);
}

