#include "DynamicClassifier.h"
#include  "Signature.h"


DynamicClassifier::DynamicClassifier(Model* model) : BaseClassifier(model)
{

}


DynamicClassifier::~DynamicClassifier()
{
}

void DynamicClassifier::doSignatures(const Leap::Frame& frame) {
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
            return;
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
            std::cout << "Label: " << std::to_string(labels.front()) << std::endl;
            signatures.clear();
        }
    }
}

