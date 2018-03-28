
#include "Decoder.h"
#include "Recorder.h"
#include "Classifier.h"
#include "DeepLearningModel.h"
#include "KSS.h"

std::pair<Mat, vector<int>> generateData(cv::Mat sign, cv::Mat noSign, vector<int> labelmathouse, vector<int> labelmatdata)
{
    Mat all_data;
    labelmathouse.insert(labelmathouse.end(), labelmatdata.begin(), labelmatdata.end());

    vconcat(sign, noSign, all_data);

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

void recorder(std::string signDir) {
    Leap::Controller controller;
    Recorder recorder = Recorder();
    recorder.setGesture(signDir);
    controller.addListener(recorder);
    Sleep(20000);
    controller.removeListener(recorder);
}

void training(std::string signDir, std::string noSignDir, std::string modelFileName) {
    Decoder decoder = Decoder();
    cv::Mat sign   = decoder.decode(signDir);
    cv::Mat noSign = decoder.decode(noSignDir);

    DeepLearningModel model(sign.cols, 2);
    vector<int> labelSign = vector<int>(sign.rows, 1);
    vector<int> labelNoSign = vector<int>(noSign.rows, 0);
    std::pair< Mat, vector<int>> pair = generateData(sign, noSign, labelSign, labelNoSign);

    model.train(pair.first, pair.second);
    model.save(modelFileName);
}

void testing(std::string signDir, std::string noSignDir, std::string modelFileName) {
    Decoder decoder = Decoder();
    cv::Mat sign = decoder.decode(signDir);
    cv::Mat noSign = decoder.decode(noSignDir);
    vector<int> labelSign = vector<int>(sign.rows, 1);
    vector<int> labelNoSign = vector<int>(noSign.rows, 0);
    std::pair< Mat, vector<int>> pair = generateData(sign, noSign, labelSign, labelNoSign);

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
    Sleep(20000);
    controller.removeListener(classifier);
}

int main(int argc, char** argv) {
    if (argv[1] == std::string("recorder"))
        recorder(argv[2]);
    if (argv[1] == std::string("training")) {
        training(argv[2], argv[3], argv[4]);
    } 
    if (argv[1] == std::string("classifier"))
        classifier(argv[2]);
    if (argv[1] == std::string("testing")) {
        testing(argv[2], argv[3], argv[4]);
    }
}
