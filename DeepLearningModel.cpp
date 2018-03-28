#include "DeepLearningModel.h"

DeepLearningModel::DeepLearningModel() {
	this->model = new CvANN_MLP();
}

DeepLearningModel::DeepLearningModel(int features, int nclasses)
{
	this->nclasses = nclasses;

	Mat layers = Mat(5,1, CV_32SC1);
	layers.at<int>(0, 0) = features;     // input
	layers.at<int>(1, 0) = 128;  // hidden
	layers.at<int>(2, 0) = 64;  // hidden
	layers.at<int>(3, 0) = 64;  // hidden
	layers.at<int>(4, 0) = nclasses;      // output, 1 pin per class.

	this->model = new CvANN_MLP();

	this->params = new CvANN_MLP_TrainParams(cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 5000, 0.000001),

		// use backpropogation for training
		CvANN_MLP_TrainParams::BACKPROP,

		// co-efficents for backpropogation training
		// (refer to manual)
		0.1,
		0.1);

	((CvANN_MLP *)this->model)->create(layers);
}

void DeepLearningModel::train(Mat training_data, vector<int> label) {

	Mat train_classes = Mat::zeros(training_data.rows, nclasses, CV_32FC1);

	for (int i = 0; i<train_classes.rows; i++)
	{
		train_classes.at<float>(i, label.at(i)) = 1.f;
	}

	((CvANN_MLP *)this->model)->train(training_data, train_classes, Mat(), Mat(), (*params), 0);
}


Mat DeepLearningModel::predict(Mat test_data) {

	Mat predictions = Mat();

	((CvANN_MLP *)this->model)->predict(test_data, predictions);

	return predictions;
}

DeepLearningModel::~DeepLearningModel()
{
}

