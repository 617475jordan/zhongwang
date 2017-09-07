#include <halcon_all.h>
#include <opencv_all.h>
#include "halconObjectRecognition.h"
#include "myKinect.h"
#include "remapImage.h"
#include "matHalcon.h"
#include "ImageEnhancement.h"
Mat m_matImg, m_matROIImg, m_matTmpImg, m_matDstImg;
Mat roi;//ROI图像

Point m_pointOriginalPosition;//初始坐标   
Rect m_recRectangle;//标记ROI的矩形框

bool m_bDrawing;
VideoCapture capture;
halconObjectRecognition objectRecognition;
int  m_icurrentRoi_X, m_icurrentRoi_Y, m_icurrentRoi_Width, m_icurrentRoi_Height;
int m_iCurrentImgNum = 1;

#define m_strDstImg  "..\\dst\\%d.jpg"
#define m_strSrcImg	 "..\\src\\current.jpg"
#define windowsName "mykinect"


void initialize();
void onMouse(int event, int x, int y, int flags, void *param);

//const Size m_size = Size(512,424);
int main(int argc, char *argv[])
{
#if defined(_WIN32)
	SetSystem("use_window_thread", "true");
#elif defined(__linux__)
	XInitThreads();
#endif
	
	objectRecognition.initialize();
	
	//Sleep(2000);
	CBodyBasics myKinect;
	remapImage 	remapImage;
	matHalcon  matHalcon;
	//HRESULT hr = myKinect.colorInitializeDefaultSensor();
	//while (FAILED(hr))
	//{
	//	hr = myKinect.colorInitializeDefaultSensor();
	//}
	//namedWindow(windowsName);
	//setMouseCallback(windowsName, onMouse);
	//if (SUCCEEDED(hr))
	//{

	//	while (1)
	//	{
	//		int m_iTime = clock();
	//		m_matImg = myKinect.colorUpdate();
	//		if (m_matImg.empty())
	//		{
	//			m_matImg = myKinect.colorUpdate();
	//		}
	//		else
	//		{
	//			cvtColor(m_matImg, m_matImg, CV_RGB2GRAY);
	//			cvtColor(m_matImg, m_matImg, CV_GRAY2RGB);
	//			m_matTmpImg = Mat::zeros(m_matImg.rows, m_matImg.cols, m_matImg.type());
	//			m_matImg = remapImage.Photo_Remap(m_matImg);
	//			cout << m_matImg.cols << "" << m_matImg.rows << endl;
	//			//resize(m_matImg, m_matImg, m_size);				
	//			objectRecognition.run(matHalcon.IplImageToHImage(m_matImg));
	//			imshow(windowsName, m_matImg);
	//			waitKey(50);
	//		}
	//		cout << "The total time is :" << clock() - m_iTime << endl;
	//	}
	//}
	capture.open(0);
	if (!capture.isOpened())
	{
		return -1;
	}
	else
	{
		namedWindow(windowsName);
		setMouseCallback(windowsName, onMouse);
		imageEnhancement ImageEnhancement;
		while (1)
		{
			int m_iTime = clock();
			capture >> m_matImg;
			if (m_matImg.empty())
			{
				return -1;
			}
			else
			{
				flip(m_matImg, m_matImg, -1);
				GaussianBlur(m_matImg, m_matImg, Size(3, 3), 0);
				GaussianBlur(m_matImg, m_matImg, Size(3, 3), 0);
				cvtColor(m_matImg, m_matImg, CV_RGB2GRAY);
				cvtColor(m_matImg, m_matImg, CV_GRAY2RGB);
				m_matImg=ImageEnhancement.SimplestCB(m_matImg, 1);
				m_matTmpImg = Mat::zeros(m_matImg.rows, m_matImg.cols, m_matImg.type());
				//resize(m_matImg, m_matImg, m_size);				
				objectRecognition.run(matHalcon.IplImageToHImage(m_matImg));
				imshow(windowsName, m_matImg);
				waitKey(1);
			}
			cout << "The total time is :" << clock() - m_iTime << endl;
		}

	}
	return 0;
}

void initialize()
{
	m_bDrawing = false;
	m_pointOriginalPosition = Point(-1, -1);
	roi.release();
}
void onMouse(int event, int x, int y, int flags, void *param)
{
	Mat m_matCurrentImg = m_matImg.clone();
	char m_charCurrrentPosition[100];
	char m_charDstImg[100];
	//按下鼠标左键
	if (event == CV_EVENT_LBUTTONDOWN)//左键按下，读取初始坐标
	{
		//点击鼠标图像时，清除之前ROI图像的显示窗口  
		destroyWindow("ROI");
		//存放起始坐标  
		m_pointOriginalPosition = Point(x, y);
		//初始化起始矩形框  
		m_recRectangle = Rect(x, y, 0, 0);
		m_bDrawing = true;
	}
	//移动光标
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上划矩形  
	{
		if (m_bDrawing)
		{
			//用MIN得到左上点作为矩形框的起始坐标，如果不加这个，画矩形时只能向一个方向进行  
			m_recRectangle.x = m_pointOriginalPosition.x;
			m_recRectangle.y = m_pointOriginalPosition.y;

			m_recRectangle.width = abs(m_recRectangle.x - x);
			m_recRectangle.height = abs(m_recRectangle.y - y);
			//防止矩形区域超出图像的范围  
			m_recRectangle &= Rect(0, 0, m_matImg.cols, m_matImg.rows);
			rectangle(m_matImg, m_recRectangle, Scalar(255), 1);  //画矩形框
			sprintf(m_charCurrrentPosition, "(%d,%d)", x, y);
			putText(m_matImg, m_charCurrrentPosition, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));//只是实时显示鼠标移动的坐标 
			imshow(windowsName, m_matImg);
			
		}

	}
	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数  
	{
		sprintf(m_charCurrrentPosition, "(%d,%d)", x, y);
		putText(m_matCurrentImg, m_charCurrrentPosition, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));//只是实时显示鼠标移动的坐标  
		imshow(windowsName, m_matCurrentImg);
	}

	//松开鼠标左键      
	else if (event == CV_EVENT_LBUTTONUP)//左键松开，将在图像上划矩形 
	{
		destroyWindow("SignROI");
		roi.release();
		m_icurrentRoi_X = m_recRectangle.x;
		m_icurrentRoi_Y = m_recRectangle.y;
		m_icurrentRoi_Width = m_recRectangle.width;
		m_icurrentRoi_Height = m_recRectangle.height;
		if (m_icurrentRoi_Width > 0 && m_icurrentRoi_Height > 0 && m_icurrentRoi_Width*m_icurrentRoi_Height > 100)
		{
			roi = m_matCurrentImg(Rect(m_icurrentRoi_X, m_icurrentRoi_Y, m_icurrentRoi_Width, m_icurrentRoi_Height));
			
			/*namedWindow("SignROI", WINDOW_AUTOSIZE);
			imshow("SignROI", roi);
			waitKey(1);*/

			sprintf(m_charDstImg, m_strDstImg, m_iCurrentImgNum);
			imwrite(m_charDstImg, m_matCurrentImg);
			objectRecognition.upDateNewData(m_charDstImg, m_icurrentRoi_X, m_icurrentRoi_Y, m_icurrentRoi_Width, m_icurrentRoi_Height);
			m_iCurrentImgNum++;
			
		}

		m_bDrawing = false;
	}
}


