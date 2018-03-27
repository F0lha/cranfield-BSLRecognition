#pragma once

#ifndef _Decoder_H_INCLUDED__
#define __Decoder_H_INCLUDED__

#define _CRT_SECURE_NO_DEPRECATE //due to errors in visual studio tools

#include <cv.h>       // opencv general include file
#include <fstream>
#include "Leap.h"
#include "dirent.h"

class Decoder
{
public:
	Decoder();
	~Decoder();

	cv::Mat decode(std::string gesture);
};

#endif