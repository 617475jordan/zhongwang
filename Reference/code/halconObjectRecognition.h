#pragma once
#include "halcon_all.h"
#include <opencv_all.h>
#include "getFiles.h"
#include "matHalcon.h"
class halconObjectRecognition
{
public:
	halconObjectRecognition();
	~halconObjectRecognition();
	void                     initialize(string m_strModelPath);
	void                     run( Mat m_matImg);
	void                     clear();
	void                     setUpModel(Mat m_matImg, int m_iCurrentRoi_X, int m_iCurrentRoi_Y, int m_iCurrentCols, int m_iCurrentRows);
	


	bool                     addModel(string m_strModelPath);
	bool                     deleteModel(string m_strModelPath);

	vector<string>           updateModelPath();
	vector<string>           currentGoalName();				  //反馈当前识别到目标与模板匹配的名字
	vector<Point>            currentGoalCoordinate();		  //反馈当前识别到目标的坐标
	vector<double>           currentAngle();

private:
	const char               *m_pcharModelPath = NULL;
	
	double 			         m_dMinScore = 0.95;
	double 			         m_dMaxOverLap = 0.15;
	
	Scalar                   m_scalarColor = Scalar(255, 0, 0);

	vector<HTuple>           hv_vecHtWidth, hv_vecHtHeight;
	vector<HTuple>			 hv_vecHtRows, hv_vecHtCols;
	vector<HTuple>           hv_vecModelID;
	vector<HObject>	         ho_vecShapeModel;
	
	vector<string>           m_vecStrModelPath;
	vector<string>           m_veccurrentGoalName;
	vector<string>           m_vecStrCurrentModel;
	vector<double>           m_vecCurrentAngle;
	vector<Point>            m_veccurrentGoalCoordinate;
	
	HTuple                   m_htRows, m_htCols, m_htAngles,m_htScore;
	HTuple                   m_htModelID;
	getFiles                 m_getAllFiles;
	matHalcon                matToHalcon;
	string                   m_strSavePathName = "//%d%d%d%d%d%d.shm";
};
