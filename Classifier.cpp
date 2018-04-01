#include "Classifier.h"



Classifier::Classifier(Model* model)
{
	this->model = model;
	this->counter = 0;
}


Classifier::~Classifier()
{
}

void Classifier::onFrame(const Leap::Controller& controller) {

	if (this->counter < 5){
		this->counter++;
		return;
	}
	this->counter = 0;

	const Leap::Frame frame = controller.frame();

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

	cv::Mat predict = cv::Mat(1, atributes.size(), CV_32FC1);

	for (int i = 0; i < atributes.size(); i++)
		predict.at<float>(0, i) = atributes[i];

	cv::Mat prediction = this->model->predict(predict);

	double min, max;

	cv::Point min_loc, max_loc;
	cv::minMaxLoc(prediction.row(0), &min, &max, &min_loc, &max_loc);
	std::cout << "Label: " <<  std::to_string(max_loc.x) << std::endl;
}