#pragma once
#include "Model.h"

using namespace cv;

class RFModel :
	public Model
{
public:
	RFModel();
	~RFModel();

	void train(Mat training_data, vector<int> label) override;
	std::vector<int> predict(Mat test_data) override;
};

