//
//  MidiWave.h
//  poly
//
//  Created by Matthew McGoogan on 12/2/14.
//  Copyright (c) 2014 Manic.io. All rights reserved.
//

#ifndef __poly__MidiWave__
#define __poly__MidiWave__

#include <stdio.h>
#include "audioeffectx.h"
#include "Wave.h"
#include "ADSREnvelope.h"

struct MidiWave {
	
public:
	MidiWave(VstInt32 note, VstInt32 vel);
	~MidiWave();
	
	float phase[3];
	
	VstInt32 note();
	VstInt32 velocity();
	void stop();
	bool readyForRemoval();
	
	ADSREnvelope *env;
	
protected:
	VstInt32 _note;
	VstInt32 _vel;
	bool _removalFlag;
};

#endif /* defined(__poly__MidiWave__) */
