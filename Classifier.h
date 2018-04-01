#pragma once

#include "Leap.h"
#include <ml.h>

#include "Model.h"

class Classifier : public Leap::Listener
{
	Model* model;
	unsigned counter;
public:
	Classifier(Model* model);
	~Classifier();
	void onFrame(const Leap::Controller&);
};

