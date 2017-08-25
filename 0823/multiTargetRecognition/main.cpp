#include <opencv_all.h>
#include "drawing.h"
#include "matchShape.h"
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
#define                       m_strCamName            "..\\data\\1\\book.mp4"
char                          m_charPictureName[100];
static const int              nY20_thresh = 50;

int main()
{
	VideoCapture capture;
	BackgroundSubtractorMOG2 mog;
	drawing drawing;
	Mat m_MatImg;
	
	capture.open(m_strCamName);
	
	if (!capture.isOpened())
	{
		return -1;
	}
	else
	{
		Mat m_matTmpImg;
		matchShape matchShape;
		m_vecPointFeatures.clear();
		m_vecStrMediaName.clear();
		for ( int j = 1; j <= m_iPictureNum; j++)
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
	}
	int m_icount = 0;
	while (1)
	{
		int t = clock();
		m_icount++;
		m_matImg = imread("..\\data\\2\\key4.jpg");
		//capture >> m_matImg;
		if (m_matImg.empty())
		{
			return -1;
		}
		resize(m_matImg, m_matImg, Size(320, 240));
		cvtColor(m_matImg, m_matImg, CV_RGB2GRAY);
		cvtColor(m_matImg, m_matImg, CV_GRAY2RGB);
		//cout << "time0:" << clock() - t << endl;
		mog(m_matImg, m_matForegroundImg, 0.001);
		// 腐蚀  
		erode(m_matForegroundImg, m_matForegroundImg, cv::Mat());
		// 膨胀  
		dilate(m_matForegroundImg, m_matForegroundImg, cv::Mat());
		mog.getBackgroundImage(m_matBackgroundImg);   // 返回当前背景图像  
		//cout << "time0:" << clock() - t << endl;
		//imshow("video", m_matForegroundImg);
		//imshow("background", background);
		if (m_icount >=10)
		{
			threshold(m_matForegroundImg, m_matForegroundImg, nY20_thresh, 255, THRESH_BINARY);
			m_MatImg = drawing.drawingContourMomentAndPointCentroid(m_matForegroundImg, m_matImg, m_vecPointFeatures, m_vecStrMediaName);
			//namedWindow("Contours", CV_WINDOW_AUTOSIZE);
			imshow("Contours", m_MatImg);
			//moveWindow("Contours", 1100, 20);
			waitKey(1);
		}
		else
		{
			namedWindow("Contours", CV_WINDOW_AUTOSIZE);
			imshow("Contours", m_matImg);
			waitKey(1);
		}
		if (waitKey(10)=='c')
		{
			waitKey();
		}
		cout << "time:" << clock() - t << endl;
	}
	waitKey(0);
	m_matImg.release();
	m_matGrayImg.release();
	return 0;
}