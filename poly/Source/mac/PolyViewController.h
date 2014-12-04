//
//  PolyViewController.h
//  poly
//
//  Created by Matthew McGoogan on 12/1/14.
//  Copyright (c) 2014 Manic.io. All rights reserved.
//

#ifndef __poly__PolyEditorMac__
#define __poly__PolyEditorMac__

#include <stdio.h>
#include "PolyEditor.h"
#include <Cocoa/Cocoa.h>

struct Opaque;


@interface PolyViewController : NSViewController {
	PolyEditor *ed;
}

@property IBOutlet NSSlider *wave1GainSlider;
@property IBOutlet NSSlider *wave2GainSlider;
@property IBOutlet NSSlider *wave3GainSlider;
@property IBOutlet NSSlider *attackTargetSlider;
@property IBOutlet NSSlider *attackLengthSlider;
@property IBOutlet NSSlider *decayTargetSlider;
@property IBOutlet NSSlider *decayLengthSlider;
@property IBOutlet NSSlider *releaseLengthSlider;
@property IBOutlet NSSlider *gainSlider;
@property IBOutlet NSSegmentedControl *wave1Control;
@property IBOutlet NSSegmentedControl *wave2Control;
@property IBOutlet NSSegmentedControl *wave3Control;

@property IBOutlet NSTextField *wave1GainField;
@property IBOutlet NSTextField *wave2GainField;
@property IBOutlet NSTextField *wave3GainField;
@property IBOutlet NSTextField *attackTargetField;
@property IBOutlet NSTextField *attackLengthField;
@property IBOutlet NSTextField *decayTargetField;
@property IBOutlet NSTextField *decayLengthField;
@property IBOutlet NSTextField *releaseLengthField;
@property IBOutlet NSTextField *gainField;

- (id)initWithEd:(PolyEditor *)editor;
- (IBAction)sliderDidMove:(id)sender;
- (IBAction)waveControlDidChange:(id)sender;
- (void)updateUIWithTag:(int)tag value:(float)val;

@end


#endif /* defined(__poly__PolyEditorMac__) */
