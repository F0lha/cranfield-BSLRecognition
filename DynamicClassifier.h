#pragma once

#include "Leap.h"
#include <ml.h>

#include "BaseClassifier.h"

class DynamicClassifier : public BaseClassifier
{
    std::vector<std::vector<float>> signatures;
    void doSignatures(const Leap::Frame& frame) override;

public:
    DynamicClassifier(Model* model);
    ~DynamicClassifier();
};

