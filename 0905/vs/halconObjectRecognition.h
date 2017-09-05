#pragma once
#include <halcon_all.h>
#include <opencv_all.h>
class halconObjectRecognition
{
public:
	halconObjectRecognition();
	~halconObjectRecognition();
	void                     initialize();
	void                     set_display_font(HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold,
		HTuple hv_Slant);
	// Chapter: Graphics / Text
	// Short Description: This procedure displays 'Click 'Run' to continue' in the lower right corner of the screen. 
	void                     disp_continue_message(HTuple hv_WindowHandle, HTuple hv_Color, HTuple hv_Box);
	void                     disp_message(HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem,
		HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box);
	void                     run(HObject ho_SearchImage, Mat m_matImg);
	void                     setUpModel(char m_charFileName[100], int m_iCurrentFlag, int m_iCurrentId);
	void                     upDateNewData(char m_charFileName[100], int m_iCurrentRoi_X, int m_iCurrentRoi_Y,
		                      int m_iCurrentCols, int m_iCurrentRows,int m_iCurrentId);
	Mat                      drawArrow(cv::Mat& img, cv::Point pStart, cv::Point pEnd,
		double angle);

private:
	const int                m_iModelImgNum = 1;
	const int                m_iImgNum = 10;
	char                     m_charfileName[100];
	const double 			 m_dMinScore = 0.25;
	const double 			 m_dMaxOverLap = 0.85;
	const int                m_iLen = 50;
	const int                m_iLineLen = 20;
	const int                m_iThickness = 2;
	const int                m_iLineType = 4;
	const double             m_dalpha = PI / 4;
	Scalar                   m_scalarColor = Scalar(255, 0, 0);
	
	vector<HTuple>           hv_vecHtWidth, hv_vecHtHeight;
	vector<HTuple>			 hv_vecHtRow, hv_vecHtColumn;
	vector<HTuple>           hv_vecModelID;
	vector<HObject>	         ho_vecShapeModel;
	HTuple                   hv_ModelID, hv_WindowHandle;
	/*RNG                      rng(time(0));
	Scalar m_scalarObjec = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));*/
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the hv angle check. </summary>
	///
	/// 
	///  run variables
	/// <value>	The hv angle check. </value>
	///-------------------------------------------------------------------------------------------------
	HTuple                    hv_RowCheck, hv_ColumnCheck, hv_AngleCheck,hv_Scale;
	HTuple                    hv_Score, hv_j, hv_MovementOfObject, hv_RowArrowHead;
	HTuple                    hv_ColumnArrowHead;
	HObject                   ho_ModelAtNewPosition;


#define m_strSrc              "..\\caliper\\test9\\src\\%d.jpg"
#define m_strDst              "..\\src\\current.jpg"
#define m_strModelDst         "..\\data\\%d.shm"
};
