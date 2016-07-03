#pragma once

/*
	This is taken from "Butterworth Optimized C++ Class" by neotec on musicdsp.org
	http://musicdsp.org/showArchiveComment.php?ArchiveID=243
*/

struct FrameData;

class CFilterButterworth24db
{
public:
    CFilterButterworth24db(void);
    ~CFilterButterworth24db(void);
    void SetSampleRate(double fs);
    void Set(double cutoff, double q);
    double Run(double input);
    FrameData Run(FrameData input);

private:
    double t0, t1, t2, t3;
    double coef0, coef1, coef2, coef3;
    double history1, history2, history3, history4;
    double gain;
    double min_cutoff, max_cutoff;
	bool doStereo;
};
