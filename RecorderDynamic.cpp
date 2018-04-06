#include <filesystem>
#include "RecorderDynamic.h"
#include "Signature.h"

namespace fs = std::experimental::filesystem;

RecorderDynamic::RecorderDynamic()
{
}

RecorderDynamic::~RecorderDynamic()
{
}

void RecorderDynamic::onFrame(const Leap::Controller& controller) {
    // Get the most recent frame and report some basic information
    const Leap::Frame frame = controller.frame();
    auto mag = 0.0f;
    for (const auto hand : frame.hands()) {
        if (mag < hand.palmVelocity().magnitude()) {
            mag = hand.palmVelocity().magnitude();
        }
    }
    if (mag > 200) {
        this->Frames.push_back(frame);
    }
}

void RecorderDynamic::setAttribute(std::string number) {
    std::ofstream outputFile(gesture + "/"+ number + ".data");
    int                      frameCount    = this->Frames.size();
    const int                frameMaxCount = 50;
    int                      step = frameCount / frameMaxCount;
    std::vector<Leap::Frame> trainFrames;
    if (Frames.empty()) {
        std::cout << "no movement recorded" << std::endl;
        return;
    }
    for (int i = 0; i < frameMaxCount; ++i) {
        trainFrames.push_back(this->Frames[i*step]);
    }
    auto attributes = signatureDynamic(trainFrames);
    std::string framestring;
    for (const auto& v : attributes) {
        framestring += std::to_string(v) + ",";
    }
    outputFile << framestring;
    outputFile.close();
}

void RecorderDynamic::setGesture(std::string gesture) {
    this->gesture = gesture;
    if (fs::exists(gesture))
        return;
    fs::create_directories(gesture);
}