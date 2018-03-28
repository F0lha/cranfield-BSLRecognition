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

	std::cout << frame.hands().isEmpty();

	std::ofstream outputFile(gesture + "/" + std::to_string(frame_counter) + ".data");

	std::vector<float> atributes(2 * (5 + 1) * 3, -1.f);

	//for each hand
	for (int hand_n = 0; hand_n < frame.hands().count(); hand_n++)
	{
		Leap::Hand hand = frame.hands()[hand_n];

		bool right = hand.isRight();

		Leap::FingerList fingers = hand.fingers();

		//for each finger
		for (int finger_n = 0; finger_n < fingers.count(); finger_n++)
		{
			Leap::Finger finger = fingers[finger_n];

			int type = finger.type();

			//update type of finger
			atributes[right*(5 + 1) * 3 + type * 3 + 0] = finger.tipPosition().x;
			atributes[right*(5 + 1) * 3 + type * 3 + 1] = finger.tipPosition().y;
			atributes[right*(5 + 1) * 3 + type * 3 + 2] = finger.tipPosition().z;
		}
		//palm of hand
		atributes[right*(5 + 1) * 3 + 5 * 3 + 0] = hand.palmPosition().x;
		atributes[right*(5 + 1) * 3 + 5 * 3 + 1] = hand.palmPosition().y;
		atributes[right*(5 + 1) * 3 + 5 * 3 + 2] = hand.palmPosition().z;

	}

	std::string framestring = "";

	for (int i = 0; i < atributes.size(); i++)
	{
		framestring += std::to_string(atributes[i]) + ",";
	}
	
	outputFile << framestring;
	outputFile.close();

	this->frame_counter++;
}

void Recorder::setGesture(std::string gesture) {
	this->gesture = gesture;
}