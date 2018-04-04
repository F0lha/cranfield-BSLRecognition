#include "Classifier.h"
#include  "Signature.h"


Classifier::Classifier(Model* model)
{
	this->model = model;
}


Classifier::~Classifier()
{
}

void Classifier::onFrame(const Leap::Controller& controller) {
	const Leap::Frame frame = controller.frame();

    auto attributes = signatureStatic(frame.hands());
	cv::Mat predict = cv::Mat(1, attributes.size(), CV_32FC1, attributes.data());
	cv::Mat prediction = model->predict(predict);

	double min, max;

	cv::Point min_loc, max_loc;
	cv::minMaxLoc(prediction.row(0), &min, &max, &min_loc, &max_loc);
	std::cout << "Label: " <<  std::to_string(max_loc.x) << std::endl;
    //std::cout << "Label: " << std::to_string(prediction.at<int>(0, 0)) << std::endl;
}