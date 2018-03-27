#pragma once
#define _CRT_SECURE_NO_DEPRECATE //due to errors in visual studio tools

#ifndef _Recorder_H_INCLUDED__
#define __Recorder_H_INCLUDED__

#include <fstream>
#include "Leap.h"


class Recorder : public Leap::Listener
{
	int frame_counter = 0;
	std::string gesture;

public:
	Recorder();
	~Recorder();

	virtual void onFrame(const Leap::Controller&);

	void setGesture(std::string gesture);
};

#endif