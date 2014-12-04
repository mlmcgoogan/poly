//
//  ADSREnvelope.h
//  poly
//
//  Created by Matthew McGoogan on 11/30/14.
//  Copyright (c) 2014 Manic.io. All rights reserved.
//

#ifndef __poly__ADSREnvelope__
#define __poly__ADSREnvelope__

#include <stdio.h>


typedef enum : int {
	IdleState,
	AttackState,
	DecayState,
	SustainState,
	ReleaseState
} ADSRState;


class ADSREnvelope
{
public:
	ADSREnvelope();
	~ADSREnvelope();
	
	void setAttackLength(float length);
	void setAttackTarget(float target);
	void setDecayLength(float length);
	void setDecayTarget(float target);
	void setRelease(float length);
	float attackLength();
	float attackTarget();
	float decayLength();
	float decayTarget();
	float releaseLength();
	void setAmplitude(float amp);
	float amplitude();
	
	void setSampleRate(float rate);
	float sampleRate();
	
	ADSRState state();
	
	const float maxLength = 5.0f;
	
	void reset();
	void beginAttack();
	void beginRelease();
	float process(float sample);
	
protected:
	float _sampleRate;
	float _attackLength, _attackTarget, _decayLength, _decayTarget, _releaseLength, _releaseCutoff;

private:
	ADSRState _state;
	float _amplitude;
};

#endif /* defined(__poly__ADSREnvelope__) */
