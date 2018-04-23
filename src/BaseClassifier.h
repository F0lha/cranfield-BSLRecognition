#pragma once

#include "Leap.h"
#include <ml.h>

#include "Model.h"

class BaseClassifier : public Leap::Listener
{
protected:
    Model* model;
    virtual void doSignatures(const Leap::Frame& frame) = 0;
public:
    BaseClassifier(Model* model): model{ model } {}
    virtual ~BaseClassifier() {}
    void onFrame(const Leap::Controller& controller) {
        const Leap::Frame frame = controller.frame();
        doSignatures(frame);
    }
};