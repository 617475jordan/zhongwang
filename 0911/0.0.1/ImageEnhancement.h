#ifndef _IMAGEENJANCEMENT_H_
#define _IMAGEENJANCEMENT_H_

#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

class imageEnhancing
{
public:
	Mat auto_White_Balance(Mat& in, float percent);
	Mat equalizehist(Mat&src);
	imageEnhancing();
	~imageEnhancing();
};
#endif