#pragma once
#include "halconHead.h"
#include <opencv_all.h>
class matHalcon
{
public:
	matHalcon();
	~matHalcon();
	Mat HImageToIplImage(HObject &Hobj);
	HObject IplImageToHImage(cv::Mat& pImage);
};

