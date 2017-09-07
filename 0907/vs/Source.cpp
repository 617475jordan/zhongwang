#include <halcon_all.h>
#include <opencv_all.h>
#include "halconObjectRecognition.h"
#include "myKinect.h"
#include "remapImage.h"
#include "matHalcon.h"
#include "ImageEnhancement.h"
Mat m_matImg, m_matROIImg, m_matTmpImg, m_matDstImg;
Mat roi;//ROIͼ��

Point m_pointOriginalPosition;//��ʼ����   
Rect m_recRectangle;//���ROI�ľ��ο�

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
	//����������
	if (event == CV_EVENT_LBUTTONDOWN)//������£���ȡ��ʼ����
	{
		//������ͼ��ʱ�����֮ǰROIͼ�����ʾ����  
		destroyWindow("ROI");
		//�����ʼ����  
		m_pointOriginalPosition = Point(x, y);
		//��ʼ����ʼ���ο�  
		m_recRectangle = Rect(x, y, 0, 0);
		m_bDrawing = true;
	}
	//�ƶ����
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//�������ʱ������ƶ�������ͼ���ϻ�����  
	{
		if (m_bDrawing)
		{
			//��MIN�õ����ϵ���Ϊ���ο����ʼ���꣬������������������ʱֻ����һ���������  
			m_recRectangle.x = m_pointOriginalPosition.x;
			m_recRectangle.y = m_pointOriginalPosition.y;

			m_recRectangle.width = abs(m_recRectangle.x - x);
			m_recRectangle.height = abs(m_recRectangle.y - y);
			//��ֹ�������򳬳�ͼ��ķ�Χ  
			m_recRectangle &= Rect(0, 0, m_matImg.cols, m_matImg.rows);
			rectangle(m_matImg, m_recRectangle, Scalar(255), 1);  //�����ο�
			sprintf(m_charCurrrentPosition, "(%d,%d)", x, y);
			putText(m_matImg, m_charCurrrentPosition, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));//ֻ��ʵʱ��ʾ����ƶ������� 
			imshow(windowsName, m_matImg);
			
		}

	}
	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//���û�а��µ����������ƶ��Ĵ�����  
	{
		sprintf(m_charCurrrentPosition, "(%d,%d)", x, y);
		putText(m_matCurrentImg, m_charCurrrentPosition, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));//ֻ��ʵʱ��ʾ����ƶ�������  
		imshow(windowsName, m_matCurrentImg);
	}

	//�ɿ�������      
	else if (event == CV_EVENT_LBUTTONUP)//����ɿ�������ͼ���ϻ����� 
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


