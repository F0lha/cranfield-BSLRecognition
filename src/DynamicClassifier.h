#pragma once

#include "Leap.h"
#include <ml.h>

#include "Model.h"

class DynamicClassifier:public Leap::Listener
{
    Model * model;
    std::vector<std::vector<float>> signatures;

public:
    DynamicClassifier(Model* model);
    ~DynamicClassifier();
    void onFrame(const Leap::Controller& controller);
};

