#pragma once
#include "halcon_all.h"
#include <opencv_all.h>
#include "getFiles.h"
#include "matHalcon.h"

struct objectCurrentData
{
	vector<string>           m_veccurrentGoalName;
	vector<double>           m_vecCurrentAngle;
	vector<Point>            m_veccurrentGoalCoordinate;
	void clear()
	{
		m_veccurrentGoalName.clear();
		m_vecCurrentAngle.clear();
		m_veccurrentGoalCoordinate.clear();
	}
};

struct objectBasicData
{
	vector<HTuple>           hv_vecHtWidth, hv_vecHtHeight;
	vector<HTuple>			 hv_vecHtRows, hv_vecHtCols;
	vector<HTuple>           hv_vecModelID;

	void clear()
	{
		hv_vecHtWidth.clear();
		hv_vecHtHeight.clear();
		hv_vecHtRows.clear();
		hv_vecHtCols.clear();
		hv_vecModelID.clear();
	}
};

struct findModelStruct
{
	HTuple  m_htRows;
	HTuple  m_htCols;
	HTuple  m_htAngles;
	HTuple  m_htScore;
	double  m_dMinScore = 0.95;
	double  m_dMaxOverLap = 0.15;

	void clear()
	{
		m_htRows.Clear();
		m_htCols.Clear();
		m_htAngles.Clear();
		m_htScore.Clear();
		m_dMinScore = 0.95;
		m_dMaxOverLap = 0.15;
	}
};

class halconObjectRecognition
{
public:
	halconObjectRecognition();
	~halconObjectRecognition();
	void                     initialize();
	void                     clear();
	void                     updateMinScore(double m_updateMinScore);
	void					 updateMaxOverLap(double m_updateMaxOverLap);

	void                     addModel(Mat m_matImg, int m_iCurrentRoi_X, int m_iCurrentRoi_Y, int m_iCurrentCols, int m_iCurrentRows);
	Mat                      drawArrow(cv::Mat m_matImg, cv::Point pStart, cv::Point pEnd, double angle);
	vector<struct objectCurrentData>
		                     run(Mat m_matImg);
	std::vector<std::string> updateModelPath();
	double                   currentMinScore();
	double                   currentMaxOverLap();

	bool                     loadExternalModel(string m_strModelPath);
	bool                     deleteModel(string m_strModelPath);


private:
#define                        m_len  100
	//	const int                m_iModelImgNum = 1;
	//	const int                m_iImgNum = 10;
	const char               *m_charfileName;
	const int                m_iLen = 50;
	const int                m_iLineLen = 20;
	const int                m_iThickness = 2;
	const int                m_iLineType = 4;
	const double             m_dalpha = PI / 4;


	objectBasicData          m_objectBasicData;
	findModelStruct          m_findModelStruct; 

	Scalar                   m_scalarColor = Scalar(255, 0, 0);

	vector<HTuple>           hv_vecModelID;

	HTuple                   hv_ModelID;

	vector<string>           m_vecStrModelPath;
	vector<string>           m_vecStrCurrentModel;
	vector<struct objectCurrentData>
		                     m_vecObject;

	HObject                  ho_ModelAtNewPosition;
	string                   m_strModelPath = "\\model";
	getFiles                 m_getAllFiles;
	matHalcon                matToHalcon;
#define                      m_strModelName "model//%d%d%d%d%d%d.shm"	
};
