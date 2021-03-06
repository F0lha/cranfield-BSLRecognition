#pragma once

#ifndef _DeepLearningModel_H_INCLUDED__
#define __DeepLearningModel_H_INCLUDED__

#include <cv.h>		  // opencv include file
#include <ml.h>		  // opencv machine learning include file
#include <memory>

#include "Model.h"

using namespace cv;

class DeepLearningModel : public Model
{
    int nclasses;
    std::unique_ptr<CvANN_MLP_TrainParams> params;
public:
    DeepLearningModel();
	DeepLearningModel(int features, int nclasses);
    ~DeepLearningModel();

	void train(Mat training_data, vector<int> label) override;
	std::vector<int> predict(Mat test_data) override;
};

#endif