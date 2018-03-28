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

			//std::cout << direct + "\\" + std::string(ent->d_name);

			std::ifstream inputFile(direct + "\\" + std::string(ent->d_name));


			std::vector<float> atributes;

			std::string value;

 			while (getline(inputFile, value, ',')) {
				atributes.push_back(std::stof(value.c_str()));
			}

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
			//std::cout << toTranformInMat[i][k] << ",";
		}
		//std::cout << std::endl;
	}

	return m;
}