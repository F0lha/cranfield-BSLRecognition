#include "DynamicClassifier.h"
#include  "Signature.h"


DynamicClassifier::DynamicClassifier(Model* modeldynamic)
{
    model = modeldynamic;
}


DynamicClassifier::~DynamicClassifier()
{
}

void DynamicClassifier::onFrame(const Leap::Controller& controller) {
    const Leap::Frame frame = controller.frame();
    auto mag = 0.0f;
    for (const auto hand : frame.hands()) {
        if (mag < hand.palmVelocity().magnitude()) {
            mag = hand.palmVelocity().magnitude();
        }
    }
	bool stop = false;

    if (mag > 50 && frame.hands().count() == 2) {
        signatures.push_back(signatureDynamic(frame));
    }
    else {
        if (signatures.size() < 50) {
            signatures.clear();
        }
        else {
            auto pcaSignatures = medianFiltering(signatures,10);
            if (pcaSignatures.empty()) {
                return;
            }

            const auto nbCols = pcaSignatures[0].size();
            if (nbCols < 5) {
                return;
            }

            const auto nbRows = pcaSignatures.size();
            cv::Mat predict(1, nbCols*nbRows, CV_32FC1);
            for (int i = 0; i < nbRows; ++i) {
                for (int j = 0; j < nbCols; ++j) {
                    predict.at<float>(0, i*nbCols + j) = pcaSignatures[i][j];
                }
            }
            auto labels = model->predict(predict);
            if (labels.front() == 0) {
                std::cout << "sign: desk" << std::endl;
                signatures.clear();
                return;
            }
            if (labels.front() == 1) {
                std::cout << "sign: earth/ball" << std::endl;
                signatures.clear();
                return;
            }

            if (labels.front() == 2) {
                signatures.clear();
                return;
            }
            signatures.clear();
        }
    }
}

