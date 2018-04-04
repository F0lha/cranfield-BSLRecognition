#include <filesystem>
#include "Recorder.h"
#include "Signature.h"

namespace fs = std::experimental::filesystem;

Recorder::Recorder()
{
}

Recorder::~Recorder()
{
}

void Recorder::onFrame(const Leap::Controller& controller) {
    // Get the most recent frame and report some basic information
	const Leap::Frame frame = controller.frame();
	std::ofstream outputFile(gesture + "/" + std::to_string(frame_counter) + ".data");
	auto attributes = signatureStatic(frame.hands());
	std::string framestring;
    for (const auto& v : attributes) {
        framestring += std::to_string(v) + ",";
    }
	
	outputFile << framestring;
	outputFile.close();

	this->frame_counter++;
}

void Recorder::setGesture(std::string gesture) {
    if (fs::exists(gesture))
        fs::remove_all(gesture);
    fs::create_directories(gesture);
    this->gesture = gesture;
}