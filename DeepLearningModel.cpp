#include "DeepLearningModel.h"

DeepLearningModel::DeepLearningModel(int features, int nclasses)
{
	this->nclasses = nclasses;

	Mat layers = Mat(5,1, CV_32SC1);
	layers.at<int>(0, 0) = features;     // input
	layers.at<int>(1, 0) = 512;  // hidden
	layers.at<int>(2, 0) = 256;  // hidden
	layers.at<int>(3, 0) = 256;  // hidden
	layers.at<int>(4, 0) = nclasses;      // output, 1 pin per class.

	this->ann = new CvANN_MLP();

	this->params = new CvANN_MLP_TrainParams(cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, 0.000001),

		// use backpropogation for training
		CvANN_MLP_TrainParams::BACKPROP,

		// co-efficents for backpropogation training
		// (refer to manual)
		0.1,
		0.1);

	ann->create(layers);
}

void DeepLearningModel::train(Mat training_data, vector<int> label) {

	Mat train_classes = Mat::zeros(training_data.rows, nclasses, CV_32FC1);

	for (int i = 0; i<train_classes.rows; i++)
	{
		train_classes.at<float>(i, label.at(i)) = 1.f;
	}

	ann->train(training_data, train_classes, Mat(), Mat(), (*params), 0);
}


Mat DeepLearningModel::predict(Mat test_data) {

	Mat predictions = Mat();

	ann->predict(test_data, predictions);

	return predictions;
}

DeepLearningModel::~DeepLearningModel()
{
}
