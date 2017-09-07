#include "halconObjectRecognition.h"


halconObjectRecognition::halconObjectRecognition()
{
}


halconObjectRecognition::~halconObjectRecognition()
{
}

void halconObjectRecognition::initialize()
{
	clear();
}
Mat halconObjectRecognition::run(HObject ho_SearchImage, Mat m_matImg)
{
	if (hv_vecModelID.size() == 0)
	{
		return m_matImg;
	}
	
	for (int j = 0; j < hv_vecHtRow.size(); ++j)
	{
		
		FindShapeModel(ho_SearchImage, hv_vecModelID[j], 0, HTuple(360).TupleRad(), m_dMinScore, 0,
			m_dMaxOverLap, "least_squares", 0, 0., &hv_RowCheck, &hv_ColumnCheck, &hv_AngleCheck,
			&hv_Score);

		//{
		//	HTuple end_val47 = (hv_Score.TupleLength()) - 1;
		//	HTuple step_val47 = 1;
		//	for (hv_j = 0; hv_j.Continue(end_val47, step_val47); hv_j += step_val47)
		//	{
		//		VectorAngleToRigid(0, 0, 0, HTuple(hv_RowCheck[hv_j]), HTuple(hv_ColumnCheck[hv_j]),
		//			HTuple(hv_AngleCheck[hv_j]), &hv_MovementOfObject);
		//		AffineTransContourXld(ho_vecShapeModel[j], &ho_ModelAtNewPosition, hv_MovementOfObject);
		//		/*if (HDevWindowStack::IsOpen())
		//		SetColor(HDevWindowStack::GetActive(), "cyan");
		//		if (HDevWindowStack::IsOpen())
		//		DispObj(ho_ModelAtNewPosition, HDevWindowStack::GetActive());
		//		if (HDevWindowStack::IsOpen())
		//		SetColor(HDevWindowStack::GetActive(), "blue");*/
		//		AffineTransPixel(hv_MovementOfObject, -120, 0, &hv_RowArrowHead, &hv_ColumnArrowHead);
		//		DispArrow(hv_WindowHandle, HTuple(hv_RowCheck[hv_j]), HTuple(hv_ColumnCheck[hv_j]),
		//			hv_RowArrowHead, hv_ColumnArrowHead, 2);
		//		/*if (HDevWindowStack::IsOpen())
		//		disp_message(hv_WindowHandle, "Matching: Time", "image", 50, 50, "white", "false");*/
		//		//disp_message(hv_WindowHandle, "Hello World!", "image", 10, 10, "blue", "false");
		//		
		//	}
		//}

		if (hv_Score.Length() > 0)
		{

			double m_dAngle = hv_AngleCheck[0].D();
			double m_dCurrent_Y = hv_RowCheck[0].D();
			double m_dCurrent_X = hv_ColumnCheck[0].D();
			if (m_dAngle > PI)
			{
				m_dAngle = m_dAngle - 2 * PI;
			}
			/*cout << "The current ID is :" << j << endl;
			cout << "The current Coordinate is:(" << m_dCurrent_X << "," << m_dCurrent_Y << ")" << endl;
			cout << "The angle is:" << m_dAngle * 180 / PI << endl;
			cout << "The similiarity value is :" << hv_Score[0].D() << endl;*/

			
			m_matImg = drawArrow(m_matImg, Point(m_dCurrent_X, m_dCurrent_Y), Point(m_dCurrent_X + m_iLen*cos(3 * PI / 2 - m_dAngle), m_dCurrent_Y + m_iLen*sin(3 * PI / 2 - m_dAngle)), PI / 2 - m_dAngle);
			char m_charOut[100];
			sprintf(m_charOut, "(%.1f,%.1f,%.1f)", m_dCurrent_X, m_dCurrent_Y, m_dAngle * 180 / PI);
			putText(m_matImg, m_charOut, Point(m_dCurrent_X, m_dCurrent_Y), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 255), 1);
			sprintf(m_charOut, "ID:%d", j);
			putText(m_matImg, m_charOut, Point(m_dCurrent_X, m_dCurrent_Y+10), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 255), 1);
			//imshow("drawArrow", m_matImg);
		}
		//disp_continue_message(hv_WindowHandle, "black", "true");
		//	Sleep(2000);
	}

	if (hv_Score.Length() <= 0)
	{
		cout << "No Matching " << endl;
	}
	return m_matImg;
}


Mat halconObjectRecognition::drawArrow(cv::Mat& img, cv::Point pStart, cv::Point pEnd, double angle)
{
	Point arrow;

	//计算箭角边的另一端的端点位置（上面的还是下面的要看箭头的指向，也就是pStart和pEnd的位置） 
	line(img, pStart, pEnd, m_scalarColor, m_iThickness, m_iLineType);

	arrow.x = pEnd.x + m_iLineLen * cos(angle + m_dalpha );
	arrow.y = pEnd.y + m_iLineLen * sin(angle + m_dalpha );
	line(img, pEnd, arrow, m_scalarColor, m_iThickness, m_iLineType);

	arrow.x = pEnd.x + m_iLineLen * cos(angle - m_dalpha );
	arrow.y = pEnd.y + m_iLineLen * sin(angle - m_dalpha );
	line(img, pEnd, arrow, m_scalarColor, m_iThickness, m_iLineType);
	return img;

}

