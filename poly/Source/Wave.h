//
//  Wave.h
//  poly
//
//  Created by Matthew McGoogan on 11/30/14.
//  Copyright (c) 2014 Manic.io. All rights reserved.
//

#ifndef __poly__Wave__
#define __poly__Wave__

#include <stdio.h>
#include <math.h>
#include <tuple>

typedef std::tuple<float, float> f2tuple;

const float kWaveCount = 3.0f;
const float kSineThresh = 0.33333f;
const float kTriangleThresh = 0.66666f;
const float kSawThresh = 0.99999f;

class Wave
{
public:
	Wave(float sampleRate);
	virtual ~Wave();
	
	virtual f2tuple process(float freq, float phase) = 0;
	void setSampleRate(float rate);
	float sampleRate();
	void setAmplitude(float amp);
	float amplitude();
	void reset();
	
	static Wave *waveForValue(float val);
	static float waveTypeNumForValue(float val);
	
	virtual float typeNum() = 0;
	virtual const char *typeStr() = 0;
	
protected:
	float _amplitude;
	float _sampleRate;
};


class SineWave : public Wave
{
public:
	SineWave(float sampleRate) : Wave(sampleRate) {};
	
	float typeNum();
	const char *typeStr();
	
	f2tuple process(float freq, float phase);
};


class TriangleWave : public Wave
{
public:
	TriangleWave(float sampleRate) : Wave(sampleRate) {};
	
	float typeNum();
	const char *typeStr();
	
	f2tuple process(float freq, float phase);
};


class SawWave : public Wave
{
public:
	SawWave(float sampleRate) : Wave(sampleRate) {};
	
	float typeNum();
	const char *typeStr();
	
	f2tuple process(float freq, float phase);
};


#endif /* defined(__poly__Wave__) */
