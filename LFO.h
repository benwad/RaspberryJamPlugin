#pragma once

class LFO {

	private:
		double phaseIncrement; // how much to move the phase per frame, defined by wavelength
		double intensity;
		double phase;

	public:
		LFO() : phaseIncrement(0.5f), intensity(1.0f), phase(0.0f) {}
		void SetWavelength(double wavelength);
		void SetIntensity(double intensity);
		void SetParams(
			double wavelength,
			double intensity
		);
		double NextFrame();
	
};
