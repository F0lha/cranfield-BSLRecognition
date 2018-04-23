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
        attributes[hIndex * count + 0] = hand.palmPosition().x;
        attributes[hIndex * count + 1] = hand.palmPosition().y;
        attributes[hIndex * count + 2] = hand.palmPosition().z;
        //attributes[hIndex * count + 3] = normal.x;
        //attributes[hIndex * count + 4] = normal.y;
        //attributes[hIndex * count + 5] = normal.z;
        //attributes[hIndex * count + 6] = plane.x;
        //attributes[hIndex * count + 7] = plane.y;
        //attributes[hIndex * count + 8] = plane.z;
    }
    return attributes;
}

//inline std::vector<float> signatureDynamic(const Leap::Frame& frame) {
//    using sf = std::vector<float>;
//    sf attributes;
//    if (frame.hands().count() != 2){
//        return attributes;
//    }
//
//    const auto& hands = frame.hands();
//    if (hands[0].fingers().count() != 5 || hands[1].fingers().count() != 5) {
//        return attributes;
//    }
//
//    const auto& leftHand = hands[0].isRight() ? hands[1] : hands[0];
//    const auto& rightHand = hands[0].isRight() ? hands[0] : hands[1];
//
//    const auto& leftVelocity = leftHand.palmVelocity();
//    const auto& rightVelocity = rightHand.palmVelocity();
//    const auto  diffVelocity = leftVelocity - rightVelocity;
//    attributes.push_back(diffVelocity.x);
//    attributes.push_back(diffVelocity.y);
//    attributes.push_back(diffVelocity.z);
//
//    const auto& leftNormal = leftHand.palmNormal();
//    const auto& rightNormal = rightHand.palmNormal();
//    const auto angle = leftNormal.angleTo(rightNormal);
//    attributes.push_back(angle);
//
//    return attributes;
//}

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

    // Compute the median of a collection.
template<class Cont>
typename Cont::value_type
median(Cont& values, typename Cont::value_type default_value = 0)
{
    const size_t count = values.size();
    if( count > 1 ) {
      const size_t lower = (count - 1) / 2;
      const size_t upper = count / 2;
      std::partial_sort(values.begin(), values.begin() + upper + 1, values.end());
      return (values[lower] + values[upper]) / 2.0;
    } else if( count == 1 )
      return values[0];
    else
      return default_value;
}

 
// Filter our time Leap's signatures.
using vvec = std::vector<std::vector<float>>;
inline auto medianFiltering(vvec data, const size_t rows) {
  const auto total_rows = data.size();
  const auto wnd_size = total_rows / rows;
  const auto cols       = data[0].size();
  vvec filtered(rows, vvec::value_type(cols));
  for(size_t c = 0; c < cols; ++c) {
      size_t rr{};
    for(size_t o = 0; o < total_rows; o += wnd_size) {
        if (o + wnd_size > total_rows)
            break;
        std::vector<float> ft{};
        for(size_t r = o, size = o + wnd_size; r < size; ++r)
            ft.push_back(data[r][c]);
        if (rr >= filtered.size()) {
            return filtered;
        }
        filtered[rr++][c] = median(ft);
    }
  }
   return filtered;
}   

inline float min(float x, float y, float z) {
    if ((x <= y) && (x <= z)) return x;
    if ((y <= x) && (y <= z)) return y;
    if ((z <= x) && (z <= y)) return z;
}

inline float DTW (std::vector<float> v, std::vector<float> w) {
    int cost;
    vvec MG;
    size_t n = v.size() - 1;
    size_t m = w.size() - 1;

    for (int i = 1; i <= w.size(); i++) {
        MG[0][i] = std::numeric_limits<float>::infinity();
    }
    for (int i = 1; i <= v.size(); i++) {
        MG[i][0] = std::numeric_limits<float>::infinity();
    }
    MG[0][0] = 0;

    for (int i = 1; i <= v.size(); i++) {
        for (int j = 1; j <= w.size(); j++) {
            //	cost = abs( v[i] - w[j] ) * abs( v[i] - w[j] );
            if (v[i - 1] == w[j - 1]) {
                cost = 0;
                //	cout << v[i - 1] << " and " << w[j - 1] << endl;
            }
            else {
                cost = 1;
            }
            MG[i][j] = cost + min(MG[i - 1][j], MG[i][j - 1], MG[i - 1][j - 1]);
        }
    }
    return MG[n][m];
}