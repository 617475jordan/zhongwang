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

	void clear(int m_iFlag)
	{
		switch (m_iFlag)
		{
		case 0:
			m_dMinScore = 0.95;
			m_dMaxOverLap = 0.15;
		case 1:
			m_htRows.Clear();
			m_htCols.Clear();
			m_htAngles.Clear();
			m_htScore.Clear();
		default:
			break;
		}
	}
};
class halconObjectRecognition
{
public:

	void					initialize(string m_strModelPath);
	vector<struct objectCurrentData>  
		                    run(Mat m_matImg);
	void					clear();
	void					addModel(Mat m_matImg, int m_iCurrentRoi_X, int m_iCurrentRoi_Y, int m_iCurrentCols, int m_iCurrentRows);
	bool					deleteModel(string m_strModelPath);
	
	halconObjectRecognition();
	~halconObjectRecognition();	

private:
	bool					loadExternalModel(string m_strModelPath);

private:
	const char               *m_pcharModelPath = NULL;
	
	Scalar                   m_scalarColor = Scalar(255, 0, 0);
	objectBasicData          m_objectBasicData;
	findModelStruct          m_findModelStruct;

	vector<string>           m_vecStrModelPath;
	vector<string>           m_vecStrCurrentModel;
	vector<struct objectCurrentData>  
		                     m_vecObject;
		
	HTuple                   m_htModelID;
	getFiles                 m_getAllFiles;
	matHalcon                matToHalcon;

	string                   m_strSavePathName = "//%d%d%d%d%d%d.shm";
};
