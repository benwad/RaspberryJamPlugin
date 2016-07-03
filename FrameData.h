#pragma once

struct FrameData
{
	double left_phase;
	double right_phase;

	FrameData() : left_phase(0.0f), right_phase(0.0f) {};
	FrameData(double left_phase, double right_phase) : left_phase(left_phase), right_phase(right_phase) {}
	FrameData operator* (const double x) { return FrameData(left_phase * x, right_phase * x); }
	FrameData operator+ (const FrameData other) {
		return FrameData(
			left_phase + other.left_phase,
			right_phase + other.right_phase
		);
	}
	FrameData operator- (const FrameData other) {
		return FrameData(
			left_phase - other.left_phase,
			right_phase - other.right_phase
		);
	}
	FrameData operator+ (const double x) {
		return FrameData(
			left_phase + x,
			right_phase + x
		);
	}
	FrameData operator- (const double x) {
		return FrameData(
			left_phase - x,
			right_phase - x
		);
	}
	FrameData operator/ (const double x) { return FrameData(left_phase / x, right_phase / x); }
};
