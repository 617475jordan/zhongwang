#ifndef MUTILOBJETCTRECOGNITIONBASEDONQT_H
#define MUTILOBJETCTRECOGNITIONBASEDONQT_H

#include <QtWidgets/QMainWindow>
#include "ui_mutilobjetctrecognitionbasedonqt.h"
#include <qt_all.h>
#include <opencv_all.h>
#include "myKinect.h"
#include "halconObjectRecognition.h"
#include "matHalcon.h"
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
private:
	QTimer                     *m_qTimer;
	QImage                     m_capQImg,m_srcQImg;
	VideoCapture               m_capture;
	halconObjectRecognition    objectRecognition;
	matHalcon                  matToHalcon;
	Mat                        m_matImg;
	Mat                        m_matSrcImg;
	vector<string>             m_vecStrExistence;

	CBodyBasics                myKinect;
	QPoint                     m_qPoint;

	Point                      m_pointOriginalPosition;//��ʼ����   
	Rect                       m_recRectangle;//���ROI�ľ��ο�
	char                       m_charCurrrentPosition[100];
	char                       m_charDstImg[100];
	char                       m_charRemove[100];
	bool                       m_bDrawing;
	int                        m_icurrentRoi_X, m_icurrentRoi_Y, m_icurrentRoi_Width, m_icurrentRoi_Height;
	int                        m_iCurrentImgId = 1;
	Mat                        roi;//ROIͼ��
	Mat                        m_matCurrentImg;
	bool                       m_bCap=false;
	QTextCodec                 *codec = QTextCodec::codecForName("GB18030");
	const int                  m_iPoint_X = 320;
	const int                  m_iPoint_Y = 320;
	Size                       m_sizeImg = Size(m_iPoint_X, m_iPoint_Y);
	double                     m_dInitialSimilariy;
	string                     m_strRemove;
#define m_strDstImg           "..\\dst\\%d.jpg"
#define m_strSrcImg	          "..\\src\\current.jpg"

private slots:
void showImage();
void on_openButton_clicked();
void on_closeButton_clicked();
void on_loadButton_clicked();
void on_removeButton_clicked();
void on_okButton_clicked();
void on_clearButton_clicked();

protected:
	void mousePressEvent(QMouseEvent *e);       //--��갴���¼�  
	void mouseMoveEvent(QMouseEvent *e);    //--����ƶ��¼�  
	void mouseReleaseEvent(QMouseEvent *e); //--����ͷţ��ɿ����¼�  
	void mouseDoubleClickEvent(QMouseEvent *e); //--���˫���¼�
private:
	QLabel *statusLabel;                //---��ʾ����ƶ�ʱ��ʵʱλ��   
	QLabel *mousePointLabel;        //---��ʾ���λ��
};

#endif // MUTILOBJETCTRECOGNITIONBASEDONQT_H
