#pragma once
#include "Model.h"

using namespace cv;

class SVMModel :
	public Model
{
public:
	SVMModel();
	~SVMModel();

	void train(Mat training_data, vector<int> label) override;
	std::vector<int> predict(Mat test_data) override;
};

