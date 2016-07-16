#include "LFO.h"

#include "Oscillator.h"

#include <math.h>
#include <iostream>

void LFO::SetWavelength(double wavelength)
{
    std::cout << "Wavelength " << wavelength << std::endl;
	this->phaseIncrement = wavelength / SAMPLE_RATE;
}

void LFO::SetIntensity(double intensity)
{
    std::cout << "Depth " << intensity << std::endl;
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
