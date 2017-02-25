rtsetparams(44100, 2)
load("./libSPECTRUM.so")


for (st = 0; st < 8; st = st + .10) {
   dur = .03 + (.06 * random())
   amp = 8000 + (rand() * 3000)
   freq = irand(400,500)
   SPECTRUM(st, dur, amp, freq, 10)
}

