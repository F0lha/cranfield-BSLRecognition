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

	char buf[256];
	GetCurrentDirectoryA(256, buf);
	std::string direct = std::string(buf) + "\\" + gesture;

	int count = 0;


	if ((dir = opendir(direct.c_str())) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {

			if (count < 2) {
				count++;
				continue;
			}
			//needed for some reason
			Leap::Controller controller;

			//std::cout << direct + "\\" + std::string(ent->d_name);

			std::ifstream inputFile(direct + "\\" + std::string(ent->d_name), std::ios::binary);

			if (!inputFile)
				std::cout << "Error";

			std::string frameData((std::istreambuf_iterator<char>(inputFile)),
				(std::istreambuf_iterator<char>()));

			std::cout << "Size :" << frameData.length() << std::endl;

			Leap::Frame reconstructedFrame;

			reconstructedFrame.deserialize(frameData);

			Leap::Hand leftHand = reconstructedFrame.hands().leftmost();
			Leap::Hand rightHand = reconstructedFrame.hands().rightmost();
			Leap::FingerList leftFingers = leftHand.fingers();
			Leap::FingerList rightFingers = rightHand.fingers();

			std::vector<float> atributes();

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

	cv::Mat m(toTranformInMat.size(), toTranformInMat.at(0).size(), CV_32FC1);

	for (int i = 0; i < toTranformInMat.size(); i++)
	{
		for (int k = 0; k < toTranformInMat[i].size(); k++)
		{
			m.at<float>(i, k) = toTranformInMat[i][k];
			std::cout << toTranformInMat[i][k] << ",";
		}
		std::cout << std::endl;
	}

	return m;
}