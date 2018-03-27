#include "Recorder.h"



Recorder::Recorder()
{
}


Recorder::~Recorder()
{
}

void Recorder::onFrame(const Leap::Controller& controller) {

	if (this->gesture.empty())
		std::cout << "No gesture was defined" << std::endl;

	// Get the most recent frame and report some basic information
	const Leap::Frame frame = controller.frame();

	std::ofstream outputFile(gesture + "/" + std::to_string(frame_counter) + ".data");
	const std::string serializedFrame = frame.serialize();

	outputFile << serializedFrame;
	outputFile.close();

	this->frame_counter++;
}

void Recorder::setGesture(std::string gesture) {
	this->gesture = gesture;
}