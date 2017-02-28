rtsetparams(44100, 2)
load("./libSPECTRUM.so")


for (st = 0; st < 10; st += 1) {
   dur = 1.1;
   amp = 10000
   freq = irand(400,1500)
   SPECTRUM(st, dur, amp, freq, 10)
}

