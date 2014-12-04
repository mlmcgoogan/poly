//
//  PolyEditor.cpp
//  poly
//
//  Created by Matthew McGoogan on 11/30/14.
//  Copyright (c) 2014 Manic.io. All rights reserved.
//

#include "PolyEditor.h"
#include "PolyViewController.h"



PolyEditor::PolyEditor(AudioEffect *effect) : AEffEditor(effect)
{
	editorBounds.top = 400.0f;
	editorBounds.left = 400.0f;
	editorBounds.right = 800.0f;
	editorBounds.bottom = 400.0f;
	
	effect->setEditor(this);
}

PolyEditor::~PolyEditor()
{
	
}


bool PolyEditor::getRect(ERect **rect)
{
	*rect = &editorBounds;
	return true;
}


static PolyViewController *controller;

bool PolyEditor::open(void *ptr)
{
	NSView *parent = (__bridge NSView*)ptr;
	
	controller = [[PolyViewController alloc] initWithEd:this];
	controller.view.frame = parent.bounds;
	[parent addSubview:controller.view];
	
	
	return true;
}

void PolyEditor::close()
{
	
}

void PolyEditor::updateEditor(int tag, float value)
{
	[controller updateUIWithTag:tag value:value];
}

void PolyEditor::updateStateFromUI(int tag, float value)
{
	static PolySynth *synth = (PolySynth *)effect;
	synth->updateFromUI(tag, value);
}