#include "halconObjectRecognition.h"
#include "stringReplace.h"

halconObjectRecognition::halconObjectRecognition()
{
}


halconObjectRecognition::~halconObjectRecognition()
{
}

void   halconObjectRecognition::initialize()
{
	clear();
	/**********获取当前位置**********************/
	char m_charCurrentPosition[100];
	_getcwd(m_charCurrentPosition, 100);

	string m_strCurrentPosition = m_charCurrentPosition + m_strModelPath;
	

	m_getAllFiles.getSpecialTypeFiles(m_strCurrentPosition, m_vecStrModelPath, ".shm");
	m_vecStrCurrentModel = m_getAllFiles.currentModel();
	if (m_vecStrModelPath.size() > 0)
	{
		stringReplace stringReplace;
		for (int i = 0; i < m_vecStrModelPath.size(); i++)
		{
			m_vecStrModelPath[i] = stringReplace.replace_all_distinct(m_vecStrModelPath[i], "\\", "/");
			m_charfileName = NULL;
			m_charfileName = m_vecStrModelPath[i].data();
			
			ReadShapeModel(m_charfileName, &hv_ModelID);
			hv_vecModelID.push_back(hv_ModelID);
		}
	}
}
vector<struct objectCurrentData> halconObjectRecognition::run(Mat m_matImg)
{
	m_vecObject.clear();

	objectCurrentData objectRec;
	HObject ho_SearchImage;

	ho_SearchImage = matToHalcon.IplImageToHImage(m_matImg);
	if (m_objectBasicData.hv_vecModelID.size() == 0)
	{
		return m_vecObject;
	}
	bool m_bOk = false;
	for (int j = 0; j < m_objectBasicData.hv_vecModelID.size(); ++j)
	{

		FindShapeModel(ho_SearchImage, m_objectBasicData.hv_vecModelID[j], 0, HTuple(360).TupleRad(), m_findModelStruct.m_dMinScore, 0,
			m_findModelStruct.m_dMaxOverLap, "least_squares", 0, 0.,
			&m_findModelStruct.m_htRows, &m_findModelStruct.m_htCols,
			&m_findModelStruct.m_htAngles, &m_findModelStruct.m_htScore);

		if (m_findModelStruct.m_htScore.Length() > 0)
		{

			double m_dAngle = m_findModelStruct.m_htAngles[0].D();
			double m_dCurrent_Y = m_findModelStruct.m_htRows[0].D();
			double m_dCurrent_X = m_findModelStruct.m_htCols[0].D();
			if (m_dAngle > PI)
			{
				m_dAngle = m_dAngle - 2 * PI;
			}
			objectRec.m_vecCurrentAngle.push_back(m_dAngle);
			objectRec.m_veccurrentGoalName.push_back(m_vecStrCurrentModel[j]);
			objectRec.m_veccurrentGoalCoordinate.push_back(Point(m_dCurrent_X, m_dCurrent_Y));
			m_bOk = true;
		}
	}
	if (m_bOk == true)
	{
		m_vecObject.push_back(objectRec);
	}
	objectRec.clear();
	return m_vecObject;
}


Mat halconObjectRecognition::drawArrow(cv::Mat  m_matImg, cv::Point pStart, cv::Point pEnd, double angle)
{
	Point arrow;

	//计算箭角边的另一端的端点位置（上面的还是下面的要看箭头的指向，也就是pStart和pEnd的位置） 
	line(m_matImg, pStart, pEnd, m_scalarColor, m_iThickness, m_iLineType);

	arrow.x = pEnd.x + m_iLineLen * cos(angle + m_dalpha);
	arrow.y = pEnd.y + m_iLineLen * sin(angle + m_dalpha);
	line(m_matImg, pEnd, arrow, m_scalarColor, m_iThickness, m_iLineType);

	arrow.x = pEnd.x + m_iLineLen * cos(angle - m_dalpha);
	arrow.y = pEnd.y + m_iLineLen * sin(angle - m_dalpha);
	line(m_matImg, pEnd, arrow, m_scalarColor, m_iThickness, m_iLineType);
	return m_matImg;

}

void halconObjectRecognition::clear()
{
	m_objectBasicData.clear();
	m_vecStrModelPath.clear();
	m_findModelStruct.clear();
	m_charfileName = NULL;
	delete[] m_charfileName;
}



