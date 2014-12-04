//
//  PolySynth.cpp
//  poly
//
//  Created by Matthew McGoogan on 11/30/14.
//  Copyright (c) 2014 Manic.io. All rights reserved.
//

#include "PolySynth.h"

const int kParamCount = 12;
static PolyEditor *ed;

static float freqTable[kNumFrequencies];

AudioEffect *createEffectInstance(audioMasterCallback amc)
{
	return new PolySynth(amc);
}


PolySynth::PolySynth(audioMasterCallback amc) : AudioEffectX(amc, 1, kParamCount)
{
	sampleIdx = 0;
	samples = nullptr;
	ed = new PolyEditor(this);
	
	adsr = new ADSREnvelope();
	
	waves = new Wave*[3];
	waves[0] = new SineWave(getSampleRate());
	waves[1] = new SineWave(getSampleRate());
	waves[2] = new SineWave(getSampleRate());
	
	// build frequency table
	double root = 1.059463094359; // 12th root of 2
	double a = 6.875;	// a in hz
	a *= root;			// a#
	a *= root;			// b
	a *= root;			// c
	for (int i=0 ; i<kNumFrequencies ; i++) {
		freqTable[i] = a;
		a *= root;
	}
}

PolySynth::~PolySynth()
{
	delete adsr; adsr = nullptr;
	for (int i=0 ; i<3 ; i++) {
		delete waves[i];
		waves[i] = nullptr;
	}
	delete waves; waves = nullptr;
}


void PolySynth::getParameterName(VstInt32 index, char *text)
{
	switch (index) {
		case kWave1TypeParam: vst_strncpy(text, "Wave 1", kVstMaxParamStrLen); break;
		case kWave2TypeParam: vst_strncpy(text, "Wave 2", kVstMaxParamStrLen); break;
		case kWave3TypeParam: vst_strncpy(text, "Wave 3", kVstMaxParamStrLen); break;
		case kWave1GainParam: vst_strncpy(text, "Wave1 Gn", kVstMaxParamStrLen); break;
		case kWave2GainParam: vst_strncpy(text, "Wave2 Gn", kVstMaxParamStrLen); break;
		case kWave3GainParam: vst_strncpy(text, "Wave3 Gn", kVstMaxParamStrLen); break;
		case kAttackTargetParam: vst_strncpy(text, "AttackTg", kVstMaxParamStrLen); break;
		case kAttackLengthParam: vst_strncpy(text, "AttackLn", kVstMaxParamStrLen); break;
		case kDecayTargetParam: vst_strncpy(text, "DecayTg", kVstMaxParamStrLen); break;
		case kDecayLengthParam: vst_strncpy(text, "DecayLn", kVstMaxParamStrLen); break;
		case kReleaseLengthParam: vst_strncpy(text, "Release", kVstMaxParamStrLen); break;
		case kGainParam: vst_strncpy(text, "Gain", kVstMaxParamStrLen); break;
		default:break;
	}
}

void PolySynth::getParameterLabel(VstInt32 index, char *label)
{
	switch (index) {
		case kWave1TypeParam:
		case kWave2TypeParam:
		case kWave3TypeParam: vst_strncpy(label, "Shape", kVstMaxParamStrLen); break;
		case kWave1GainParam:
		case kWave2GainParam:
		case kWave3GainParam:
		case kAttackTargetParam:
		case kGainParam:
		case kDecayTargetParam: vst_strncpy(label, "dB", kVstMaxParamStrLen); break;
		case kAttackLengthParam:
		case kDecayLengthParam:
		case kReleaseLengthParam: vst_strncpy(label, "sec", kVstMaxParamStrLen); break;
		default:break;
	}
}

void PolySynth::getParameterDisplay(VstInt32 index, char *text)
{
	switch (index) {
		case kWave1TypeParam: vst_strncpy(text, waves[0]->typeStr(), kVstMaxParamStrLen); break;
		case kWave2TypeParam: vst_strncpy(text, waves[1]->typeStr(), kVstMaxParamStrLen); break;
		case kWave3TypeParam: vst_strncpy(text, waves[2]->typeStr(), kVstMaxParamStrLen); break;
		case kWave1GainParam: float2string(waves[0]->amplitude(), text, kVstMaxParamStrLen); break;
		case kWave2GainParam: float2string(waves[1]->amplitude(), text, kVstMaxParamStrLen); break;
		case kWave3GainParam: float2string(waves[2]->amplitude(), text, kVstMaxParamStrLen); break;
		case kAttackTargetParam: float2string(adsr->attackTarget(), text, kVstMaxParamStrLen); break;
		case kGainParam: float2string(gain, text, kVstMaxParamStrLen); break;
		case kDecayTargetParam: float2string(adsr->decayTarget(), text, kVstMaxParamStrLen); break;
		case kAttackLengthParam: float2string(adsr->attackLength(), text, kVstMaxParamStrLen); break;
		case kDecayLengthParam: float2string(adsr->decayLength(), text, kVstMaxParamStrLen); break;
		case kReleaseLengthParam: float2string(adsr->releaseLength(), text, kVstMaxParamStrLen); break;
		default:break;
	}
}

