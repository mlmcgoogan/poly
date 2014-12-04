//
//  PolyEditor.h
//  poly
//
//  Created by Matthew McGoogan on 11/30/14.
//  Copyright (c) 2014 Manic.io. All rights reserved.
//

#ifndef __poly__PolyEditor__
#define __poly__PolyEditor__

#include <stdio.h>
#include "audioeffectx.h"
#include "aeffeditor.h"
#include "PolySynth.h"

class PolyEditor : public AEffEditor
{
public:
	PolyEditor(AudioEffect *effect);
	~PolyEditor();
	
	bool getRect(ERect **rect);
	bool open(void *ptr);
	void close();
	
	void updateEditor(int tag, float value);
	void updateStateFromUI(int tag, float value);
	
protected:
	
	ERect editorBounds;
};

#endif /* defined(__poly__PolyEditor__) */