void halconObjectRecognition::clear()
{
	hv_vecHtWidth.clear(), hv_vecHtHeight.clear();
	hv_vecHtRow.clear(), hv_vecHtColumn.clear();
	hv_vecModelID.clear();
	ho_vecShapeModel.clear();
	m_vecModelName.clear();
	m_dMinScore = 0.95;
	m_dMaxOverLap = 0.15;
}

double halconObjectRecognition::minScore()
{
	return m_dMinScore;
}

void halconObjectRecognition::updateMinScore(double m_updateMinScore)
{
	m_dMinScore = m_updateMinScore;
}

double halconObjectRecognition::maxOverLap()
{
	return  m_dMaxOverLap;
}

void halconObjectRecognition::updateMaxOverLap(double m_updateMaxOverLap)
{
	m_dMaxOverLap = m_updateMaxOverLap;
}

bool halconObjectRecognition::loadModel(string m_strModelPath)
{
	for (int i = 0; i < m_vecModelName.size(); i++)
	{
		if (m_strModelPath == m_vecModelName[i])
		{
			return false;
		}
	}
	int m_iFirstSize = hv_vecModelID.size();
	m_strModelPath.copy(m_charfileName, m_strModelPath.length());
	m_vecModelName.push_back(m_charfileName);
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

bool halconObjectRecognition::removdeModel(string m_strModelPath)
{
	int m_iFirstSize = hv_vecModelID.size();
	m_strModelPath.copy(m_charfileName, m_strModelPath.length());

	bool m_bNameExistence = false;
	vector<string>::iterator it;
	vector<HObject>::iterator ho_obID;
	for (it = m_vecModelName.begin() ; it != m_vecModelName.end();)
	{

		if (*it == m_charfileName)
		{
			it = m_vecModelName.erase(it);
			//ho_obID = hv_vecModelID.erase(ho_obID);
			m_bNameExistence = true;
		}
		else
		{
			++it;
			//++ho_obID;
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

void halconObjectRecognition::upDateNewData(char m_charFileName[100], int m_iCurrentRoi_X, int m_iCurrentRoi_Y, int m_iCurrentCols, int m_iCurrentRows, int m_iCurrentImgId)
{
	hv_vecHtRow.push_back(m_iCurrentRoi_Y);
	hv_vecHtColumn.push_back(m_iCurrentRoi_X);
	hv_vecHtWidth.push_back(m_iCurrentRows + m_iCurrentRoi_Y);
	hv_vecHtHeight.push_back(m_iCurrentCols + m_iCurrentRoi_X);
	setUpModel(m_charFileName, hv_vecHtRow.size() - 1, m_iCurrentImgId);
}


void halconObjectRecognition::setUpModel(char m_charFileName[100], int m_iCurrentSize, int m_iCurrentImgId)
{
	// Local iconic variables
	HObject  ho_ModelImage, ho_ModelROI;
	HObject	 ho_ImageROI;
	HObject  ho_ShapeModelImage, ho_ShapeModelRegion, ho_ShapeModel;

	// Local control variables
	HTuple   hv_FGHandle, hv_Pointer, hv_Type;
	HTuple   hv_Width, hv_Height;

	OpenFramegrabber("File", 1, 1, 0, 0, 0, 0, "default", -1, "default", -1, "default",
		m_charFileName, "default", -1, 1, &hv_FGHandle);
	GrabImage(&ho_ModelImage, hv_FGHandle);


	GetImagePointer1(ho_ModelImage, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	GenRectangle1(&ho_ModelROI, hv_vecHtRow[m_iCurrentSize], hv_vecHtColumn[m_iCurrentSize], hv_vecHtWidth[m_iCurrentSize], hv_vecHtHeight[m_iCurrentSize]);
	ReduceDomain(ho_ModelImage, ho_ModelROI, &ho_ImageROI);
	InspectShapeModel(ho_ImageROI, &ho_ShapeModelImage, &ho_ShapeModelRegion, 1, 30);
	
	/*HObject ho_binThreshold, ho_ConnectedRegions, ho_SelectedRegions, ho_RegionFillUp, ho_RegionDilation;
	BinThreshold(ho_ImageROI, &ho_binThreshold);
	Connection(ho_binThreshold, &ho_ConnectedRegions);
	SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 10, 50000);
	FillUp(ho_SelectedRegions, &ho_RegionFillUp);
	DilationCircle(ho_RegionFillUp, &ho_RegionDilation, 3.3);


	ReduceDomain(ho_ModelImage, ho_RegionDilation, &ho_ImageROI);*/
	CreateShapeModel(ho_ImageROI, "auto", 0, HTuple(360).TupleRad(), "auto", "none",
		"ignore_global_polarity", 30, 10, &hv_ModelID);

	hv_vecModelID.push_back(hv_ModelID);
	ho_vecShapeModel.push_back(ho_ShapeModel);
	if (m_iCurrentImgId > 0)
	{
		sprintf(m_charFileName, m_strModelDst, m_iCurrentImgId);
		WriteShapeModel(hv_ModelID, m_charFileName);
	}
	 
}