void PolySynth::setParameter(VstInt32 index, float value)
{
	ed->updateEditor(index, value);
	
	switch (index) {
		case kWave1TypeParam:
			if (waves[0]) {
				if (Wave::waveTypeNumForValue(value) != waves[0]->typeNum()) {
					delete waves[0]; waves[0] = Wave::waveForValue(value); waves[0]->setSampleRate(getSampleRate()); break;
				}
			}
		case kWave2TypeParam:
			if (waves[1]) {
				if (Wave::waveTypeNumForValue(value) != waves[1]->typeNum()) {
					delete waves[1]; waves[1] = Wave::waveForValue(value); waves[1]->setSampleRate(getSampleRate()); break;
				}
			}
		case kWave3TypeParam:
			if (waves[2]) {
				if (Wave::waveTypeNumForValue(value) != waves[2]->typeNum()) {
					delete waves[2]; waves[2] = Wave::waveForValue(value); waves[2]->setSampleRate(getSampleRate()); break;
				}
			}
		case kWave1GainParam: if (waves[0]) waves[0]->setAmplitude(value); break;
		case kWave2GainParam: if (waves[1]) waves[1]->setAmplitude(value); break;
		case kWave3GainParam: if (waves[2]) waves[2]->setAmplitude(value); break;
		case kAttackTargetParam: adsr->setAttackTarget(value); break;
		case kGainParam: gain = value; break;
		case kDecayTargetParam: adsr->setDecayTarget(value); break;
		case kAttackLengthParam: adsr->setAttackLength(value * adsr->maxLength); break;
		case kDecayLengthParam: adsr->setDecayLength(value * adsr->maxLength); break;
		case kReleaseLengthParam: adsr->setRelease(value * adsr->maxLength); break;
		default:break;
	}
	return;
}

float PolySynth::getParameter(VstInt32 index)
{
	float value = 0.0f;
	
	switch (index) {
		case kWave1TypeParam: if (waves[0]) value=waves[0]->typeNum(); break;
		case kWave2TypeParam: if (waves[1]) value=waves[1]->typeNum(); break;
		case kWave3TypeParam: if (waves[2]) value=waves[2]->typeNum(); break;
		case kWave1GainParam: if (waves[0]) value=waves[0]->amplitude(); break;
		case kWave2GainParam: if (waves[1]) value=waves[1]->amplitude(); break;
		case kWave3GainParam: if (waves[2]) value=waves[2]->amplitude(); break;
		case kAttackTargetParam: value=adsr->attackTarget(); break;
		case kGainParam: value = gain; break;
		case kDecayTargetParam: value=adsr->decayTarget(); break;
		case kAttackLengthParam: value=adsr->attackLength()/adsr->maxLength; break;
		case kDecayLengthParam: value=adsr->decayLength()/adsr->maxLength; break;
		case kReleaseLengthParam: value=adsr->releaseLength()/adsr->maxLength; break;
		default:break;
	}
	
	return value;
}

bool PolySynth::getOutputProperties(VstInt32 index, VstPinProperties *properties)
{
	if (index < 2)
	{
		vst_strncpy (properties->label, "Poly ", 63);
		char temp[11] = {0};
		int2string (index + 1, temp, 10);
		vst_strncat (properties->label, temp, 63);
		
		properties->flags = kVstPinIsActive;
		if (index < 2)
			properties->flags |= kVstPinIsStereo;	// make channel 1+2 stereo
		return true;
	}
	return false;
}

bool PolySynth::getEffectName(char *name)
{
	vst_strncpy (name, "PolySynth", kVstMaxEffectNameLen);
	return true;
}

bool PolySynth::getVendorString(char *text)
{
	vst_strncpy (text, "Manic.io", kVstMaxVendorStrLen);
	return true;
}

bool PolySynth::getProductString(char *text)
{
	vst_strncpy (text, "Vst Poly Synth", kVstMaxProductStrLen);
	return true;
}

VstInt32 PolySynth::getVendorVersion()
{
	return 1000;
}

VstInt32 PolySynth::canDo(char *text)
{
	if (!strcmp (text, "receiveVstEvents"))
		return 1;
	if (!strcmp (text, "receiveVstMidiEvent"))
		return 1;
	
	return -1;	// explicitly can't do; 0 => don't know
}

VstInt32 PolySynth::getNumMidiInputChannels()
{
	return 1;
}

VstInt32 PolySynth::getNumMidiOutputChannels()
{
	return 0;
}


