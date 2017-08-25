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

vector<int> matchShape::computeContoursSimilarity(vector<vector<vector<Point>>> m_vecPointFeatures, 
	vector<string> m_vecStrMediaName, vector<vector<KeyPoint>> m_vecVecKeyPoint, vector<Mat> m_vecMat)
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
	int t = clock();
	/*cout << "m_iMaxSimiliartyValue:" << m_iMaxSimiliartyValue << endl;
    cout << "m_iMaxNumber:" << m_iMaxNumber << endl;

	m_vecMaxResult.push_back(m_iMaxSimiliartyValue);
	m_vecMaxResult.push_back(m_iMaxNumber);*/
	Mat m_vecMatDescriptorsImg = m_vecMat[m_iMaxNumber];

	//cout << "Testtime:" << clock() - t << "ms" << endl;
	if (m_matMainDescriptorsImg.empty() || m_vecMatDescriptorsImg.empty())
	{
		m_vecMaxResult.push_back(-1);
		m_vecMaxResult.push_back(-1);
	}
	else
	{
		resize(m_vecMatDescriptorsImg, m_vecMatDescriptorsImg, Size(m_matMainDescriptorsImg.cols, m_matMainDescriptorsImg.rows));
		//cvtColor(m_vecMatDescriptorsImg, m_vecMatDescriptorsImg, CV_RGB2GRAY);
		//imshow("test", m_vecMatDescriptorsImg);
		//cout << "len: " << computeFeaturesSimilarty(m_matMainDescriptorsImg, m_vecMatDescriptorsImg, m_iMaxNumber, m_vecVecKeyPoint) << endl;
		if (computeFeaturesSimilarty(m_matMainDescriptorsImg, m_vecMatDescriptorsImg, m_iMaxNumber, m_vecVecKeyPoint))
		{
			cout << "m_iMaxSimiliartyValue:" << m_iMaxSimiliartyValue << endl;
			cout << "m_iMaxNumber:" << m_iMaxNumber << endl;
			m_vecMaxResult.push_back(m_iMaxSimiliartyValue);
			m_vecMaxResult.push_back(m_iMaxNumber);
		}
		else
		{
			m_vecMaxResult.push_back(-1);
			m_vecMaxResult.push_back(-1);
		}
	}
	
	return m_vecMaxResult;
}

