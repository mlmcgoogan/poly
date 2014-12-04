//
//  Wave.cpp
//  poly
//
//  Created by Matthew McGoogan on 11/30/14.
//  Copyright (c) 2014 Manic.io. All rights reserved.
//

#include "Wave.h"


Wave::Wave(float sampleRate)
{
	_amplitude = 1.0f;
	_sampleRate = sampleRate;
}

Wave::~Wave()
{
	
}

void Wave::setSampleRate(float rate)
{
	_sampleRate = rate;
}

float Wave::sampleRate()
{
	return _sampleRate;
}

void Wave::setAmplitude(float amp)
{
	_amplitude = amp;
}

float Wave::amplitude()
{
	return _amplitude;
}


Wave *Wave::waveForValue(float val)
{
	if (val <= kSineThresh) {
		return new SineWave(0.0f);
	}
	else if (val <= kTriangleThresh) {
		return new TriangleWave(0.0f);
	}
	else {
		return new SawWave(0.0f);
	}
}

float Wave::waveTypeNumForValue(float val)
{
	if (val <= kSineThresh) {
		return kSineThresh;
	}
	else if (val <= kTriangleThresh) {
		return kTriangleThresh;
	}
	else {
		return kSawThresh;
	}
}


f2tuple SineWave::process(float freq, float phase)
{
	float p;
	float sample = sinf(2.0f * M_PI * phase) * _amplitude;
	
	p = phase + freq / _sampleRate;
	if (p > 1.0f) {
		p -= 2.0f;
	}
	
	return f2tuple(sample, p);
}

float SineWave::typeNum()
{
	return kSineThresh;
}

const char *SineWave::typeStr()
{
	return "Sine";
}

f2tuple TriangleWave::process(float freq, float phase)
{
	float p;
	float sample = (1.0f - fabsf(fmodf(phase, 2.0f) - 1.0f)) * 2.0f - 1.0f;
	
	p = phase + freq / _sampleRate;
	if (p > 1.0f) {
		p -= 2.0f;
	}
	
	return f2tuple(sample, p);
}

float TriangleWave::typeNum()
{
	return kTriangleThresh;
}

const char *TriangleWave::typeStr()
{
	return "Triangle";
}


f2tuple SawWave::process(float freq, float phase)
{
	float p;
	float sample = (phase - floorf(phase)) * 2.0f - 1.0f;
	
	p = phase + freq / _sampleRate;
	if (p > 1.0f) {
		p -= 2.0f;
	}
	
	return f2tuple(sample, p);
}

float SawWave::typeNum()
{
	return kSawThresh;
}

const char *SawWave::typeStr()
{
	return "Saw";
}