void PolySynth::updateFromUI(int index, float value)
{
	switch (index) {
		case kWave1TypeParam:
			if (waves[0]) {
				if (Wave::waveTypeNumForValue(value) != waves[0]->typeNum()) {
					delete waves[0]; waves[0] = Wave::waveForValue(value); waves[0]->setSampleRate(getSampleRate()); break;
				}
			}
		case kWave2TypeParam:
			if (waves[1]) {
				if (Wave::waveTypeNumForValue(value) != waves[1]->typeNum()) {
					delete waves[1]; waves[1] = Wave::waveForValue(value); waves[1]->setSampleRate(getSampleRate()); break;
				}
			}
		case kWave3TypeParam:
			if (waves[2]) {
				if (Wave::waveTypeNumForValue(value) != waves[2]->typeNum()) {
					delete waves[2]; waves[2] = Wave::waveForValue(value); waves[2]->setSampleRate(getSampleRate()); break;
				}
			}
		case kWave1GainParam: if (waves[0]) waves[0]->setAmplitude(value); break;
		case kWave2GainParam: if (waves[1]) waves[1]->setAmplitude(value); break;
		case kWave3GainParam: if (waves[2]) waves[2]->setAmplitude(value); break;
		case kAttackTargetParam: adsr->setAttackTarget(value); break;
		case kGainParam: gain = value; break;
		case kDecayTargetParam: adsr->setDecayTarget(value); break;
		case kAttackLengthParam: adsr->setAttackLength(value * adsr->maxLength); break;
		case kDecayLengthParam: adsr->setDecayLength(value * adsr->maxLength); break;
		case kReleaseLengthParam: adsr->setRelease(value * adsr->maxLength); break;
		default:break;
	}
	return;
}

void PolySynth::setSampleRate(float sampleRate)
{
	AudioEffectX::setSampleRate(sampleRate);
	
	waves[0]->setSampleRate(sampleRate);
	waves[1]->setSampleRate(sampleRate);
	waves[2]->setSampleRate(sampleRate);
}

void PolySynth::setBlockSize(VstInt32 blockSize)
{
	AudioEffectX::setBlockSize(blockSize);
	
	if (samples) {
		delete samples;
		samples = nullptr;
	}
	
	adsr->setSampleRate((float)blockSize);
	
	sampleIdx = 0;
	samples = new float[blockSize];
	for (int i=0 ; i<blockSize ; i++) {
		samples[i] = 0.0f;
	}
}

bool isIdle (MidiWave mw) { return (mw.env->state()==IdleState); }

void PolySynth::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames)
{
	float *out1 = outputs[0];
	float *out2 = outputs[1];
	float s1, s2, s3, sample = 0.0f, freq=0.0f;
	int count = 0;
	
	mWaves.erase(std::remove_if(mWaves.begin(), mWaves.end(), isIdle), mWaves.end());
	
	if (sampleIdx >= getBlockSize()) {
		sampleIdx = 0;
	}
	
	for (int i=0 ; i<sampleFrames ; i++) {
		samples[sampleIdx] = 0.0f;
		
		samples[sampleIdx] = (int)mWaves.size()==0 ? samples[sampleIdx] : 0.0f;
		sample = samples[sampleIdx];
		
		for (auto &mNote : mWaves) {
			if (i==0) {
				count++;
			}
			
			mNote.env->setSampleRate(adsr->sampleRate());
			mNote.env->setAttackLength(adsr->attackLength());
			mNote.env->setAttackTarget(adsr->attackTarget());
			mNote.env->setDecayLength(adsr->decayLength());
			mNote.env->setDecayTarget(adsr->decayTarget());
			mNote.env->setRelease(adsr->releaseLength());
			
			freq = freqTable[mNote.note()];
			
			std::tie(s1, mNote.phase[0]) = waves[0]->process(freq, mNote.phase[0]);
			std::tie(s2, mNote.phase[1]) = waves[1]->process(freq, mNote.phase[1]);
			std::tie(s3, mNote.phase[2]) = waves[2]->process(freq, mNote.phase[2]);
			sample += ((s1 + s2 + s3) / 3.0f) * (mNote.velocity() / 128.0f);
			sample = mNote.env->process(sample);
		}
		
		sample = sample / 10.0f * gain;
		samples[sampleIdx++] = sample;
		
		(*out1++) = sample;
		(*out2++) = sample;
	}
}

VstInt32 PolySynth::processEvents(VstEvents *events)
{
	float prevAmp = 0.0f;
	for (int i=0; i<events->numEvents ; i++) {
		bool exists = false;
		if (events->events[i]->type != kVstMidiType) {
			continue;
		}
		
		VstMidiEvent *midi = (VstMidiEvent *)events->events[i];
		
		// Zero the channel, not observing channels
		VstInt32 status = midi->midiData[0] & 0xf0;
		
		// Clamp values at max of 127 (0x7f)
		VstInt32 note = midi->midiData[1] & 0x7f;
		VstInt32 vel = midi->midiData[2] & 0x7f;
		
		// Midi off signal
		if (status == 0x80 || (status == 0x90 && vel == 0)) {
			for (int i=0 ; i<mWaves.size() ; i++) {
				if (mWaves.at(i).note() == note) {
					mWaves.at(i).env->beginRelease();
				}
			}
		}
		else if (status == 0x90) {
			for (auto &mw : mWaves) {
				if (mw.note() == note) {
					mw.env->beginAttack();
					exists = true;
				}
				prevAmp = mw.env->amplitude();
			}
			if (!exists) {
				MidiWave newmw = MidiWave(note, vel);
				newmw.env->setAmplitude(prevAmp);
				newmw.env->beginAttack();
				mWaves.push_back(newmw);
			}
		}
	}
	return 1;
}











