//
//  PolyViewController.cpp
//  poly
//
//  Created by Matthew McGoogan on 12/1/14.
//  Copyright (c) 2014 Manic.io. All rights reserved.
//

#include "PolyViewController.h"





@implementation PolyViewController

- (id)initWithEd:(PolyEditor *)editor
{
	if ((self = [super initWithNibName:@"PolyViewController" bundle:[NSBundle bundleWithIdentifier:@"com.manic.poly"]])) {
		ed = editor;
		return self;
	}
	
	return nil;
}

- (IBAction)sliderDidMove:(id)sender
{
	NSSlider *con = (NSSlider *)sender;
	ed->updateStateFromUI((int)[con tag], [con floatValue]);
}

- (IBAction)waveControlDidChange:(id)sender
{
	NSSegmentedControl *con = (NSSegmentedControl *)sender;
	
	float val = (float)[con selectedSegment] / 3.0f;
	ed->updateStateFromUI((int)[con tag], val);
}

- (void)updateUIWithTag:(int)tag value:(float)val
{
	switch (tag) {
		case kWave1TypeParam: [self.wave1Control setSelectedSegment:(NSInteger)fminf(floorf(val*3.0f), 2.0f)]; break;
		case kWave2TypeParam: [self.wave2Control setSelectedSegment:(NSInteger)fminf(floorf(val*3.0f), 2.0f)]; break;
		case kWave3TypeParam: [self.wave3Control setSelectedSegment:(NSInteger)fminf(floorf(val*3.0f), 2.0f)]; break;
		case kWave1GainParam: [self.wave1GainSlider setFloatValue:val]; [self.wave1GainField setFloatValue:val]; break;
		case kWave2GainParam: [self.wave2GainSlider setFloatValue:val]; [self.wave2GainField setFloatValue:val]; break;
		case kWave3GainParam: [self.wave3GainSlider setFloatValue:val]; [self.wave3GainField setFloatValue:val]; break;
		case kAttackTargetParam: [self.attackTargetSlider setFloatValue:val]; [self.attackTargetField setFloatValue:val]; break;
		case kGainParam: [self.gainSlider setFloatValue:val]; [self.gainField setFloatValue:val]; break;
		case kDecayTargetParam: [self.decayTargetSlider setFloatValue:val]; [self.decayTargetField setFloatValue:val]; break;
		case kAttackLengthParam: [self.attackLengthSlider setFloatValue:val]; [self.attackLengthField setFloatValue:val]; break;
		case kDecayLengthParam: [self.decayLengthSlider setFloatValue:val]; [self.decayLengthField setFloatValue:val]; break;
		case kReleaseLengthParam: [self.releaseLengthSlider setFloatValue:val]; [self.releaseLengthField setFloatValue:val]; break;
		default:break;
	}
}

@end