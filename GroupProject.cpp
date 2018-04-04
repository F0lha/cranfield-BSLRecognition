
#include "Decoder.h"
#include "Recorder.h"
#include "Classifier.h"
#include "DeepLearningModel.h"
#include "SVMModel.h"
#include "KSS.h"
#ifdef __APPLE__
#include <zconf.h>
#endif


std::pair<Mat, vector<int>> generateData(vector<cv::Mat> signs, int classes)
{
    Mat all_data;

    vector<int> labels = vector<int>();

    for (int i = 0; i < classes; i++) {
        if (all_data.empty())
            all_data = cv::Mat(signs[i]);
        else
        {
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

    std::cerr << "out:" << classification.size() << std::endl;

    return std::make_pair(output, classification);
}

void recorder(std::string signDir) {
    Leap::Controller controller;
    Recorder recorder = Recorder();
    recorder.setGesture(signDir);
    controller.addListener(recorder);
#ifdef _WIN32
    Sleep(20000);
#endif
    controller.removeListener(recorder);
}

void training(std::string modelFileName, std::string modelType) {
    DIR *dir;

    struct dirent *ent;

    char buf[256];

#ifdef _WIN32
    GetCurrentDirectoryA(256, buf);
    std::string direct = std::string(buf) + "\\dataset";
#elif defined __APPLE__
    getcwd(buf, 256);
    std::string direct = std::string(buf) + "/";
#endif

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

    Model* model;

    if (modelType == "deep")
        model = new DeepLearningModel(signs[0].cols, signs_name.size());
    else
        model = new SVMModel();

    std::cout << "Training Model" << std::endl;

    model->train(pair.first, pair.second);

    std::cout << "Saving Model" << std::endl;
    model->save(modelFileName);
}

void testing(std::string modelFileName, std::string modelType) {
    DIR *dir;

    struct dirent *ent;

    char buf[256];

#ifdef _WIN32
    GetCurrentDirectoryA(256, buf);
    std::string direct = std::string(buf) + "\\dataset";
#elif defined __APPLE__
    getcwd(buf, 256);
    std::string direct = std::string(buf) + "/";
#endif

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

    Model* model;

    if (modelType == "deep")
        model = new DeepLearningModel(signs[0].cols, signs_name.size());
    else
        model = new SVMModel();
    model->load(modelFileName);

    Mat prediction = model->predict(pair.first);
    KSS<float> kss(1, 0);
    for (int i = 0; i < prediction.rows; i++) {
        if (modelType == "deep") {
            double min, max;
            cv::Point min_loc, max_loc;
            cv::minMaxLoc(prediction.row(i), &min, &max, &min_loc, &max_loc);

            std::cout << "Correct Label :" + std::to_string(pair.second[i]) + " and predicted is " + std::to_string(max_loc.x) << std::endl;
            kss((float)max_loc.x, (float)pair.second[i]);
        }
        else
        {
            std::cout << "Correct Label :" + std::to_string(pair.second[i]) + " and predicted is " << prediction.at<int>(i,0) << std::endl;
            kss(prediction.at<int>(i, 0), (int)pair.second[i]);
        }

    }

    std::cout << kss << std::endl;
}

void classifier(std::string modelFileName, std::string modelType) {
    Model* model;

    if (modelType == "deep")
        model = new DeepLearningModel();
    else
        model = new SVMModel();
    model->load(modelFileName);

    Leap::Controller controller;
    Classifier classifier = Classifier(model);
    controller.addListener(classifier);
#ifdef _WIN32
    Sleep(60000);
#endif
    controller.removeListener(classifier);
}


//argv2 is the name of the xml file containing the model
//argv3 is the type of model deep/svm
int main(int argc, char** argv) {
    if (argv[1] == std::string("recorder"))
        recorder(argv[2]);
    if (argv[1] == std::string("training")) {
        training(argv[2], argv[3]);
    }
    if (argv[1] == std::string("classifier"))
        classifier(argv[2], argv[3]);
    if (argv[1] == std::string("testing")) {
        testing(argv[2], argv[3]);
    }
//    string arg1, arg2, arg3;
//    arg1 = "training/house";
//    arg2 = "training/data";
//    arg3 = "training/house2";
//    training(arg1, arg2, arg3);
//    arg1 = "testing/house";
//    arg2 = "testing/data";
//    arg3 = "training/house2.xml";
//    testing(arg1, arg2, arg3);


#ifdef _WIN32
    system("pause");
#endif
}
