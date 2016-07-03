#pragma once

class ADSR {
	enum State {
		Off,
		Attack,
		Decay,
		Sustain,
		Release
	};

	private:
		double attackSpeed;
		double decaySpeed;
		double sustain;
		double releaseSpeed;

		double currentLevel;

		State state;

	public:
		ADSR();
		void SetParams(
			unsigned int attackTime,
			unsigned int decayTime,
			double sustain,
			unsigned int releaseTime
		);
		double NextFrame();
		void NoteOn();
		void NoteOff();
		bool IsActive();
};