bool halconObjectRecognition::loadExternalModel(string m_strModelPath)
{

	for (int i = 0; i < m_vecStrModelPath.size(); i++)
	{
		if (m_strModelPath == m_vecStrModelPath[i])
		{
			return false;
		}
	}
	int m_iFirstSize = hv_vecModelID.size();
	m_charfileName = NULL;
	m_charfileName = m_strModelPath.data();
	m_vecStrModelPath.push_back(m_charfileName);
	ReadShapeModel(m_charfileName, &hv_ModelID);
	hv_vecModelID.push_back(hv_ModelID);

	m_strModelPath.clear();
	int m_iSecondSize = hv_vecModelID.size();
	if (m_iSecondSize > m_iFirstSize)
	{

		return true;
	}
	else
	{
		return false;
	}
}

bool halconObjectRecognition::deleteModel(string m_strModelPath)
{
	int m_iFirstSize = hv_vecModelID.size();

	vector<string>::iterator it;
	vector<string>::iterator it_model;
	vector<HTuple>::iterator id;
	for (it = m_vecStrModelPath.begin(), id = hv_vecModelID.begin(); it != m_vecStrModelPath.end(), id != hv_vecModelID.end();)
	{

		if (*it == m_strModelPath)
		{
			it = m_vecStrModelPath.erase(it);
			id = hv_vecModelID.erase(id);
		}
		else
		{
			++it;
			++id;
		}
	}

	m_strModelPath.clear();
	int m_iSecondSize = hv_vecModelID.size();
	if (m_iSecondSize < m_iFirstSize)
	{
		return true;
	}
	else
	{
		return false;
	}
}



void halconObjectRecognition::addModel(Mat m_matImg, int m_iCurrentRoi_X, int m_iCurrentRoi_Y, int m_iCurrentCols, int m_iCurrentRows)
{
	HObject  ho_ModelImage;
	m_objectBasicData.hv_vecHtRows.push_back(m_iCurrentRoi_Y);
	m_objectBasicData.hv_vecHtCols.push_back(m_iCurrentRoi_X);
	m_objectBasicData.hv_vecHtWidth.push_back(m_iCurrentRows + m_iCurrentRoi_Y);
	m_objectBasicData.hv_vecHtHeight.push_back(m_iCurrentCols + m_iCurrentRoi_X);


	// Local iconic variables
	HObject   ho_ModelROI;
	HObject	  ho_ImageROI;
	HObject   ho_ShapeModelImage, ho_ShapeModelRegion, ho_ShapeModel;

	// Local control variables
	HTuple    hv_Pointer, hv_Type;
	HTuple    hv_Width, hv_Height;

	int m_iCurrentSize = m_objectBasicData.hv_vecHtCols.size() - 1;

	GetImagePointer1(ho_ModelImage, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	GenRectangle1(&ho_ModelROI, m_objectBasicData.hv_vecHtRows[m_iCurrentSize],
		m_objectBasicData.hv_vecHtCols[m_iCurrentSize],
		m_objectBasicData.hv_vecHtWidth[m_iCurrentSize], 
		m_objectBasicData.hv_vecHtHeight[m_iCurrentSize]);

	ReduceDomain(ho_ModelImage, ho_ModelROI, &ho_ImageROI);

	CreateShapeModel(ho_ImageROI, "auto", 0, HTuple(360).TupleRad(), "auto", "none",
		"ignore_global_polarity", 30, 10, &hv_ModelID);
	hv_vecModelID.push_back(hv_ModelID);
	/*if (m_iCurrentImgId > 0)
	{*/
	time_t m_currentTime = time(0);
	struct tm * timeinfo;
	timeinfo = localtime(&m_currentTime);
	char m_charDstModelName[100];
	sprintf(m_charDstModelName, m_strModelName, timeinfo->tm_year, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	WriteShapeModel(hv_ModelID, m_charDstModelName);
	m_vecStrModelPath.push_back(m_charDstModelName);
	m_vecStrCurrentModel.push_back(m_charDstModelName);
	stringReplace stringReplace;
	m_vecStrCurrentModel[m_vecStrCurrentModel.size() - 1] = stringReplace.replace_all_distinct(m_vecStrCurrentModel[m_vecStrCurrentModel.size() - 1], "model//", "");
	/*}*/
}


double halconObjectRecognition::currentMinScore()
{
	return m_findModelStruct.m_dMinScore;
}

void halconObjectRecognition::updateMinScore(double m_updateMinScore)
{
	m_findModelStruct.m_dMinScore = m_updateMinScore;
}

double halconObjectRecognition::currentMaxOverLap()
{
	return  m_findModelStruct.m_dMaxOverLap;
}

void halconObjectRecognition::updateMaxOverLap(double m_updateMaxOverLap)
{
	m_findModelStruct.m_dMaxOverLap = m_updateMaxOverLap;
}
std::vector<std::string> halconObjectRecognition::updateModelPath()
{
	return m_vecStrModelPath;
}

