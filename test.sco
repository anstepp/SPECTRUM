rtsetparams(44100, 2)
load("./libSPECTRUM.so")


for (st = 0; st < 100; st += 10) {
   dur = 10.1;
   amp = 500;
   freq = 440;
   table = maketable("curve", "nonorm", 1000, 0,0,2, 500,50,2, 1000,0);
   SPECTRUM(st, dur, amp, freq, 5, table, 1);
}

