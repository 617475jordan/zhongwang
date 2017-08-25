#pragma once
#include <opencv_all.h>
///��״ƥ��---�Ƚ�������״������������ƶ�  
class matchShape
{
public:
	matchShape();
	~matchShape();
	vector<int>                          computeContoursSimilarity(vector<vector<vector<Point>>> m_vecPointFeatures);
	bool                                 findMainRectContours(Mat m_matMainImg);
	vector<vector<Point>>                findAllCommonRectContours(Mat m_matCommImg);
private:
	vector<vector<Point>>                m_vecMainPointContours, m_vecCommonPointContours;
	Mat                                  m_matMainGrayImg, m_matCommonGrayImg;
	Mat                                  m_matMainTmpImg, m_matCommonTmpImg;
	int                                  m_iMinThreshold = 100;
	int                                  m_iMaxThreshold = 255;
	double                               m_dReslutSimilarity;
};