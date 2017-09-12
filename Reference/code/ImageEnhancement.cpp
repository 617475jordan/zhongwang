#include "ImageEnhancement.h"

imageEnhancing::imageEnhancing()
{

}
imageEnhancing::~imageEnhancing()
{

}
/// perform the Simplest Color Balancing algorithm
/**********OpenCV_Auto_White_Balance*************/
Mat imageEnhancing::auto_White_Balance(Mat& in,  float percent)
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

Mat imageEnhancing::equalizehist(Mat&src)
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

cv::Mat imageEnhancing::nightEnhance(Mat m_matImg)
{
	int size = m_matImg.cols * m_matImg.rows;
	uchar r = 0, g = 0, b = 0, g_alpha = 0;
	uchar *pSrcData = m_matImg.data;
	for (int i = 0; i < size; i++, pSrcData += 3)
	{
		g_alpha = 255 - pSrcData[1];
		b = pSrcData[0] * g_alpha >> 8;
		g = pSrcData[1] * g_alpha >> 8;
		r = pSrcData[2] * g_alpha >> 8;

		pSrcData[0] = SCREEN_XY(pSrcData[0], b);
		pSrcData[1] = SCREEN_XY(pSrcData[1], g);
		pSrcData[2] = SCREEN_XY(pSrcData[2], r);
	}
	return m_matImg;
}

cv::Mat imageEnhancing::gammaEnhance(Mat m_matImg)
{
	Mat m_matGamma(m_matImg.size(), CV_32FC3);
	for (int i = 0; i < m_matImg.rows; i++)
	{
		for (int j = 0; j < m_matImg.cols; j++)
		{
			m_matGamma.at<Vec3f>(i, j)[0] = (m_matImg.at<Vec3b>(i, j)[0])*(m_matImg.at<Vec3b>(i, j)[0])*(m_matImg.at<Vec3b>(i, j)[0]);
			m_matGamma.at<Vec3f>(i, j)[1] = (m_matImg.at<Vec3b>(i, j)[1])*(m_matImg.at<Vec3b>(i, j)[1])*(m_matImg.at<Vec3b>(i, j)[1]);
			m_matGamma.at<Vec3f>(i, j)[2] = (m_matImg.at<Vec3b>(i, j)[2])*(m_matImg.at<Vec3b>(i, j)[2])*(m_matImg.at<Vec3b>(i, j)[2]);
		}
	}
	//归一化到0~255    
	normalize(m_matGamma, m_matGamma, 0, 255, CV_MINMAX);
	//转换成8bit图像显示    
	convertScaleAbs(m_matGamma, m_matGamma);
	
	return m_matGamma;
}

cv::Mat imageEnhancing::logEnhance(Mat m_matImg)
{
	Mat m_matLog(m_matImg.size(), CV_32FC3);
	for (int i = 0; i < m_matImg.rows; i++)
	{
		for (int j = 0; j < m_matImg.cols; j++)
		{
			m_matLog.at<Vec3f>(i, j)[0] = log(1 + m_matImg.at<Vec3b>(i, j)[0]);
			m_matLog.at<Vec3f>(i, j)[1] = log(1 + m_matImg.at<Vec3b>(i, j)[1]);
			m_matLog.at<Vec3f>(i, j)[2] = log(1 + m_matImg.at<Vec3b>(i, j)[2]);
		}
	}
	//归一化到0~255    
	normalize(m_matLog, m_matLog, 0, 255, CV_MINMAX);
	//转换成8bit图像显示    
	convertScaleAbs(m_matLog, m_matLog);
	
	return m_matLog;
}

cv::Mat imageEnhancing::laplaceEnhance(Mat m_matImg)
{
	Mat m_matLaplace;
	Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);
	filter2D(m_matImg, m_matLaplace, CV_8UC3, kernel);
	return m_matLaplace;
}
