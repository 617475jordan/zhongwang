#include "ImageEnhancement.h"

imageEnhancement::imageEnhancement()
{

}
imageEnhancement::~imageEnhancement()
{

}
/// perform the Simplest Color Balancing algorithm
/**********OpenCV_Auto_White_Balance*************/
Mat imageEnhancement::SimplestCB(Mat& in,  float percent)
{
	Mat out;
	
	assert(in.channels() == 3);
	assert(percent > 0 && percent < 100);

	float half_percent = percent / 200.0f;

	vector<Mat> tmpsplit; split(in, tmpsplit);
	for (int i = 0; i<3; i++) 
	{
		//find the low and high precentile values (based on the input percentile)
		Mat flat; tmpsplit[i].reshape(1, 1).copyTo(flat);
		cv::sort(flat, flat, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);
		int lowval = flat.at<uchar>(cvFloor(((float)flat.cols) * half_percent));
		int highval = flat.at<uchar>(cvCeil(((float)flat.cols) * (1.0 - half_percent)));
		//cout << lowval << " " << highval << endl;

		//saturate below the low percentile and above the high percentile
		tmpsplit[i].setTo(lowval, tmpsplit[i] < lowval);
		tmpsplit[i].setTo(highval, tmpsplit[i] > highval);

		//scale the channel
		normalize(tmpsplit[i], tmpsplit[i], 0, 255, NORM_MINMAX);
	}
	merge(tmpsplit, out);
	
	return out;
}

Mat imageEnhancement::equalizehist(Mat&src)
{
	Mat imageRGB[3];
	split(src, imageRGB);
	for (int i = 0; i < 3; i++)
	{
		equalizeHist(imageRGB[i], imageRGB[i]);
	}
	merge(imageRGB, 3, src);
	return src;
}
