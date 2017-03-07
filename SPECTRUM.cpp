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
#include <vector>


SPECTRUM::SPECTRUM()
	
{
	_branch = 0;
}


SPECTRUM::~SPECTRUM()
{
	delete [] wavetable;
	delete [] freqarray;
	delete [] osc;
	delete [] theDetuners;
	for (int i = 0; i < partials; i++){
		delete osc[i];
		delete theDetuners[i];
	}
}


double * SPECTRUM::getDetuneArray(double array[], int arrayLen, int partial)
{
	returnArray = new double [arrayLen];
	float val = theRand->rand();
	float mult = 1;
	if (val > 0) {
		mult = 0.5 * (partial + 1);
	} else {
		mult = -0.5 * (partial + 1);
	}
	float lastI = array[0];
	for (int i = 0; i < arrayLen; i++){
		returnArray[i] = array[i] * mult;
		//printf("%f\n", returnArray[i]);
		if (lastI == 0.0 && array[i] > 0.0 || lastI == 0.0 && array[i] < 0.0){
			float val = theRand->rand();
			if (val > 0) {
				mult = 0.5 * (partial + 1);
			} else {
				mult = -0.5 * (partial + 1);
			}
		}
		lastI = array[i];
	}
	return returnArray;
}

int SPECTRUM::init(double p[], int n_args)
{

	int seed = p[6];
	theRand = new Orand(seed);

	const float outskip = p[0];
	const float dur = p[1];

	if (rtsetoutput(outskip, dur, this) == -1)
		return DONT_SCHEDULE;

	if (outputChannels() > 2)
		return die("SPECTRUM", "Use mono or stereo output only.");

	_amp = p[2];

	float freqraw = p[3];
	if (freqraw < 15.0)
		freq = cpspch(freqraw);
	else
		freq = freqraw;

	partials = p[4];

	osc = new Ooscili * [partials];
	theDetuners = new Ooscili * [partials];

	//create sine wave
	tablelen = 1024;
	wavetable = new double [tablelen];
	const double twopi = M_PI * 2.0;
	for (int i = 0; i < tablelen; i++){
		wavetable[i] = sin(twopi * ((double) i / tablelen));
	}

	//table for detuning p[5]
	int wavelen;
	double *wavet = (double *) getPFieldTable(5, &wavelen);

	for (int i = 0; i < partials; i++){
		double *theArray = getDetuneArray(wavet, wavelen, i);
		theDetuners[i] = new Ooscili(SR, 1.0/dur, theArray, wavelen);
	}

	for (int i = 0; i < partials; i++){
		//make oscillator bank
		osc[i] = new Ooscili(SR, freq * (i + 1), wavetable, tablelen);
	}


	return nSamps();
}


int SPECTRUM::configure()
{
	return 0;
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

		float out[2];

		out[0] = 0;

		for (int j = 0; j < partials; j++){

			float detuneAmount = theDetuners[j]->next(currentFrame());
			//printf("%f\n", detuneAmount);
			float current_freq = freq + detuneAmount;
			osc[j]->setfreq(current_freq);
			float local_amp = (_amp / (j + 1)) / (partials / 3);
			float sig = osc[j]->next() * local_amp;
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


