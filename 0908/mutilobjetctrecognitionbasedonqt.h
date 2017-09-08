#ifndef MUTILOBJETCTRECOGNITIONBASEDONQT_H
#define MUTILOBJETCTRECOGNITIONBASEDONQT_H

#include <QtWidgets/QMainWindow>
#include "ui_mutilobjetctrecognitionbasedonqt.h"
#include <qt_all.h>
#include <opencv_all.h>
#include "halconObjectRecognition.h"
#include "matHalcon.h"
#include "remapImage.h"
#include "ImageEnhancement.h"
class MutilObjetctRecognitionBasedOnQt : public QMainWindow
{
	Q_OBJECT

public:
	MutilObjetctRecognitionBasedOnQt(QWidget *parent = 0);
	~MutilObjetctRecognitionBasedOnQt();

private:
	Ui::MutilObjetctRecognitionBasedOnQtClass ui;
	QImage                     Mat2QImage(cv::Mat cvImg);
	bool                       mousePosition();
	void                       initialize();
	Mat                        imageEnhancement(Mat m_mat);
	Mat                        imageTransform(Mat m_mat);

private:

	QTimer                     *m_qTimer,*m_qCurrentTimer;
	QImage                     m_capQImg,m_srcQImg,m_tmpQImg;
	VideoCapture               m_capture;
	halconObjectRecognition    objectRecognition;
	matHalcon                  matToHalcon;
	Mat                        m_matImg,m_matTmpImg;
	Mat                        m_matSrcImg;
	Mat                        m_matCurrentImg;

	vector<string>             m_vecStrExistence;
	vector<string>             m_vecModelName;
	QPoint                     m_qPoint;

	Point                      m_pointOriginalPosition;//初始坐标   
	Rect                       m_recRectangle;//标记ROI的矩形框
	char                       m_charCurrrentPosition[m_len];
	char                       m_charDstImgName[m_len];
	char                       m_charRemove[m_len];
	char                       m_charCurrentTime[m_len];

	int                        m_icurrentRoi_X, m_icurrentRoi_Y, m_icurrentRoi_Width, m_icurrentRoi_Height;
	int                        m_iCurrentImgId = 0;
	int                        m_iPoint_X;
	int                        m_iPoint_Y;
	int                        m_iMinArea = 0;
	int                        m_iMaxArea = 0;
	int                        m_iSingleStep = 100;
	bool                       m_bCap = false, m_bDrawing=false;
	QTextCodec                 *codec = QTextCodec::codecForName("GB18030");
	
	double                     m_dInitialMinScore,m_dInitialMaxLap,m_dCurrentFps=0;
	
	Scalar                     m_scalarColor = Scalar(255, 0, 255);
	time_t                     m_currentTime;
	
	remapImage                 remapMatImage;
	string                     m_strFileStorage = "..//data//data.xml";
	Size                       m_Size = Size(360, 270);
	FileStorage                m_fileStorage;

#define m_strDstImg            "image\\%d.jpg"

private slots:
void runDemo();
void on_openButton_clicked();
void on_closeButton_clicked();
void on_loadButton_clicked();
void on_loadModelButton_clicked();
void on_removeModelButton_clicked();
void on_screenShootButton_clicked();
void on_minScoreOkButton_clicked();
void on_maxOverLapOkButton_clicked();
void on_clearButton_clicked();
void updateData();


protected:
	void mousePressEvent(QMouseEvent *e);       //--鼠标按下事件  
	void mouseMoveEvent(QMouseEvent *e);    //--鼠标移动事件  
	void mouseReleaseEvent(QMouseEvent *e); //--鼠标释放（松开）事件  
	void mouseDoubleClickEvent(QMouseEvent *e); //--鼠标双击事件
 
};

#endif // MUTILOBJETCTRECOGNITIONBASEDONQT_H
