#include "matchShape.h"
///形状匹配---比较两个形状或轮廓间的相似度  
matchShape::matchShape()
{
	m_vecMainPointContours.clear();
	m_vecCommonPointContours.clear();
}
matchShape::~matchShape()
{
	m_vecMainPointContours.clear();
	m_vecCommonPointContours.clear();
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Match shape find contours. </summary>
///
/// <remarks>	Admin, 2017/8/18. </remarks>
///  m_matSrc_1,  m_matSrc_2  must be rgb
/// <param name="m_matSrc_1">	The first m matrix source. </param>
/// <param name="m_matSrc_2">	The second m matrix source. </param>
///-------------------------------------------------------------------------------------------------

vector<int> matchShape::computeContoursSimilarity(vector<vector<vector<Point>>> m_vecPointFeatures)
{
	int m_iMaxNumber = -1;
	int m_iMaxSimiliartyValue = -1;
	vector<int> m_vecMaxResult;
	m_vecMaxResult.clear();
	if (m_vecPointFeatures.size() == 0)
	{
		m_vecMaxResult.push_back(-1);
		m_vecMaxResult.push_back(-1);
		return m_vecMaxResult;
	}
	for (unsigned int i = 0; i < m_vecPointFeatures.size(); i++)
	{
		for (unsigned int j = 0; j < m_vecPointFeatures[i].size(); j++)
		{
			m_dReslutSimilarity = 1 - matchShapes(m_vecMainPointContours[0], m_vecPointFeatures[i][j], CV_CONTOURS_MATCH_I1, 0);
			if (m_dReslutSimilarity>m_iMaxSimiliartyValue)
			{
				m_iMaxSimiliartyValue = m_dReslutSimilarity;
				m_iMaxNumber = i;
			}
		}
	}
	if (m_iMaxSimiliartyValue > 1)
	{
		m_iMaxSimiliartyValue = 1;
	}
	cout << "m_iMaxSimiliartyValue:" << m_iMaxSimiliartyValue << endl;
	cout << "m_iMaxNumber:" << m_iMaxNumber << endl;
	m_vecMaxResult.push_back(m_iMaxSimiliartyValue);
	m_vecMaxResult.push_back(m_iMaxNumber);
	return m_vecMaxResult;
}

bool matchShape::findMainRectContours(Mat m_matMainImg)
{
	Mat element(5, 5, CV_8U, Scalar(255));
	m_vecMainPointContours.clear();
	cvtColor(m_matMainImg, m_matMainGrayImg, CV_RGB2GRAY);
	threshold(m_matMainGrayImg, m_matMainGrayImg, m_iMinThreshold, m_iMaxThreshold, THRESH_BINARY_INV);//确保黑中找白
	//dilate(m_matMainGrayImg, m_matMainGrayImg, element);
	findContours(m_matMainGrayImg, m_vecMainPointContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//最外层轮廓  
	/*drawContours(m_matMainGrayImg, m_vecMainPointContours, -1, Scalar(255));
	imshow("m_matMainGrayImg", m_matMainGrayImg);
	waitKey();*/
	if (m_vecMainPointContours.size() == 0)
	{
		return false;
	}
	else
	{
		/*for (unsigned int i = 0; i < m_vecMainPointContours.size(); i++)
		{
			if (contourArea(m_vecMainPointContours[i])>0)
				m_dMainArea = m_dMainArea + contourArea(m_vecMainPointContours[i]);
		}*/
		return true;
	}	
}
vector<vector<Point>>  matchShape::findAllCommonRectContours(Mat m_matCommImg)
{
	
	Mat element(5, 5, CV_8U, Scalar(255));
	m_vecCommonPointContours.clear();
	cvtColor(m_matCommImg, m_matCommonGrayImg, CV_RGB2GRAY);
	threshold(m_matCommonGrayImg, m_matCommonGrayImg, m_iMinThreshold, m_iMaxThreshold, THRESH_BINARY_INV);//确保黑中找白
	//dilate(m_matCommonGrayImg, m_matCommonGrayImg, element);
	findContours(m_matCommonGrayImg, m_vecCommonPointContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//最外层轮廓
	//drawContours(m_matCommonGrayImg, m_vecCommonPointContours, -1, // draw all contours  
	//	Scalar(255), // in black  
	//	2);
	//imshow("m_matCommonGrayImg", m_matCommonGrayImg);
	//waitKey();
	return m_vecCommonPointContours;
}

void matchShape::clear()
{
	m_vecMainPointContours.clear();
	m_vecCommonPointContours.clear();
	m_matMainGrayImg.release(), m_matCommonGrayImg.release();
	m_matMainTmpImg.release(), m_matCommonTmpImg.release();
}
