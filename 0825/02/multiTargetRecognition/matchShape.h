#pragma once
#include <opencv_all.h>
///形状匹配---比较两个形状或轮廓间的相似度  
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
	Ptr<FeatureDetector>                 m_ptrFeatureDetector = new ORB(1000);                        // 创建orb特征点检测
	Ptr<DescriptorExtractor>             m_ptrDescriptorExtractor = new FREAK(true, true);           // 用Freak特征来描述特征点 
	Ptr<DescriptorMatcher>               m_ptrDescriptorMatcher = new BFMatcher(NORM_HAMMING, true);// 特征匹配，计算Hamming距离
	vector<KeyPoint>                     m_vecKeyPoint;
	vector<KeyPoint>                     m_vecMainKeyPoint;
	vector<KeyPoint>                     m_vecVecCommonKeyPoint;
	Mat                                  m_matDescriptorsImg, m_matMainDescriptorsImg;
	double                               m_douFeaturesThreshold = 0.15;
};