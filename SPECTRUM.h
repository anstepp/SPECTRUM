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
	double *onsetArray;

private:
	void doupdate();
	double * getDetuneArray(double *, int, int);

	int *lastOnsetState;

	struct State{
		enum playState { playing, notPlaying, starting };
	};

	int _branch;
	int partials, tablelen;
	float _amp, _freq, _pan;
	double *wavetable;

	Ooscili **osc;
	Ooscili **theDetuners;
	Orand *theRand;
	Ooscili *onsetTimes;

};

enum{
	kAmp = 1 << 2,
	kFreq = 1 << 3,
	kPan = 1 << 8,
};

