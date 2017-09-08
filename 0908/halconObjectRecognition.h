#pragma once
#include <halcon_all.h>
#include <opencv_all.h>
#include "getFiles.h"
class halconObjectRecognition
{
public:
	halconObjectRecognition();
	~halconObjectRecognition();
	void         initialize();

	Mat                      run(HObject ho_SearchImage, Mat m_matImg);
	void                     setUpModel(char m_charFileName[100], int m_iCurrentFlag, int m_iCurrentImgId);
	void                     upDateNewData(char m_charFileName[100], int m_iCurrentRoi_X, int m_iCurrentRoi_Y, int m_iCurrentCols, int m_iCurrentRows, int m_iCurrentImgId);
	Mat                      drawArrow(cv::Mat& img, cv::Point pStart, cv::Point pEnd,
		                      double angle);
	void                     clear();
	double                   minScore();
	void                     updateMinScore(double m_updateMinScore);
	double                   maxOverLap();
	void					 updateMaxOverLap(double m_updateMaxOverLap);
	bool                     loadModel(string m_strModelPath);
	bool                     removdeModel(string m_strModelPath);
	void                     stringToChar(string m_strModelPath);
	vector<string>           updateModel();

private:
#define                        m_len  100
//	const int                m_iModelImgNum = 1;
//	const int                m_iImgNum = 10;
	char                     m_charfileName[100];
    double 			         m_dMinScore = 0.95;
    double 			         m_dMaxOverLap = 0.15;
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
	HObject                  ho_ModelTrans;
	HTuple                   hv_ModelID, hv_WindowHandle;
	HTuple                   hv_Area, hv_RowRef, hv_ColumnRef, hv_HomMat2D;
	vector<string>           m_vecStrModelName;
	
	/*RNG                      rng(time(0));
	Scalar m_scalarObjec = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));*/
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the hv angle check. </summary>
	///
	/// 
	///  run variables
	/// <value>	The hv angle check. </value>
	///-------------------------------------------------------------------------------------------------
	HTuple    hv_RowCheck, hv_ColumnCheck, hv_AngleCheck;
	HTuple    hv_Score, hv_j, hv_MovementOfObject, hv_RowArrowHead;
	HTuple    hv_ColumnArrowHead;
	HObject   ho_ModelAtNewPosition;
#define  m_strModelName "model//%d%d%d%d%d%d.shm"	
};