bool matchShape::findMainRectContours(Mat m_matMainImg)
{
	/*resize(m_matMainImg, m_matMainImg, Size(640, 480));
	m_ptrFeatureDetector->detect(m_matMainImg, m_vecMainKeyPoint);
	m_ptrDescriptorExtractor->compute(m_matMainImg, m_vecMainKeyPoint, m_matDescriptorsImg);
	imshow("m_matDescriptorsImg", m_matDescriptorsImg);
	waitKey();*/

	Mat element(5, 5, CV_8U, Scalar(255));
	m_vecMainPointContours.clear();
	m_vecMainKeyPoint.clear();
	cvtColor(m_matMainImg, m_matMainGrayImg, CV_RGB2GRAY);
	m_matMainGrayImg.copyTo(m_matMainTmpImg);
	
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
		resize(m_matMainTmpImg, m_matMainTmpImg, Size(640, 480));
		//imwrite("..\\data\\tmp.jpg", m_matMainTmpImg);
		//m_matMainTmpImg = imread("..\\data\\tmp.jpg");
		m_ptrFeatureDetector->detect(m_matMainTmpImg, m_vecMainKeyPoint);
		//cout << "m_vecMainKeyPoint:" << m_vecMainKeyPoint.size() << endl;
		//waitKey();
		if (!m_vecMainKeyPoint.empty())
		{
			//imshow("m_matMainTmpImg", m_matMainTmpImg);
			//waitKey();
			/********m_matDescriptorsImg输出无结果************/
			m_ptrDescriptorExtractor->compute(m_matMainTmpImg, m_vecMainKeyPoint, m_matMainDescriptorsImg);
			//cout << " " << m_matDescriptorsImg << endl;
			//waitKey();
			//imshow("m_matDescriptorsImg", m_matMainDescriptorsImg);

			return true;
		}
		else
		{
			return false;
		}
		/*for (unsigned int i = 0; i < m_vecMainPointContours.size(); i++)
		{
			if (contourArea(m_vecMainPointContours[i])>0)
				m_dMainArea = m_dMainArea + contourArea(m_vecMainPointContours[i]);
		}*/

		
	}	
}
vector<vector<Point>>  matchShape::findAllCommonRectContours(Mat m_matCommImg)
{
	
	Mat element(5, 5, CV_8U, Scalar(255));
	m_vecCommonPointContours.clear();
	cvtColor(m_matCommImg, m_matCommonGrayImg, CV_RGB2GRAY);
	m_matCommonGrayImg.copyTo(m_matCommonTmpImg);
	threshold(m_matCommonGrayImg, m_matCommonGrayImg, m_iMinThreshold, m_iMaxThreshold, THRESH_BINARY_INV);//确保黑中找白
	//dilate(m_matCommonGrayImg, m_matCommonGrayImg, element);
	findContours(m_matCommonGrayImg, m_vecCommonPointContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//最外层轮廓
	
	//drawContours(m_matCommonGrayImg, m_vecCommonPointContours, -1, // draw all contours  
	//	Scalar(255), // in black  
	//	2);
	//imshow("m_matCommonGrayImg", m_matCommonGrayImg);
	//waitKey();
	m_ptrFeatureDetector->detect(m_matCommonTmpImg, m_vecKeyPoint);
	m_ptrDescriptorExtractor->compute(m_matCommonTmpImg, m_vecKeyPoint, m_matDescriptorsImg);
	//imshow("m_matDescriptorsImg", m_matDescriptorsImg);
	//waitKey();
	if (!m_vecKeyPoint.empty())
	{
		m_vecVecCommonKeyPoint.clear();
		m_vecVecCommonKeyPoint=m_vecKeyPoint;
		m_matDescriptorsImg.release();
		return m_vecCommonPointContours;
	}
	else
	{
		m_vecCommonPointContours.clear();
		//m_matDescriptorsImg.release();
		return m_vecCommonPointContours;
	}
}

void matchShape::clear()
{
	m_vecMainPointContours.clear();
	m_vecCommonPointContours.clear();
	m_matMainGrayImg.release(), m_matCommonGrayImg.release();
	m_matMainTmpImg.release(), m_matCommonTmpImg.release();
}

vector<DMatch> matchShape::refineMatchesWithHomography(const std::vector<cv::KeyPoint>& queryKeypoints,
	const std::vector<cv::KeyPoint>& trainKeypoints, float reprojectionThreshold, 
	std::vector<cv::DMatch>& matches, cv::Mat& homography)
{
	const int minNumberMatchesAllowed = 8;

	if (matches.size() < minNumberMatchesAllowed)
		return matches;

	// Prepare data for cv::findHomography  
	std::vector<cv::Point2f> srcPoints(matches.size());
	std::vector<cv::Point2f> dstPoints(matches.size());

	for (size_t i = 0; i < matches.size(); i++)
	{
		srcPoints[i] = trainKeypoints[matches[i].trainIdx].pt;
		dstPoints[i] = queryKeypoints[matches[i].queryIdx].pt;
	}

	// Find homography matrix and get inliers mask  
	std::vector<unsigned char> inliersMask(srcPoints.size());
	homography = cv::findHomography(srcPoints, dstPoints, CV_FM_RANSAC,
		reprojectionThreshold, inliersMask);

	std::vector<cv::DMatch> inliers;
	for (size_t i = 0; i < inliersMask.size(); i++) {
		if (inliersMask[i])
			inliers.push_back(matches[i]);
	}

	matches.swap(inliers);
	return matches;
}

bool matchShape::computeFeaturesSimilarty(Mat m_matDescriptors1, Mat m_matDescriptors2, int m_iMaxNum,vector<vector<KeyPoint>> m_vecVecKeyPoint)
{
	
	vector<DMatch>  m_vecDMatches;
	Mat             m_matHomo;
	m_ptrDescriptorMatcher->match(m_matDescriptors1, m_matDescriptors2, m_vecDMatches);
	double m_iTotolFeaturesNum = m_vecDMatches.size();
	double m_douNewFeaturesNum = refineMatchesWithHomography(m_vecMainKeyPoint, m_vecVecKeyPoint[m_iMaxNum], 3, m_vecDMatches, m_matHomo).size();
	
	if (m_douNewFeaturesNum>5 /*/ m_iTotolFeaturesNum >= m_douFeaturesThreshold*/)
	{
		return true;
	}
	else
	{
		return false;
	}
}

vector<cv::KeyPoint> matchShape::commonKeyPoint()
{
	return m_vecVecCommonKeyPoint;
}
