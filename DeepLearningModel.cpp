#include "DeepLearningModel.h"

DeepLearningModel::DeepLearningModel() :
    nclasses{},
    params{}
{
    model.reset(new CvANN_MLP());
}

DeepLearningModel::DeepLearningModel(int features, int nclasses) :
    nclasses{ nclasses }, 
    params{ new CvANN_MLP_TrainParams(
        cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, 0.000001),
        // use backpropogation for training
        CvANN_MLP_TrainParams::BACKPROP,
        // co-efficents for backpropogation training
        // (refer to manual)
        0.1,
        0.1) }
{
    model.reset(new CvANN_MLP());
    Mat layers = Mat(5,1, CV_32SC1);
	layers.at<int>(0, 0) = features;     // input
	layers.at<int>(1, 0) = 128;  // hidden
	layers.at<int>(2, 0) = 64;  // hidden
	layers.at<int>(3, 0) = 64;  // hidden
	layers.at<int>(4, 0) = this->nclasses;      // output, 1 pin per class.
	static_cast<CvANN_MLP*>(model.get())->create(layers, CvANN_MLP::SIGMOID_SYM, 1, 1);
}

void DeepLearningModel::train(Mat training_data, vector<int> label) {
	CvMat* train_classes = cvCreateMat(training_data.rows, this->nclasses, CV_32FC1);
	cvZero(train_classes);
	for (int i = 0; i < label.size(); i++) {
		CV_MAT_ELEM(*train_classes, float, i, label[i]) = 1.0;
	}
    static_cast<CvANN_MLP*>(model.get())->train(training_data, train_classes, Mat(), Mat(), (*params), 0);
}


Mat DeepLearningModel::predict(Mat test_data) {
	Mat predictions = Mat();
    static_cast<CvANN_MLP*>(model.get())->predict(test_data, predictions);
	return predictions;
}

DeepLearningModel::~DeepLearningModel()
{
}
