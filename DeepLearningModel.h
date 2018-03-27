#pragma once

#ifndef _DeepLearningModel_H_INCLUDED__
#define __DeepLearningModel_H_INCLUDED__

#include <cv.h>		  // opencv include file
#include <ml.h>		  // opencv machine learning include file



using namespace cv;

class DeepLearningModel
{
	CvANN_MLP* ann;
	int nclasses;
	CvANN_MLP_TrainParams* params;

public:
	DeepLearningModel(int features, int nclasses);
	~DeepLearningModel();


	void train(Mat training_data, vector<int> label);

	Mat predict(Mat test_data);
};

#endif