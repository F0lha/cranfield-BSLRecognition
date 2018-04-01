
#include "Decoder.h"
#include "Recorder.h"
#include "Classifier.h"
#include "DeepLearningModel.h"
#include "KSS.h"

std::pair<Mat, vector<int>> generateData(vector<cv::Mat> signs, int classes)
{
    Mat all_data;

	vector<int> labels;

	for (int i = 0; i < classes; i++) {
		if (all_data.empty())
			all_data = cv::Mat(signs[i]);
		else
			vconcat(all_data, signs[i], all_data);
		std::vector<int> newlabel(signs[i].rows, i);
		labels.insert(labels.end(), newlabel.begin(), newlabel.end());
	}

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
        classification.push_back(labels.at(seeds[cont]));
    }
    return std::make_pair(output, classification);
}

void recorder(std::string signDir) {
    Leap::Controller controller;
    Recorder recorder = Recorder();
    recorder.setGesture(signDir);
    controller.addListener(recorder);
    Sleep(20000);
    controller.removeListener(recorder);
}

void training(std::string modelFileName) {
	DIR *dir;

	struct dirent *ent;

	char buf[256];
	GetCurrentDirectoryA(256, buf);
	std::string direct = std::string(buf) + "\\dataset";

	int count = 0;

	vector<cv::Mat> signs;
	vector<std::string> signs_name;


	if ((dir = opendir(direct.c_str())) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {

			if (count < 2) {
				count++;
				continue;
			}

			Decoder decoder = Decoder();

			cv::Mat sign = decoder.decode(std::string(ent->d_name));

			signs.push_back(sign);
			signs_name.push_back(std::string(ent->d_name));

		}
	}

    std::pair< Mat, vector<int>> pair = generateData(signs, signs_name.size());


	DeepLearningModel model(signs[0].cols, signs_name.size());

	std::cout << "Training Model" << std::endl;

    model.train(pair.first, pair.second);

	std::cout << "Saving Model" << std::endl;
    model.save(modelFileName);
}

void testing(std::string modelFileName) {
	DIR *dir;

	struct dirent *ent;

	char buf[256];
	GetCurrentDirectoryA(256, buf);
	std::string direct = std::string(buf) + "\\dataset";

	int count = 0;

	vector<cv::Mat> signs;
	vector<std::string> signs_name;


	if ((dir = opendir(direct.c_str())) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {

			if (count < 2) {
				count++;
				continue;
			}

			Decoder decoder = Decoder();

			cv::Mat sign = decoder.decode(std::string(ent->d_name));

			signs.push_back(sign);
			signs_name.push_back(std::string(ent->d_name));

		}
	}

	std::pair< Mat, vector<int>> pair = generateData(signs, signs_name.size());

    DeepLearningModel model;
    model.load(modelFileName);

    Mat prediction = model.predict(pair.first);
    KSS<float> kss(1, 0);
    for (int i = 0; i < prediction.rows; i++) {
        double min, max;
        cv::Point min_loc, max_loc;
        cv::minMaxLoc(prediction.row(i), &min, &max, &min_loc, &max_loc);
        std::cout << "Correct Label :" + std::to_string(pair.second[i]) + " and predicted is " + std::to_string(max_loc.x) << std::endl;
        kss((float)max_loc.x, (float)pair.second[i]);
    }

    std::cout << kss << std::endl;
}

void classifier(std::string modelFileName) {
    DeepLearningModel model;
    model.load(modelFileName);

    Leap::Controller controller;
    Classifier classifier = Classifier(&model);
    controller.addListener(classifier);
    Sleep(60000);
    controller.removeListener(classifier);
}

int main(int argc, char** argv) {
    if (argv[1] == std::string("recorder"))
        recorder(argv[2]);
    if (argv[1] == std::string("training")) {
        training(argv[2]);
    } 
    if (argv[1] == std::string("classifier"))
        classifier(argv[2]);
    if (argv[1] == std::string("testing")) {
        testing(argv[2]);
    }

	system("pause");
}
