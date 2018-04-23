#include <filesystem>
#include "RecorderDynamic.h"
#include "Signature.h"
#include <core.hpp>



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
    if (mag > 50) {
        signatures.push_back(signatureDynamic(frame));
    }
}

void RecorderDynamic::writeSignatures(std::string number) {
    if (signatures.empty()) {
        std::cout << "no signatures" << std::endl;
        return;
    }
    const auto filteredSignatures = medianFiltering(signatures,10);
    //const auto pcaSignatures = pcaComputing(signatures);
    std::ofstream outputFile(gesture + "/"+ number + ".data");
    std::string framestring;
    for (const auto& signature : filteredSignatures) {
        for (const auto& v : signature) {
            framestring += std::to_string(v) + ",";
        }
        //framestring += "\n";
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