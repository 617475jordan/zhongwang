#include "halconObjectRecognition.h"
#include "stringReplace.h"

halconObjectRecognition::halconObjectRecognition()
{
}


halconObjectRecognition::~halconObjectRecognition()
{
}

void   halconObjectRecognition::initialize(string m_strModelPath)
{
	clear();
	/**********获取当前位置**********************/
	char m_charCurrentPosition[100];
	_getcwd(m_charCurrentPosition, 100);

	string m_strCurrentPosition = m_charCurrentPosition + m_strModelPath;
    m_strSavePathName = m_strCurrentPosition+m_strSavePathName;

	stringReplace stringReplace;

	m_strSavePathName = stringReplace.replace_all_distinct(m_strSavePathName, "\\", "/");
	m_getAllFiles.getSpecialTypeFiles(m_strCurrentPosition, m_vecStrModelPath, ".shm");
	m_vecStrCurrentModel.clear();

	m_vecStrCurrentModel = m_getAllFiles.currentModel();
	if (m_vecStrModelPath.size() > 0)
	{
		
		for (int i = 0; i < m_vecStrModelPath.size(); i++)
		{
			m_vecStrModelPath[i] = stringReplace.replace_all_distinct(m_vecStrModelPath[i], "\\", "/");
			m_pcharModelPath = NULL;
			m_pcharModelPath = m_vecStrModelPath[i].data();
			ReadShapeModel(m_pcharModelPath, &m_htModelID);
			hv_vecModelID.push_back(m_htModelID);
		}
	}
}
void halconObjectRecognition::run(Mat m_matImg)
{
	HObject ho_SearchImage;
	ho_SearchImage = matToHalcon.IplImageToHImage(m_matImg);
	if (hv_vecModelID.size() == 0)
	{
		return;
	}
	m_veccurrentGoalCoordinate.clear();
	m_veccurrentGoalName.clear();
	for (int j = 0; j < hv_vecModelID.size(); ++j)
	{

		FindShapeModel(ho_SearchImage, hv_vecModelID[j], 0, HTuple(360).TupleRad(), m_dMinScore, 0,
			m_dMaxOverLap, "least_squares", 0, 0., &m_htRows, &m_htCols, &m_htAngles,
			&m_htScore);

		if (m_htScore.Length() > 0)
		{

			double m_dAngle = m_htAngles[0].D();
			double m_dCurrent_Y = m_htRows[0].D();
			double m_dCurrent_X = m_htCols[0].D();
			if (m_dAngle > PI)
			{
				m_dAngle = m_dAngle - 2 * PI;
			}
			m_vecCurrentAngle.push_back(m_dAngle);
			m_veccurrentGoalName.push_back(m_vecStrCurrentModel[j]);
			m_veccurrentGoalCoordinate.push_back(Point(m_dCurrent_X, m_dCurrent_Y));
		}
	}
}



void halconObjectRecognition::clear()
{
	hv_vecHtWidth.clear(), hv_vecHtHeight.clear();
	hv_vecHtRows.clear(), hv_vecHtCols.clear();
	hv_vecModelID.clear();
	ho_vecShapeModel.clear();
	m_vecStrModelPath.clear();
	m_vecCurrentAngle.clear();
	m_dMinScore = 0.95;
	m_dMaxOverLap = 0.15;
	m_pcharModelPath = NULL;
	delete[] m_pcharModelPath;
	
	
}



bool halconObjectRecognition::addModel(string m_strModelPath)
{

	for (int i = 0; i < m_vecStrModelPath.size(); i++)
	{
		if (m_strModelPath == m_vecStrModelPath[i])
		{
			return false;
		}
	}
	int m_iFirstSize = hv_vecModelID.size();
	m_pcharModelPath = NULL;
	m_pcharModelPath = m_strModelPath.data();
	m_vecStrModelPath.push_back(m_pcharModelPath);
	ReadShapeModel(m_pcharModelPath, &m_htModelID);
	hv_vecModelID.push_back(m_htModelID);

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



void halconObjectRecognition::setUpModel(Mat m_matImg, int m_iCurrentRoi_X, int m_iCurrentRoi_Y, int m_iCurrentCols, int m_iCurrentRows)
{
	HObject  ho_ModelImage = matToHalcon.IplImageToHImage(m_matImg);
	hv_vecHtRows.push_back(m_iCurrentRoi_Y);
	hv_vecHtCols.push_back(m_iCurrentRoi_X);
	hv_vecHtWidth.push_back(m_iCurrentRows + m_iCurrentRoi_Y);
	hv_vecHtHeight.push_back(m_iCurrentCols + m_iCurrentRoi_X);


	// Local iconic variables
	HObject   ho_ModelROI;
	HObject	  ho_ImageROI;
	HObject   ho_ShapeModelImage, ho_ShapeModelRegion, ho_ShapeModel;

	// Local control variables
	HTuple    hv_Pointer, hv_Type;
	HTuple    hv_Width, hv_Height;

	int m_iCurrentSize = hv_vecHtRows.size() - 1;

	GetImagePointer1(ho_ModelImage, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	GenRectangle1(&ho_ModelROI, hv_vecHtRows[m_iCurrentSize], hv_vecHtCols[m_iCurrentSize], hv_vecHtWidth[m_iCurrentSize], hv_vecHtHeight[m_iCurrentSize]);
	ReduceDomain(ho_ModelImage, ho_ModelROI, &ho_ImageROI);
	InspectShapeModel(ho_ImageROI, &ho_ShapeModelImage, &ho_ShapeModelRegion, 1, 30);

	CreateShapeModel(ho_ImageROI, "auto", 0, HTuple(360).TupleRad(), "auto", "none",
		"ignore_global_polarity", 30, 10, &m_htModelID);
	hv_vecModelID.push_back(m_htModelID);
	ho_vecShapeModel.push_back(ho_ShapeModel);
	/*if (m_iCurrentImgId > 0)
	{*/
	time_t m_currentTime = time(0);
	struct tm * timeinfo;
	timeinfo = localtime(&m_currentTime);
	char  m_charDstModelName[100];
	const char *m_pcharSavePathName = NULL;
	m_pcharSavePathName = m_strSavePathName.data();

	sprintf(m_charDstModelName, m_pcharSavePathName, timeinfo->tm_year, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	WriteShapeModel(m_htModelID, m_charDstModelName);

	m_vecStrModelPath.push_back(m_charDstModelName);
	m_vecStrCurrentModel.push_back(m_charDstModelName);
	stringReplace stringReplace;
	m_vecStrCurrentModel[m_vecStrCurrentModel.size() - 1] = stringReplace.replace_all_distinct(m_vecStrCurrentModel[m_vecStrCurrentModel.size() - 1], "model//", "");
	m_pcharSavePathName = NULL;
	delete[] m_pcharSavePathName;
}



std::vector<std::string> halconObjectRecognition::updateModelPath()
{
	return m_vecStrModelPath;
}

std::vector<Point> halconObjectRecognition::currentGoalCoordinate()
{
	return m_veccurrentGoalCoordinate;
}

std::vector<std::string> halconObjectRecognition::currentGoalName()
{
	return m_veccurrentGoalName;
}


vector<double>halconObjectRecognition::currentAngle()
{
	return m_vecCurrentAngle;
}


