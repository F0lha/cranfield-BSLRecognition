#include "Classifier.h"
#include  "Signature.h"


Classifier::Classifier(Model* model): BaseClassifier(model)
{
	
}


Classifier::~Classifier()
{
}

void Classifier::doSignatures(const Leap::Frame& frame) {
    auto attributes = signatureStatic(frame.hands());
	cv::Mat predict = cv::Mat(1, attributes.size(), CV_32FC1, attributes.data());
	auto labels = model->predict(predict);
    std::cout << "Label: " << std::to_string(labels.front()) << std::endl;
}