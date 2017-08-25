#include <opencv_all.h>
#include "drawing.h"
#include "matchShape.h"
#include "RegionGrowing.h"
Mat                           m_matImg;
Mat                           m_matGrayImg;
Mat                           m_matForegroundImg;
Mat                           m_matBackgroundImg;
int                           m_iThresholdValue = 30;
int                           m_iMaxThresholdValue = 255;
vector<vector<vector<Point>>> m_vecPointFeatures;
vector<string>                m_vecStrMediaName;
#define                       m_strPictureName        "..\\data\\2\\key%d.jpg" //从1开始标记
int                           m_iPictureNum = 4;
#define                       m_strCamName            "..\\data\\1\\panda.mpg"
char                          m_charPictureName[100];
static const int              nY20_thresh = 50;
int                           m_icount = 0;
VideoCapture                  capture;
BackgroundSubtractorMOG2      mog;
int   initialize();

int main()
{	
	drawing drawing;
	regionGrowing regionGrowing;
	if (initialize() == -1)
	{
		return -1;
	}
	while (1)
	{
		int t = clock();
		m_icount++;
		m_matImg = imread("..\\data\\1\\key.jpg");
		//capture >> m_matImg;
		if (m_matImg.empty())
		{
			return -1;
		}
		//拉普拉斯算子图像增强效果
	/*	Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);
		filter2D(m_matImg, m_matImg, CV_8UC3, kernel);
		flip(m_matImg, m_matImg, -1);*/
		resize(m_matImg, m_matImg, Size(320, 240));
		cvtColor(m_matImg, m_matGrayImg, CV_RGB2GRAY);
		cvtColor(m_matGrayImg, m_matImg, CV_GRAY2RGB);
		mog(m_matImg, m_matForegroundImg, 0.001);
		imshow("m_matForegroundImg", m_matForegroundImg);
		//m_matForegroundImg=regionGrowing.RegionGrow(m_matForegroundImg, 140, 2);
		
		//waitKey();
		// 腐蚀  
		Mat element(5, 5, CV_8U, Scalar(255));
		erode(m_matForegroundImg, m_matForegroundImg, element);
		// 膨胀  

		dilate(m_matForegroundImg, m_matForegroundImg, element);
		
		mog.getBackgroundImage(m_matBackgroundImg);   // 返回当前背景图像  
		
		if (m_icount >= 10)
		{
			threshold(m_matForegroundImg, m_matForegroundImg, nY20_thresh, 255, THRESH_BINARY);
			m_matImg = drawing.drawingContourMomentAndPointCentroid(m_matForegroundImg, m_matImg, m_vecPointFeatures, m_vecStrMediaName);
		}

		namedWindow("Contours", CV_WINDOW_AUTOSIZE);
		imshow("Contours", m_matImg);
		waitKey(1);
		cout << "time:" << clock() - t << endl;
	}
	m_matImg.release();
	m_matGrayImg.release();
	capture.release();
	return 0;
}

int   initialize()
{
	capture.open(0);

	if (!capture.isOpened())
	{
		return -1;
	}
	
	Mat m_matTmpImg;
	matchShape matchShape;
	m_vecPointFeatures.clear();
	m_vecStrMediaName.clear();
	for (int j = 1; j <= m_iPictureNum; j++)
	{
		sprintf(m_charPictureName, m_strPictureName, j);
		m_matTmpImg = imread(m_charPictureName);
		if (m_matTmpImg.empty())
		{
			continue;
		}
		resize(m_matTmpImg, m_matTmpImg, Size(320, 240));
		cvtColor(m_matTmpImg, m_matTmpImg, CV_RGB2GRAY);
		cvtColor(m_matTmpImg, m_matTmpImg, CV_GRAY2RGB);
		if (matchShape.findAllCommonRectContours(m_matTmpImg).size() != 0)
		{
			m_vecPointFeatures.push_back(matchShape.findAllCommonRectContours(m_matTmpImg));
			m_vecStrMediaName.push_back(m_charPictureName);
		}
	}
	m_matTmpImg.release();
	matchShape.clear();
	return 0;
}