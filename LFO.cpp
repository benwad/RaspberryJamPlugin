#include "LFO.h"

#include "Oscillator.h"

#include <math.h>

void LFO::SetWavelength(double wavelength)
{
	this->phaseIncrement = wavelength / SAMPLE_RATE;
}

void LFO::SetIntensity(double intensity)
{
	this->intensity = intensity;
}

void LFO::SetParams(double wavelength, double intensity)
{
	this->SetWavelength(wavelength);
	this->SetIntensity(intensity);
}

double LFO::NextFrame()
{
	this->phase += this->phaseIncrement;
	if (this->phase > TWO_PI) {
		this->phase -= TWO_PI;
	}

	return this->intensity * sin(this->phase);
}
