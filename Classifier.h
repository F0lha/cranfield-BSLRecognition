#pragma once

#include "Leap.h"
#include <ml.h>

#include "BaseClassifier.h"

class Classifier : public BaseClassifier
{
    void doSignatures(const Leap::Frame& frame) override;

public:
	Classifier(Model* model);
	~Classifier();
};

