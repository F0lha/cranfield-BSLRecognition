#include "Decoder.h"
#include <filesystem>
#include <sstream>

Decoder::Decoder()
{}

Decoder::~Decoder()
{}

cv::Mat Decoder::decode(std::string gesture) {
    namespace fs = std::experimental::filesystem;
    std::vector<std::vector<float>> features;
    for (const auto& p : fs::directory_iterator(gesture)) {
        if (fs::is_regular_file(p.path())) {
            std::ifstream file(p.path());
            std::vector<float> attributes;
            std::string l;
            while (std::getline(file, l)) {
                std::stringstream ss;
                ss << l;
                while(ss) {
                    float v{};
                    char  c{};
                    ss >> v >> c;
                    if(ss.good())
                        attributes.push_back(v);
                }
            }
            features.push_back(attributes);
        }
    }

    if (features.empty())
        return cv::Mat();

    cv::Mat m(features.size(), features[0].size(), CV_32FC1);
    for (int i = 0; i < static_cast<int>(features.size()); ++i) {
        for (int j = 0; j < static_cast<int>(features[i].size()); ++j) {
            m.at<float>(i, j) = features[i][j];
        }
    }

    return m;
}