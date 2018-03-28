
#include "Decoder.h"
#include "Recorder.h"
#include "DeepLearningModel.h"

/**
Edited and based on: https://stackoverflow.com/a/23152473
**/
std::pair < Mat, vector<int>> shuffleData(cv::Mat mathouse, cv::Mat matdata, vector<int> labelmathouse, vector<int> labelmatdata)
{
	Mat all_data;
	labelmathouse.insert(labelmathouse.end(), labelmatdata.begin(), labelmatdata.end());

	vconcat(mathouse, matdata, all_data);

	std::vector <int> seeds;
	for (int cont = 0; cont < all_data.rows; cont++)
		seeds.push_back(cont);

	RNG rng = RNG();
	rng.state = getTickCount();
	cv::randShuffle(seeds, rng);

	cv::Mat output;
	vector<int> classification;
	for (int cont = 0; cont < all_data.rows; cont++) {
		output.push_back(all_data.row(seeds[cont]));
		classification.push_back(labelmathouse.at(seeds[cont]));
	}
	return std::make_pair(output, classification);
}

int main(int argc, char** argv) {

	/*Recorder recorder = Recorder();

	recorder.setGesture("house");

	Leap::Controller controller;

	controller.addListener(recorder);

	Sleep(5000);

	controller.removeListener(recorder);*/

	Decoder decoder = Decoder();

	cv::Mat mathouse = decoder.decode("house");

	cv::Mat matdata = decoder.decode("data");

	DeepLearningModel model = DeepLearningModel(mathouse.cols, 2);

	vector<int> labelmathouse = vector<int>(mathouse.rows, 1);
	vector<int> labelmatdata = vector<int>(matdata.rows, 0);

	std::pair < Mat, vector<int>> pair = shuffleData(mathouse, matdata, labelmathouse, labelmatdata);

	std::cout << "train" << std::endl;

	model.train(pair.first, pair.second);

	Mat prediction = model.predict(pair.first.rowRange(0, 50));

	for (int i = 0; i < prediction.rows; i++)
	{
		std::cout << "Correct Label :" + std::to_string(pair.second[i]) + " and predicted is " + std::to_string(pair.first.at<float>(i, 0)) + "/" + std::to_string(pair.first.at<float>(i, 1)) << std::endl;
	}
	
	system("pause");

}
