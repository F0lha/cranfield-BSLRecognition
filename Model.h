#pragma once

#include <ml.h>

class Model 
{
protected:
	CvStatModel* model;
public:
	Model();
	~Model();

	void save(std::string name);

	void load(std::string name);

	virtual void train(cv::Mat training_data, std::vector<int> label) = 0;

	virtual cv::Mat predict(cv::Mat test_data)=0;
};
