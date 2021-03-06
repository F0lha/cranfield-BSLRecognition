#include <filesystem>
#include "Decoder.h"
#include "Recorder.h"
#include "Classifier.h"
#include "DeepLearningModel.h"
#include "RFModel.h"
#include "SVMModel.h"
#include "KSS.h"
#include "RecorderDynamic.h"
#include "DynamicClassifier.h"
#include "FinalClassifier.h"

namespace fs = std::experimental::filesystem;

std::pair<cv::Mat, std::vector<int>>
generateData(vector<cv::Mat> signs, int classes)
{
    Mat         all_data;
	vector<int> labels;
	for (int i = 0; i < classes; i++) {
		if (all_data.empty())
			all_data = cv::Mat(signs[i]);
		else {
			Mat new_all_data;
			vconcat(all_data, signs[i], new_all_data);
			all_data = new_all_data;
		}
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

	std::cout << "out:" << classification.size() << std::endl;

    return { output, classification };
    //return { all_data, labels };
}

std::pair<std::vector<cv::Mat>, std::vector<std::string>>
loadDataSet(std::string datasetDir)
{
    vector<cv::Mat>     signs;
    vector<std::string> signNames;
    for (const auto& p : fs::directory_iterator(datasetDir)) {
        if (fs::is_directory(p.path()))
            signNames.push_back(p.path().filename().string());
    }

    std::sort(signNames.begin(), signNames.end());

    for(const auto& name: signNames) {
        std::cout << "loading sign: " << name << std::endl;
        Decoder decoder;
        signs.push_back(decoder.decode(datasetDir + "/" + name));
    }

    return { signs, signNames };
}

void recorder(std::string signDir) {
    Leap::Controller controller;
    Recorder recorder = Recorder();
    Sleep(3000);
    std::cout << "recording" << std::endl;
    recorder.setGesture(signDir);
    controller.addListener(recorder);
    Sleep(10000);
    controller.removeListener(recorder);
}

void recorderDynamic(std::string signDir, std::string number) {
    Leap::Controller controller;
    RecorderDynamic recorder;
    recorder.setGesture(signDir);
    Sleep(3000);
    //system("pause");
    std::cout << "recording" << std::endl;
    controller.addListener(recorder);
    //system("pause");
    Sleep(5000);
    controller.removeListener(recorder);
    recorder.writeSignatures(number);
}


void training(std::string datasetDir, std::string modelFileName, std::string modelType) {
    auto dataset = loadDataSet(datasetDir);
    auto data    = generateData(dataset.first, dataset.second.size());
	std::unique_ptr<Model> model;
	if (modelType == "deep")
		model.reset(new DeepLearningModel(dataset.first[0].cols, dataset.second.size()));
	else
		model.reset(new SVMModel());

	std::cout << "Training Model" << std::endl;
    model->train(data.first, data.second);
	std::cout << "Saving Model" << std::endl;
    model->save(modelFileName);
}

void testing(std::string datasetDir, std::string modelFileName, std::string modelType) {
    auto dataset    = loadDataSet(datasetDir);
    auto classCount = dataset.second.size();
    auto data       = generateData(dataset.first, classCount);
    std::unique_ptr<Model> model;
    if (modelType == "deep")
        model.reset(new DeepLearningModel(dataset.first[0].cols, classCount));
    else
        model.reset(new SVMModel());
    model->load(modelFileName);

    auto labels = model->predict(data.first);
    std::vector<KSS<size_t>> kss;
    for (size_t i = 0; i < classCount; ++i)
        kss.emplace_back(i);
	for (int i = 0; i < labels.size(); i++) {
        auto actualLabel = data.second[i];
		std::cout << "Correct Label :" + std::to_string(actualLabel) + " and predicted is " + std::to_string(labels[i]) << std::endl;
		kss[actualLabel](labels[i], actualLabel);
    }

    for (const auto kssi : kss) {
        std::cout << kssi << std::endl;
    }
}

void classifier(std::string modelFileName, std::string modelType) {
    std::unique_ptr<Model> model;
    if (modelType == "deep")
        model.reset(new DeepLearningModel());
    else
        model.reset(new SVMModel());
    model->load(modelFileName);

    Leap::Controller controller;
    Classifier classifier = Classifier(model.get());
    controller.addListener(classifier);
    system("pause");
    controller.removeListener(classifier);
}

void dynamicClassifier(std::string modelFileName, std::string modelType) {
    std::unique_ptr<Model> model;
    if (modelType == "deep")
        model.reset(new DeepLearningModel());
	if (modelType == "rf")
		model.reset(new RFModel());
    else
        model.reset(new SVMModel());
    model->load(modelFileName);
    Leap::Controller controller;
    DynamicClassifier classifier = DynamicClassifier(model.get());
    controller.addListener(classifier);
    system("pause");
    controller.removeListener(classifier);
}

void finalClassifier(std::string modelFileName, std::string modelType, std::string modelFileName2, std::string modelType2) {
    std::unique_ptr<Model> model;
    if (modelType == "deep")
        model.reset(new DeepLearningModel());
	if (modelType == "rf")
		model.reset(new RFModel());
    else
        model.reset(new SVMModel());
    model->load(modelFileName);
    std::unique_ptr<Model> model2;
    if (modelType2 == "deep")
        model2.reset(new DeepLearningModel());
    else
        model2.reset(new SVMModel());
    model2->load(modelFileName2);

    Sleep(5000);
    Leap::Controller controller;
    FinalClassifier classifier = FinalClassifier(model.get(), model2.get());
    controller.addListener(classifier);
    system("pause");
    controller.removeListener(classifier);
}

int main(int argc, char** argv) {
	//clock_t tStart = clock();
    if (argv[1] == std::string("recorder"))
        recorder(argv[2]);
    if (argv[1] == std::string("training")) {
        training(argv[2], argv[3], argv[4]);
    } 
    if (argv[1] == std::string("classifier"))
        classifier(argv[2], argv[3]);
    if (argv[1] == std::string("dynamic_classifier"))
        dynamicClassifier(argv[2], argv[3]);
    if (argv[1] == std::string("testing")) {
        testing(argv[2], argv[3], argv[4]);
    }
    if (argv[1] == std::string("recorder_dynamic")) {
        recorderDynamic(argv[2], argv[3]);
    }
    if (argv[1] == std::string("final_classifier"))
        finalClassifier(argv[2], argv[3], argv[4], argv[5]);
	//printf("Time Exectuion: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	//system("pause");
}
