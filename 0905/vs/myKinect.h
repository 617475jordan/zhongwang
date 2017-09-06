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
	//kinect 2.0 ����ȿռ�ĸ�*���� 424 * 512���ڹ�������˵��
	static const int        cDepthWidth = 512;
	static const int        cDepthHeight = 424;

public:
	CBodyBasics();
	~CBodyBasics();
	Mat                     deepUpdate();//��ùǼܡ�������ֵͼ�������Ϣ
	Mat                     colorUpdate();
	HRESULT                 deepInitializeDefaultSensor();//���ڳ�ʼ�����kinect
	HRESULT                 colorInitializeDefaultSensor();//���ڳ�ʼ��kinect
	void clear();
private:
	IKinectSensor*          m_pKinectSensor=NULL;//kinectԴ
	ICoordinateMapper*      m_pCoordinateMapper=NULL;//��������任
	IBodyFrameReader*       m_pBodyFrameReader = NULL;//���ڹǼ����ݶ�ȡ
	IDepthFrameReader*      m_pDepthFrameReader = NULL;//����������ݶ�ȡ
	IBodyIndexFrameReader*  m_pBodyIndexFrameReader = NULL;//���ڱ�����ֵͼ��ȡ

	/******��ɫ����*****************/
	IColorFrameSource *colorsource;
	IColorFrameReader *colorreader;
	IFrameDescription *colorde;
	//ͨ����õ�����Ϣ���ѹǼܺͱ�����ֵͼ������
	void                    ProcessBody(int nBodyCount, IBody** ppBodies);
	//���Ǽܺ���
	void DrawBone(const Joint* pJoints, const DepthSpacePoint* depthSpacePosition, JointType joint0, JointType joint1);
	//���ֵ�״̬����
	void DrawHandState(const DepthSpacePoint depthSpacePosition, HandState handState);

	//��ʾͼ���Mat
	cv::Mat skeletonImg;
	cv::Mat depthImg;
	int height;
	int width;
};

