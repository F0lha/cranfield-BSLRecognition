#pragma once

#include "Leap.h"
#include <ml.h>

#include "Model.h"

class FinalClassifier :public Leap::Listener
{
    Model * model;
    Model * model2;
    std::vector<std::vector<float>> signatures;

public:
    FinalClassifier(Model* model, Model * model2);
    ~FinalClassifier();
    void onFrame(const Leap::Controller& controller);
};

