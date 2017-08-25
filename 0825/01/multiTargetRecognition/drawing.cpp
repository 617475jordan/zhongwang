#include "drawing.h"

drawing::drawing()
{
	m_vecFilterContours.clear();
	m_vecContours.clear();
	m_vecOutHierarchy.clear();
	m_vecContourMoment.clear();//计算轮廓矩  
	m_vecPointCentroid.clear();//计算轮廓的质心  
	m_vecRectangle.clear();
	m_vecRec.clear();
	m_vecHierarchy.clear();
}
drawing::~drawing()
{
	m_vecFilterContours.clear();
	m_vecContours.clear();
	m_vecOutHierarchy.clear();
	m_vecContourMoment.clear();//计算轮廓矩  
	m_vecPointCentroid.clear();//计算轮廓的质心  
	m_vecRectangle.clear();
	m_vecRec.clear();
	m_vecHierarchy.clear();
}

Mat drawing::drawingContourMomentAndPointCentroid(Mat m_matCommonImg, Mat m_matMainImg, vector<vector<vector<Point>>> m_vecPointFeatures, 
	     vector<string> m_vecStrMediaName, vector<vector<KeyPoint>>      m_vecVecCommonKeyPoint)
{
	double m_dAreaThreshold = m_matMainImg.cols*m_matMainImg.rows*0.005;
	m_vecFilterContours.clear();
	m_vecContours.clear();
	m_vecOutHierarchy.clear();
	m_vecContourMoment.clear();//计算轮廓矩  
	m_vecPointCentroid.clear();//计算轮廓的质心  
	m_vecRectangle.clear();
	m_vecRec.clear();
	m_vecHierarchy.clear();
	//查找轮廓 
	findContours(m_matCommonImg, m_vecContours, m_vecHierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	if (m_vecContours.size() != 0)
	{
		for (unsigned int i = 0; i < m_vecContours.size(); i++)
		{
			double  m_iCurrentArea = contourArea(m_vecContours[i]);
			if (m_iCurrentArea>150 && m_iCurrentArea<m_dAreaThreshold * 5)
			{
				m_vecFilterContours.push_back(m_vecContours[i]);
			}
		}
	}

	if (m_vecFilterContours.size() != 0)
	{
		m_iKeeping = -1;
		return drawingImage(m_matMainImg, m_vecPointFeatures, m_vecStrMediaName,m_vecVecCommonKeyPoint);
	}

	else if (m_vecContours.size() == 0 || m_vecContours.size()==0)
	{
		cout << "m_iKeeping:" << m_iKeeping << endl;
		if (m_iKeeping > 0)
		{
			return m_imatKeepImg;
		}
		m_iKeeping++;
		m_vecFilterContours.clear();
		m_vecContours.clear();
		m_vecHierarchy.clear();
		cvtColor(m_matMainImg, m_matCommonImg, CV_BGR2GRAY);//灰度化  
		GaussianBlur(m_matMainImg, m_matMainImg, Size(3, 3), 0.1, 0, BORDER_DEFAULT);
		blur(m_matCommonImg, m_matCommonImg, Size(3, 3)); //滤波       
		//利用canny算法检测边缘       
		Canny(m_matCommonImg, m_matCommonImg, 30, 255, 3);
		threshold(m_matCommonImg, m_matCommonImg, 145, 255, CV_THRESH_BINARY);

		Mat element(5, 5, CV_8U, Scalar(255));
		dilate(m_matCommonImg, m_matCommonImg, element);
		findContours(m_matCommonImg, m_vecContours, m_vecHierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	}
	//drawContours(m_matCommonImg, m_vecContours, -1, Scalar(255));
	//imshow("m_matMainGrayImg", m_matCommonImg);
	//waitKey();
	if (m_vecContours.size() == 0 && m_iKeeping==-1)
	{
		return m_matMainImg;
	}
	for (unsigned int i = 0; i < m_vecContours.size(); i++)
	{
		
		double m_iCurrentArea = contourArea(m_vecContours[i]);
		if (m_iCurrentArea>300 /*&& m_iCurrentArea < m_dAreaThreshold * 5*/)
		{
			m_vecFilterContours.push_back(m_vecContours[i]);
			//m_vecOutHierarchy.push_back(m_vecHierarchy[i]);
		}
	}
	if (m_vecFilterContours.size() == 0)
	{
		return m_matMainImg;
	}
	//drawContours(m_matCommonImg, m_vecFilterContours, -1, m_scalarTextColor);
	//imshow("m_matCommonImg", m_matCommonImg);
	return drawingImage(m_matMainImg, m_vecPointFeatures,m_vecStrMediaName,m_vecVecCommonKeyPoint);
}

Mat drawing::drawingImage(Mat m_matMainImg, vector<vector<vector<Point>>> m_vecPointFeatures, vector<string> m_vecStrMediaName,
	                       vector<vector<KeyPoint>> m_vecVecCommonKeyPoint)
{
	Mat drawing;
	m_matMainImg.copyTo(drawing);

	for (unsigned int i = 0; i < m_vecFilterContours.size(); i++)
	{
		m_vecRec.push_back(boundingRect(m_vecFilterContours.at(i)));
	//	rectangle(drawing, m_vecRec[i], m_scalarRectangleColor, m_iThickness, m_iType);
	}
	//imshow("m_vecOutHierarchy", drawing);
	m_vecRectangle.clear();
	m_vecContours.clear();

	/***********************矩形框嵌套消除*****************/
	for (unsigned int i = 0; i < m_vecRec.size(); i++)
	{
		Rect r = m_vecRec[i];

		unsigned int j = 0;
		for (; j < m_vecRec.size(); j++)
		{
			//如果时嵌套的就推出循环
			if (j != i && (r & m_vecRec[j]) == r)
				break;
		}
		if (j == m_vecRec.size())
		{
			m_vecRectangle.push_back(r);
			m_vecContours.push_back(m_vecFilterContours[i]);
		}
	}

	//计算轮廓矩
	for (unsigned int i = 0; i < m_vecContours.size(); i++)
	{
		m_vecContourMoment.push_back(moments(m_vecContours[i], false));
	
	}

	//计算轮廓的质心
	for (unsigned int i = 0; i < m_vecContours.size(); i++)
	{
		m_vecPointCentroid.push_back(Point2d(m_vecContourMoment[i].m10 / m_vecContourMoment[i].m00,
			m_vecContourMoment[i].m01 / m_vecContourMoment[i].m00));
	}
	Mat m_matTmpImg;

	for (unsigned int i = 0; i < m_vecRectangle.size(); i++)
	{
		Mat m_rect(m_matMainImg, Rect(m_vecRectangle[i].x, m_vecRectangle[i].y, m_vecRectangle[i].width, m_vecRectangle[i].height));
		int m_iMaxSimilarityFlag = -1;

		/*char file[100];
		sprintf(file, "%d", i);
		imshow(file, m_rect);*/
		//waitKey();

		if (matchShape.findMainRectContours(m_rect) == true)
		{
			vector<int> m_vecMaxValueImg;
			m_vecMaxValueImg.clear();
			m_vecMaxValueImg = matchShape.computeContoursSimilarity(m_vecPointFeatures, m_vecStrMediaName, m_vecVecCommonKeyPoint);
			/*cout << "m_vecMaxValueImg0:" << m_vecMaxValueImg[0] << endl;
			cout << "m_vecMaxValueImg1:" << m_vecMaxValueImg[1] << endl;*/
			if (m_vecMaxValueImg[0] >= 0 && m_vecMaxValueImg[1] >= 0)
			{
			//	m_dMaxSimilarity = 0;
				//drawContours(drawing, m_vecContours, i, m_scalarColor, 2, 8, -1, 0, Point());
				circle(drawing, m_vecPointCentroid[i], m_iRaduis, m_scalarCircleColor, -1, m_iType, 0);
				rectangle(drawing, m_vecRectangle[i], m_scalarRectangleColor);

				/**********最小外接矩形*****************/
				RotatedRect rect = minAreaRect(m_vecContours[i]);
				Point2f P[4];
				rect.points(P);
				double m_iThreshold = 0;
				double m_fWidth, m_fHeight;
				double m_fSum;
				int   m_iCoordinateFlag = 0;
				for (unsigned int j = 0; j <= 3; j++)
				{
				//	int m_iTempThreshold;
					line(drawing, P[j], P[(j + 1) % 4], m_scalarLineColor, m_iThickness);
					m_fWidth = abs(P[j].x - P[(j + 1) % 4].x);
					m_fHeight = abs(P[j].y - P[(j + 1) % 4].y);
					m_fSum = sqrt(m_fWidth*m_fWidth + m_fHeight*m_fHeight);
					if (m_fSum > m_iThreshold)
					{
						m_iThreshold = m_fSum;
						m_iCoordinateFlag = j;
					}

				}
				m_iThreshold = (P[m_iCoordinateFlag].y - P[(m_iCoordinateFlag + 1) % 4].y) / (P[m_iCoordinateFlag].x - P[(m_iCoordinateFlag + 1) % 4].x);
				char m_charFile[100];
				m_iThreshold = atan(m_iThreshold) * 180 / m_fPI;
				if (m_iThreshold < 0)
				{
					m_iThreshold = abs(m_iThreshold);
				}
				else
				{
					m_iThreshold = 180 - m_iThreshold;
				}
				sprintf(m_charFile, "(%0.0f,%0.0f,%0.0f)", m_vecPointCentroid[i].x, m_vecPointCentroid[i].y, m_iThreshold);
				putText(drawing, m_charFile, Point(m_vecPointCentroid[i].x, m_vecPointCentroid[i].y), FONT_HERSHEY_SIMPLEX, 0.4, m_scalarTextColor, 1);
				putText(drawing, m_vecStrMediaName[m_vecMaxValueImg[1]], Point(m_vecPointCentroid[i].x, m_vecPointCentroid[i].y + 10), FONT_HERSHEY_SIMPLEX, 0.4, m_scalarTextColor, 1);
			}
		}
		
	}
	m_imatKeepImg = drawing;
	//imshow("m_imatKeepImg", m_imatKeepImg);
	return drawing;
}