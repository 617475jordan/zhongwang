#include <halcon_all.h>
#include <opencv_all.h>
#include "halconObjectRecognition.h"
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

#define windowsName "mykinect"
const Size m_size = Size(640, 480);


void initialize();
void testVideo();
void onMouse(int event, int x, int y, int flags, void *param);


int main(int argc, char *argv[])
{
#if defined(_WIN32)
	SetSystem("use_window_thread", "true");
#elif defined(__linux__)
	XInitThreads();
#endif
	
	
	
	//Sleep(2000);
	
	testVideo();

	return 0;
}
void testVideo()
{
	vector<struct objectCurrentData> obj;
	imageEnhancing ImageEnhancement;
	objectRecognition.initialize("\\model");
	capture.open(0);
	if (!capture.isOpened())
	{
		return;
	}
	namedWindow(windowsName);
	setMouseCallback(windowsName, onMouse);
	matHalcon  matHalcon;
	
	
	while (1)
	{
		int m_iTime = clock();
		capture >> m_matImg;
		if (m_matImg.empty())
		{
			return ;
		}
		m_matImg = ImageEnhancement.auto_White_Balance(m_matImg, 1);
		flip(m_matImg, m_matImg, - 1);
		m_matTmpImg = Mat::zeros(m_matImg.rows, m_matImg.cols, m_matImg.type());

		resize(m_matImg, m_matImg, m_size);
		obj=objectRecognition.run(m_matImg);

		imshow(windowsName, m_matImg);
		waitKey(1);
		for (int i = 0; i < obj.size(); i++)
		{
			cout << obj[0].m_veccurrentGoalName[i] << endl;
			cout << obj[0].m_vecCurrentAngle[i] << endl;
		}
		cout << "The total time is :" << clock() - m_iTime << "ms" << endl;
		
	}
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
			putText(m_matImg, m_charCurrrentPosition, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 255));//只是实时显示鼠标移动的坐标 
			imshow(windowsName, m_matImg);
			
		}

	}
	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数  
	{
		sprintf(m_charCurrrentPosition, "(%d,%d)", x, y);
		putText(m_matCurrentImg, m_charCurrrentPosition, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255 ,0, 255 ));//只是实时显示鼠标移动的坐标  
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
			
			namedWindow("SignROI", WINDOW_AUTOSIZE);
			imshow("SignROI", roi);
			waitKey(1);

			objectRecognition.addModel(m_matCurrentImg, m_icurrentRoi_X, m_icurrentRoi_Y, m_icurrentRoi_Width, m_icurrentRoi_Height);
			m_iCurrentImgNum++;
			
		}
		else
		{
			destroyWindow("SignROI");
			roi.release();
		}
		m_bDrawing = false;
	}
}


