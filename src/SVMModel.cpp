#include "SVMModel.h"



SVMModel::SVMModel()
{
	model.reset(new CvSVM());
}


SVMModel::~SVMModel()
{
}

void SVMModel::train(Mat training_data, vector<int> label) {
	CvMat* train_classes = cvCreateMat(training_data.rows, 1, CV_32SC1);
	for (int i = 0; i < static_cast<int>(label.size()); i++) {
		CV_MAT_ELEM(*train_classes, int, i, 0) = label[i];
	}
	static_cast<CvSVM*>(model.get())->train(training_data, train_classes);
}


std::vector<int> SVMModel::predict(Mat test_data) {
	Mat predictions = cvCreateMat(test_data.rows, 1, CV_32SC1);
	for(int i = 0; i < test_data.rows; i++)
		predictions.at<int>(i,0) = static_cast<int>(
            static_cast<CvSVM*>(model.get())->predict(test_data.row(i)));
    std::vector<int> labels;
    for (int i = 0; i < test_data.rows; i++)
        labels.push_back(predictions.at<int>(i, 0));
	return predictions;
}