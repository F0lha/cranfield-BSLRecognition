#pragma once

#include "Leap.h"
#include <array>

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

inline std::vector<float> signatureDynamic(const Leap::HandList& hands) {
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
        auto handSpeed = hand.palmVelocity();
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
            if (max < mag) {
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
                auto fli = fingerPos[0][i];
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
            if (v != invalidValue)
                v /= max;
        }
    }

    return attributes;
}