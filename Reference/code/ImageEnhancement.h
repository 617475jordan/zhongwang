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
	Mat nightEnhance(Mat m_matImg);
	Mat gammaEnhance(Mat m_matImg);
	Mat logEnhance(Mat m_matImg);
	Mat laplaceEnhance(Mat m_matImg);
	imageEnhancing();
	~imageEnhancing();

#ifndef SCREEN_XY  
#define SCREEN_XY(x, y)  (255 - ((255 - (x))*(255 - (y)) >> 8))  
#endif  
};
#endif