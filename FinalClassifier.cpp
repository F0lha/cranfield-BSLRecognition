#include "FinalClassifier.h"
#include  "Signature.h"


FinalClassifier::FinalClassifier(Model* modeldynamic, Model* modelstatic)
{

    model = modeldynamic;
    model2 = modelstatic;
}


FinalClassifier::~FinalClassifier()
{
}

void FinalClassifier::onFrame(const Leap::Controller& controller) {
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
            auto attributes = signatureStatic(frame.hands());
            cv::Mat predict = cv::Mat(1, attributes.size(), CV_32FC1, attributes.data());
            auto labels = model2->predict(predict);
            if (labels.front() == 0) {
                return;
            }
            if (labels.front() == 1) {
                std::cout << "sign: B letter" << std::endl;
                return;
            }

            if (labels.front() == 2) {
                std::cout << "sign: c letter" << std::endl;
                return;
            }
            if (labels.front() == 3) {
                std::cout << "sign: ghost" << std::endl;
                return;
            }
            if (labels.front() == 4) {
                std::cout << "sign: house" << std::endl;
                return;
            }
        }
        else {
            auto pcaSignatures = medianFiltering(signatures, 10);
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

