#pragma once
#include <opencv_all.h>
///��״ƥ��---�Ƚ�������״������������ƶ�  
class matchShape
{
public:
	matchShape();
	~matchShape();
	vector<int>                          computeContoursSimilarity(vector<vector<vector<Point>>> m_vecPointFeatures, 
		                                 vector<string> m_vecStrMediaName, vector<vector<KeyPoint>> m_vecVecKeyPoint,
										 vector<Mat> m_vecMat);
	bool                                 findMainRectContours(Mat m_matMainImg);
	vector<vector<Point>>                findAllCommonRectContours(Mat m_matCommImg);
	void                                 clear();
	vector<DMatch>                       refineMatchesWithHomography(
		                                 const std::vector<cv::KeyPoint>& queryKeypoints,
		                                 const std::vector<cv::KeyPoint>& trainKeypoints,
		                                 float reprojectionThreshold, std::vector<cv::DMatch>& matches,
		                                 cv::Mat& homography);
	bool                                 computeFeaturesSimilarty(Mat m_matDescriptors1, Mat m_matDescriptors2,
		                                  int m_iMaxNum,vector<vector<KeyPoint>> m_vecVecKeyPoint);
	vector<KeyPoint>                     commonKeyPoint();
private:
	vector<vector<Point>>                m_vecMainPointContours, m_vecCommonPointContours;
	Mat                                  m_matMainGrayImg, m_matCommonGrayImg;
	Mat                                  m_matMainTmpImg, m_matCommonTmpImg;
	int                                  m_iMinThreshold = 150;
	int                                  m_iMaxThreshold = 255;
	double                               m_dReslutSimilarity;
	Ptr<FeatureDetector>                 m_ptrFeatureDetector = new ORB(1000);                        // ����orb��������
	Ptr<DescriptorExtractor>             m_ptrDescriptorExtractor = new FREAK(true, true);           // ��Freak���������������� 
	Ptr<DescriptorMatcher>               m_ptrDescriptorMatcher = new BFMatcher(NORM_HAMMING, true);// ����ƥ�䣬����Hamming����
	vector<KeyPoint>                     m_vecKeyPoint;
	vector<KeyPoint>                     m_vecMainKeyPoint;
	vector<KeyPoint>                     m_vecVecCommonKeyPoint;
	Mat                                  m_matDescriptorsImg, m_matMainDescriptorsImg;
	double                               m_douFeaturesThreshold = 0.15;
};