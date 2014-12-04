//
//  ADSREnvelope.cpp
//  poly
//
//  Created by Matthew McGoogan on 11/30/14.
//  Copyright (c) 2014 Manic.io. All rights reserved.
//

#include "ADSREnvelope.h"


ADSREnvelope::ADSREnvelope()
{
	_state = IdleState;
	_amplitude = 0.0f;
	_sampleRate = 44100.0f;
	_attackLength = 0.0f * _sampleRate;
	_attackTarget = 1.0f;
	_decayLength = 0.0f * _sampleRate;
	_decayTarget = 1.0f;
	_releaseLength = 0.0f * _sampleRate;
	_releaseCutoff = 0.0000001f;
}

ADSREnvelope::~ADSREnvelope()
{
	
}

void ADSREnvelope::setAttackLength(float length)
{
	_attackLength = length * _sampleRate;
}

void ADSREnvelope::setAttackTarget(float target)
{
	_attackTarget = target;
}

void ADSREnvelope::setDecayLength(float length)
{
	_decayLength = length * _sampleRate;
}

void ADSREnvelope::setDecayTarget(float target)
{
	_decayTarget = target;
}

void ADSREnvelope::setAmplitude(float amp)
{
	_amplitude = amp;
}

float ADSREnvelope::attackLength()
{
	return _attackLength;
}

float ADSREnvelope::attackTarget()
{
	return _attackTarget;
}

float ADSREnvelope::decayLength()
{
	return _decayLength;
}

float ADSREnvelope::decayTarget()
{
	return _decayTarget;
}

float ADSREnvelope::releaseLength()
{
	return _releaseLength;
}

float ADSREnvelope::amplitude()
{
	return _amplitude;
}

void ADSREnvelope::setRelease(float length)
{
	_releaseLength = length * _sampleRate;
}

void ADSREnvelope::setSampleRate(float rate)
{
	_sampleRate = rate;
}

float ADSREnvelope::sampleRate()
{
	return _sampleRate;
}

void ADSREnvelope::reset()
{
	_state = IdleState;
	_amplitude = 0.0f;
}

void ADSREnvelope::beginAttack()
{
	_state = AttackState;
}

void ADSREnvelope::beginRelease()
{
	if (_state != IdleState) {
		_state = ReleaseState;
	}
}

ADSRState ADSREnvelope::state()
{
	return _state;
}

float ADSREnvelope::process(float sample)
{
	sample *= _amplitude;
	
	switch (_state) {
		case IdleState:
			break;
		
		case SustainState:
			break;
			
		case AttackState:
			if (_attackLength == 0.0f) {
				_amplitude = _attackTarget;
			}
			else {
				_amplitude += _attackTarget / _attackLength;
			}
			if (_amplitude >= _attackTarget) {
				_amplitude = _attackTarget;
				_state = DecayState;
			}
			break;
			
		case DecayState:
			if (_decayLength == 0.0f) {
				_amplitude = _decayTarget;
			}
			else {
				_amplitude -= (_attackTarget - _decayTarget) / _decayLength;
			}
			if (_amplitude <= _decayTarget) {
				_amplitude = _decayTarget;
				_state = SustainState;
			}
			break;
			
		case ReleaseState:
			if (_releaseLength == 0.0f) {
				_amplitude = 0.0f;
			}
			else {
				_amplitude -= (_decayTarget - _releaseCutoff) / _releaseLength;
			}
			if (_amplitude <= _releaseCutoff) {
				_amplitude = 0.0f;
				_state = IdleState;
			}
			break;
			
		default:
			break;
	}
	
	return sample;
}