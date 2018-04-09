#pragma once

#include "Leap.h"
#include <array>
#include <core.hpp>

//Compute signature for hands capture w/ leap motion detector.
//these attributes are not invariant in space.
//inline std::vector<float> signature(const Leap::HandList& hands) {
//    //for each hand
//    std::vector<float> attributes(2 * (5 + 1) * 3, -1.f);
//    for(const auto& hand: hands) {
//        //for each finger
//        bool right = hand.isRight();
//        for(const auto& finger: hand.fingers()) {
//            int type = finger.type();
//            //update type of finger
//            attributes[right*(5 + 1) * 3 + type * 3 + 0] = finger.tipPosition().x;
//            attributes[right*(5 + 1) * 3 + type * 3 + 1] = finger.tipPosition().y;
//            attributes[right*(5 + 1) * 3 + type * 3 + 2] = finger.tipPosition().z;
//        }
//        //palm of hand
//        attributes[right*(5 + 1) * 3 + 5 * 3 + 0] = hand.palmPosition().x;
//        attributes[right*(5 + 1) * 3 + 5 * 3 + 1] = hand.palmPosition().y;
//        attributes[right*(5 + 1) * 3 + 5 * 3 + 2] = hand.palmPosition().z;
//    }
//    return attributes;
//}

inline std::vector<float> signatureStatic(const Leap::HandList& hands) {
    //for each hand
    using sf = std::vector<float>;
    using sv = std::vector<Leap::Vector>;

    const float invalidValue = 1E6;

    sf attributes(3 * (5 + 10), invalidValue);
    if (hands.count() > 2)
        return attributes;

    std::array<sv, 2> fingerPos;
    float             max{};
    for (const auto& hand : hands) {
        auto handPose = hand.palmPosition();
        //for each finger
        auto hIndex = hand.isRight() ? 0 : 1;
        for (const auto& finger : hand.fingers()) {
            //update type of finger
            auto type = finger.type();
            fingerPos[hIndex].push_back(finger.tipPosition());
            auto fh = fingerPos[hIndex].back() - handPose;
            attributes[hIndex * 5 * 3 + type * 3 + 0] = fh.x;
            attributes[hIndex * 5 * 3 + type * 3 + 1] = fh.y;
            attributes[hIndex * 5 * 3 + type * 3 + 2] = fh.z;
            auto mag = fh.magnitude();
            if( max < mag ) {
                max = mag;
            }
        }
    }

    if (hands.count() == 2) {
        auto iter = attributes.begin() + 30;
        float mag = 0.0;
        for (size_t i = 0; i != 5; ++i) {
            if ((!fingerPos[0].empty()) && (!fingerPos[1].empty())) {
                auto fri = fingerPos[0][i];
                auto fli = fingerPos[1][i];
                auto frli = fri - fli;
                *iter++ = frli[0];
                *iter++ = frli[1];
                *iter++ = frli[2];
                mag = frli.magnitude();
            }
            if (max < mag) {
                max = mag;
            }
        }
    }

    if (max > 0) {
        for (auto& v : attributes) {
            if( v != invalidValue )
                v /= max;
        }
    }

    return attributes;
}

//inline std::vector<float> signatureDynamic(const std::vector<Leap::Frame> frames, const int frameMaxCount) {
//    using sf = std::vector<float>;
//    const float invalidValue = 1E6;
//    int i = 0;
//    const auto count = frameMaxCount * 9;
//    sf attributes(count*2, invalidValue);
//    for (auto frame : frames) {
//        for (auto hand : frame.hands()) {
//            auto hIndex = hand.isRight() ? 0 : 1;
//            auto direction = (hand.palmVelocity())/(hand.palmVelocity().magnitude());
//            auto normal = hand.palmNormal();
//            auto plane = hand.direction();
//            attributes[hIndex * count + 0 + i * 9] = direction.x;
//            attributes[hIndex * count + 1 + i * 9] = direction.y;
//            attributes[hIndex * count + 2 + i * 9] = direction.z;
//            attributes[hIndex * count + 3 + i * 9] = normal.x;
//            attributes[hIndex * count + 4 + i * 9] = normal.y;
//            attributes[hIndex * count + 5 + i * 9] = normal.z;
//            attributes[hIndex * count + 6 + i * 9] = plane.x;
//            attributes[hIndex * count + 7 + i * 9] = plane.y;
//            attributes[hIndex * count + 8 + i * 9] = plane.z;
//        }
//        ++i;
//    }
//    return attributes;
//}

inline std::vector<float> signatureDynamic(Leap::Frame frame) {
    using sf = std::vector<float>;
    const float invalidValue = 1E6;
    int i = 0;
    const auto count = 3;
    sf attributes(6, invalidValue);
    for (auto hand : frame.hands()) {
        auto hIndex = hand.isRight() ? 0 : 1;
        auto direction = (hand.palmVelocity()) / (hand.palmVelocity().magnitude());
        auto normal = hand.palmNormal();
        auto plane = hand.direction();
        attributes[hIndex * count + 0] = direction.x;
        attributes[hIndex * count + 1] = direction.y;
        attributes[hIndex * count + 2] = direction.z;
        //attributes[hIndex * count + 3] = normal.x;
        //attributes[hIndex * count + 4] = normal.y;
        //attributes[hIndex * count + 5] = normal.z;
        //attributes[hIndex * count + 6] = plane.x;
        //attributes[hIndex * count + 7] = plane.y;
        //attributes[hIndex * count + 8] = plane.z;
    }
    return attributes;
}

inline auto pcaComputing(std::vector<std::vector<float>> signatures) {
    
    std::vector<std::vector<float>> pcaSignatures;
    const auto featureCount = signatures.front().size();
    const auto sampleCount = signatures.size();
    cv::Mat data(featureCount, sampleCount, CV_32F);
    for (size_t i = 0; i < featureCount; ++i) {
        for (size_t j = 0; j < sampleCount; ++j) {
            data.at<float>(i, j) = signatures[j][i];
        }
    }
    const int finalDim = 5;
    cv::PCA pca(data, cv::Mat(), 0, finalDim);
    for (size_t i = 0; i < featureCount; ++i) {
        cv::Mat sample(1, sampleCount, CV_32F);
        for (size_t j = 0; j < sampleCount; ++j) {
            sample.at<float>(0, j) = signatures[j][i];
        }
        auto pcaSample = pca.project(sample);
        cv::normalize(pcaSample, pcaSample);
        pcaSignatures.push_back(std::vector<float>(finalDim));
        for (size_t k = 0; k < finalDim; ++k) {
            pcaSignatures.back()[k] = pcaSample.at<float>(0, k);
        }
    }
    return pcaSignatures;
}