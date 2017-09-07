#pragma once
#include <halcon_all.h>
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
	void                     run(HObject ho_SearchImage);
	void                     setUpModel(char m_charFileName[100],int m_iCurrentFlag);
	void                     upDateNewData(char m_charFileName[100], int m_iCurrentRoi_X, int m_iCurrentRoi_Y, int m_iCurrentCols, int m_iCurrentRows);
private:
	const int                m_iModelImgNum    = 1;
	const int                m_iImgNum         =10;
	char                     m_charfileName   [100];
	const double 			 m_dMinScore       = 0.95;
	const double 			 m_dMaxOverLap     = 0.15;
	vector<HTuple>           hv_vecHtWidth, hv_vecHtHeight;
	vector<HTuple>			 hv_vecHtRow, hv_vecHtColumn;
	vector<HTuple>           hv_vecModelID;
	vector<HObject>	         ho_vecShapeModel;
	HTuple                   hv_ModelID, hv_WindowHandle;

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the hv angle check. </summary>
	///
	/// 
	///  run variables
	/// <value>	The hv angle check. </value>
	///-------------------------------------------------------------------------------------------------
	HTuple    hv_RowCheck, hv_ColumnCheck, hv_AngleCheck;
	HTuple   hv_Score, hv_j, hv_MovementOfObject, hv_RowArrowHead;
	HTuple   hv_ColumnArrowHead;
	HObject   ho_ModelAtNewPosition;
	

#define m_strSrc  "..\\caliper\\test9\\src\\%d.jpg"
#define m_strDst  "..\\src\\current.jpg"
};

