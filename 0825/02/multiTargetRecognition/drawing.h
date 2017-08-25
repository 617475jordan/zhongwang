#pragma once
#include <opencv_all.h>
#include "matchShape.h"

class drawing
{
public:
	drawing();
	~drawing();
	Mat                       drawingContourMomentAndPointCentroid(Mat m_matCommonImg, Mat m_matMainImg, 
		                       vector<vector<vector<Point>>> m_vecPointFeatures, vector<string> m_vecStrMediaName,
							   vector<vector<KeyPoint>>m_vecVecCommonKeyPoint, vector<Mat> m_vecMat);
	Mat                       drawingImage(Mat m_matMainImg, vector<vector<vector<Point>>> m_vecPointFeatures, 
		                       vector<string> m_vecStrMediaName, vector<vector<KeyPoint>> m_vecVecCommonKeyPoint,
							   vector<Mat> m_vecMat);
private:
	vector<Moments>           m_vecContourMoment;//计算轮廓矩  
	vector<Point2f>           m_vecPointCentroid;//计算轮廓的质心  
	vector<vector<Point>>     m_vecContours;
	vector<vector<Point>>     m_vecFilterContours;
	vector<Rect>              m_vecRectangle;
	vector<Rect>              m_vecRec;
	vector<Vec4i>             m_vecHierarchy;
	vector<Vec4i>             m_vecOutHierarchy;
	double                     m_fPI = acos(-1.0);
	char                      m_charPictureName[100] ;
	matchShape                matchShape;
	Scalar                    m_scalarLineColor = Scalar(255, 0, 0);
	Scalar                    m_scalarCircleColor = Scalar(0, 0, 255);
	Scalar                    m_scalarRectangleColor = Scalar(0,255,0);
	Scalar                    m_scalarTextColor = Scalar(255, 0, 255);
	int                       m_iRaduis = 4;
	int                       m_iType = 8;
	int                       m_iThickness = 1;
	int                       m_iPictureNum = 2;
	int                       m_iKeeping=-1;
	Mat                       m_imatKeepImg;
	double                    m_dAreaThreshold;
};