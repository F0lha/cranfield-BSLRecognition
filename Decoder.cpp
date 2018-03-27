#include "Decoder.h"



Decoder::Decoder()
{
}


Decoder::~Decoder()
{
}



cv::Mat Decoder::decode(std::string gesture) {

	std::vector<std::vector<float>> toTranformInMat;

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir("house/")) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			printf("%s\n", ent->d_name);

			std::cout << "name: " + std::string(ent->d_name) << std::endl;

			std::ifstream inputFile("house/" + std::string(ent->d_name));

			std::string serializedString;
			inputFile >> serializedString;

			Leap::Frame reconstructedFrame;

			reconstructedFrame.deserialize(serializedString);
			Leap::Hand leftHand = reconstructedFrame.hands().leftmost();
			Leap::Hand rightHand = reconstructedFrame.hands().rightmost();
			Leap::FingerList leftFingers = leftHand.fingers();
			Leap::FingerList rightFingers = rightHand.fingers();

			std::vector<float> atributes;

			std::vector<float> leftPosition;
			std::vector<float> rightPosition;
			for (int i = 0; i<leftFingers.count(); ++i) {
				leftPosition.push_back(leftFingers[i].tipPosition().x);
				leftPosition.push_back(leftFingers[i].tipPosition().y);
				leftPosition.push_back(leftFingers[i].tipPosition().z);
				rightPosition.push_back(rightFingers[i].tipPosition().x);
				rightPosition.push_back(rightFingers[i].tipPosition().y);
				rightPosition.push_back(rightFingers[i].tipPosition().z);
			}
			leftPosition.push_back(leftHand.palmPosition().x);
			leftPosition.push_back(leftHand.palmPosition().y);
			leftPosition.push_back(leftHand.palmPosition().z);
			rightPosition.push_back(rightHand.palmPosition().x);
			rightPosition.push_back(rightHand.palmPosition().y);
			rightPosition.push_back(rightHand.palmPosition().z);

			atributes.insert(atributes.end(), leftPosition.begin(), leftPosition.end());
			atributes.insert(atributes.end(), rightPosition.begin(), rightPosition.end());

			toTranformInMat.push_back(atributes);
		}
		closedir(dir);
	}

	cv::Mat m;

	for (int i = 0; i < toTranformInMat.size(); i++)
	{
		//memcpy(m.data, toTranformInMat[i].data(), toTranformInMat[i].size() * sizeof(int));
	}

	return m;
}