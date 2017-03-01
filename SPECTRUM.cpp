/* SPECTRUM - simple additive synthesis instrument


*/
#include <stdio.h>
#include <stdlib.h>
#include <ugens.h>
#include "SPECTRUM.h"
#include <rt.h>
#include <rtdefs.h>
#include <stdlib.h>
#include <Instrument.h>
#include <ugens.h>
#include <Ougens.h>



SPECTRUM::SPECTRUM()
	
{
	_branch = 0;
	detunetable = NULL;
}


SPECTRUM::~SPECTRUM()
{
	delete [] wavetable;
	delete detunetable;
}


int SPECTRUM::init(double p[], int n_args)
{
	_nargs = n_args;		// store this for use in doupdate()

	// Store pfields in variables, to allow for easy pfield renumbering.
	// You should retain the RTcmix numbering convention for the first
	// 4 pfields: outskip, inskip, dur, amp; or, for instruments that 
	// take no input: outskip, dur, amp.

	const float outskip = p[0];
	const float dur = p[1];

	if (rtsetoutput(outskip, dur, this) == -1)
		return DONT_SCHEDULE;

	if (outputChannels() > 2)
		return die("SPECTRUM", "Use mono or stereo output only.");

	_amp = p[2];

	float freqraw = p[3];
	float freq;
	if (freqraw < 15.0)
		freq = cpspch(freqraw);
	else
		freq = freqraw;

	partials = p[4];

	osc = new Ooscili * [partials];

	//create sine wave
	tablelen = 2048;
	wavetable = new double [tablelen];
	const double twopi = M_PI * 2.0;
	for (int i = 0; i < tablelen; i++){
		wavetable[i] = sin(twopi * ((double) i / tablelen));
	}

	for (int i = 0; i < partials; i++){
		osc[i] = new Ooscili(SR, freq * (i + 1), wavetable, tablelen);
	}

	//table for detuning p[5]
	int wavelen;
	double *wavet = (double *) getPFieldTable(5, &wavelen);
	theDetuner = new Ooscili(SR, 1.0/dur, wavet, wavelen);
	
	//seed
	
	int seed = p[6];
	theRand = new Orand(seed);

	return nSamps();
}


int SPECTRUM::configure()
{
	return 0;
}


// Called at the control rate to update parameters like amplitude, pan, etc.

void SPECTRUM::doupdate()
{

	// double p[_nargs];
	// update(p, 5);

	// for (int i = 0; i < partials; i++){
	// 	float freqraw = freq * (i + 1);
	// 	float detunebottom = freqraw - (detuneamount / 2);
	// 	float detunetop = freqraw + (detuneamount / 2);
	// 	float adjust = freqraw + theRand->range(detunebottom, detunetop);
	// 	freq = freqraw + adjust;
	// 	osc[i]->setfreq(freq);
	// }

}


int SPECTRUM::run()
{
	for (int i = 0; i < framesToRun(); i++) {

		if (--_branch <= 0) {
			doupdate();
			_branch = getSkip();
		}

		float out[2];

		for (int j = 0; j < partials; j++){

			float sig = osc[j]->next() * _amp;
			sig = sig / (partials * (j+1));
			out[0] += sig;

		}

		out[1] = 0;

		rtaddout(out);
		increment();
	}

	return framesToRun();
}


Instrument *makeSPECTRUM()
{
	SPECTRUM *inst = new SPECTRUM();
	inst->set_bus_config("SPECTRUM");

	return inst;
}


void rtprofile()
{
	RT_INTRO("SPECTRUM", makeSPECTRUM);
}


