#pragma once
#include <Kinect.h>
#include <opencv_all.h>
// Safe release for interfaces
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

class CBodyBasics
{
	//kinect 2.0 的深度空间的高*宽是 424 * 512，在官网上有说明
	static const int        cDepthWidth = 512;
	static const int        cDepthHeight = 424;

public:
	CBodyBasics();
	~CBodyBasics();
	Mat                     deepUpdate();//获得骨架、背景二值图和深度信息
	Mat                     colorUpdate();
	HRESULT                 deepInitializeDefaultSensor();//用于初始化深度kinect
	HRESULT                 colorInitializeDefaultSensor();//用于初始化kinect
	void clear();
private:
	IKinectSensor*          m_pKinectSensor=NULL;//kinect源
	ICoordinateMapper*      m_pCoordinateMapper=NULL;//用于坐标变换
	IBodyFrameReader*       m_pBodyFrameReader = NULL;//用于骨架数据读取
	IDepthFrameReader*      m_pDepthFrameReader = NULL;//用于深度数据读取
	IBodyIndexFrameReader*  m_pBodyIndexFrameReader = NULL;//用于背景二值图读取

	/******彩色数据*****************/
	IColorFrameSource *colorsource;
	IColorFrameReader *colorreader;
	IFrameDescription *colorde;
	//通过获得到的信息，把骨架和背景二值图画出来
	void                    ProcessBody(int nBodyCount, IBody** ppBodies);
	//画骨架函数
	void DrawBone(const Joint* pJoints, const DepthSpacePoint* depthSpacePosition, JointType joint0, JointType joint1);
	//画手的状态函数
	void DrawHandState(const DepthSpacePoint depthSpacePosition, HandState handState);

	//显示图像的Mat
	cv::Mat skeletonImg;
	cv::Mat depthImg;
	int height;
	int width;
};

