#include <Instrument.h>		  // the base class for this instrument
#include <stdlib.h>
#include <Instrument.h>
#include <ugens.h>
#include <Ougens.h>
#include <rt.h>
#include <rtdefs.h>

class SPECTRUM : public Instrument {

public:
	SPECTRUM();
	virtual ~SPECTRUM();
	virtual int init(double *, int);
	virtual int configure();
	virtual int run();

	double *detunetable; 
	float *freqarray;
	double *returnArray;

private:
	void doupdate();
	double * getDetuneArray(double *, int, int);

	int _branch;
	int partials, tablelen;
	float _amp, freq;
	double *wavetable;

	Ooscili **osc;
	Ooscili **theDetuners;
	Orand *theRand;

};

