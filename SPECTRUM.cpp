/* SPECTRUM - simple additive synthesis instrument


*/
#include <stdio.h>
#include <stdlib.h>
#include <ugens.h>
#include "SPECTRUM.h"			  // declarations for this instrument class
#include <rt.h>
#include <rtdefs.h>
#include <stdlib.h>
#include <Instrument.h>
#include <ugens.h>
#include <Ougens.h>



SPECTRUM::SPECTRUM()
	: _branch(0)
{
}


SPECTRUM::~SPECTRUM()
{
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

	_amp = p[3];

	float freqraw = p[4];
	float freq;
	if (freqraw < 15.0)
		freq = cpspch(freqraw);
	else
		freq = freqraw;

	partials = p[5];

	Ooscili *osc[partials];

	//create sine wave
	const double twopi = M_PI * 2;
	tablelen = 1000;
	int wavetable[tablelen];
	for (int i = 0; i < tablelen; i++){
		wavetable[i] = sin(twopi * ((double) i / tablelen));
	}

	for (int i = 0; i < partials; i++){
		*osc[i] = Ooscili(SR, freq, *wavetable);
	}

	return nSamps();
}


int SPECTRUM::configure()
{
	return 0;	// IMPORTANT: Return 0 on success, and -1 on failure.
}


// Called at the control rate to update parameters like amplitude, pan, etc.

void SPECTRUM::doupdate()
{

}


int SPECTRUM::run()
{
	for (int i = 0; i < framesToRun(); i++) {

		if (--_branch <= 0) {
			doupdate();
			_branch = getSkip();
		}

		float out[1];

		for (int j = 0; j < partials; j++){

			out [0] += osc[j]->next() * _amp;

		}

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


