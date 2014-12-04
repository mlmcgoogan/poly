//
//  PolySynth.h
//  poly
//
//  Created by Matthew McGoogan on 11/30/14.
//  Copyright (c) 2014 Manic.io. All rights reserved.
//

#ifndef __poly__PolySynth__
#define __poly__PolySynth__

#include <stdio.h>
#include <vector>
#include "audioeffectx.h"
#include "Wave.h"
#include "ADSREnvelope.h"
#include "PolyEditor.h"
#include "MidiWave.h"

typedef std::vector<MidiWave> vecMw;

enum : VstInt32 {
	kWave1TypeParam,
	kWave2TypeParam,
	kWave3TypeParam,
	kWave1GainParam,
	kWave2GainParam,
	kWave3GainParam,
	kAttackTargetParam,
	kAttackLengthParam,
	kDecayTargetParam,
	kDecayLengthParam,
	kReleaseLengthParam,
	kGainParam
};

enum {
	kNumFrequencies = 128,
	kMaxMidiNotes = 10
};

class PolySynth : public AudioEffectX
{
public:
	PolySynth(audioMasterCallback amc);
	~PolySynth();
	
	virtual void setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterName (VstInt32 index, char* text);
	
	virtual bool getOutputProperties (VstInt32 index, VstPinProperties* properties);
	
	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion ();
	virtual VstInt32 canDo (char* text);
	
	virtual VstInt32 getNumMidiInputChannels ();
	virtual VstInt32 getNumMidiOutputChannels ();
	
	void updateFromUI(int index, float value);
	
	void setSampleRate(float sampleRate);
	void setBlockSize(VstInt32 blockSize);
	
	void processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames);
	VstInt32 processEvents(VstEvents *events);
	
protected:
	float gain;
	vecMw mWaves;
	Wave **waves;
	ADSREnvelope *adsr;
	
private:
	float *samples;
	int sampleIdx;
};

#endif /* defined(__poly__PolySynth__) */
