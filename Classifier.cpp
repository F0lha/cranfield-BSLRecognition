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
	auto labels = model->predict(predict);
    std::cout << "Label: " << std::to_string(labels.front()) << std::endl;
}