//
//  MidiWave.cpp
//  poly
//
//  Created by Matthew McGoogan on 12/2/14.
//  Copyright (c) 2014 Manic.io. All rights reserved.
//

#include "MidiWave.h"
#include "PolySynth.h"

MidiWave::MidiWave(VstInt32 note, VstInt32 vel)
{
	_note = note;
	_vel = vel;
	env = new ADSREnvelope();
	
	for (int i=0;i<3;i++) {
		phase[i] = 0.0f;
	}
}

MidiWave::~MidiWave()
{

}

VstInt32 MidiWave::note()
{
	return _note;
}

VstInt32 MidiWave::velocity()
{
	return _vel;
}

void MidiWave::stop()
{
	
}