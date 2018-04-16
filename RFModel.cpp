#include "RFModel.h"



RFModel::RFModel()
{
	model.reset(new CvRTrees());
}


RFModel::~RFModel()
{
}


void RFModel::train(Mat training_data, vector<int> label) {
	CvMat* train_classes = cvCreateMat(training_data.rows, 1, CV_32SC1);
	for (int i = 0; i < static_cast<int>(label.size()); i++) {
		CV_MAT_ELEM(*train_classes, int, i, 0) = label[i];
	}
	static_cast<CvRTrees*>(model.get())->train(training_data, CV_ROW_SAMPLE, train_classes);
}


std::vector<int> RFModel::predict(Mat test_data) {
	Mat predictions = cvCreateMat(test_data.rows, 1, CV_32SC1);
	for (int i = 0; i < test_data.rows; i++)
		predictions.at<int>(i, 0) = static_cast<int>(
			static_cast<CvRTrees*>(model.get())->predict(test_data.row(i)));
	std::vector<int> labels;
	for (int i = 0; i < test_data.rows; i++)
		labels.push_back(predictions.at<int>(i, 0));
	return predictions;
}