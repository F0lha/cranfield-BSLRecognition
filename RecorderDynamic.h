#pragma once
#define _CRT_SECURE_NO_DEPRECATE //due to errors in visual studio tools

#ifndef _Recorder_H_INCLUDED__
#define __Recorder_H_INCLUDED__

#include <fstream>
#include "Leap.h"


class RecorderDynamic : public Leap::Listener
{
    int frame_counter = 0;
    std::string gesture;
    std::vector<std::vector<float>> signatures;

public:
    RecorderDynamic();
    ~RecorderDynamic();

    virtual void onFrame(const Leap::Controller&);

    void setGesture(std::string gesture);

    void writeSignatures(std::string number);
    
};

#endif#pragma once
