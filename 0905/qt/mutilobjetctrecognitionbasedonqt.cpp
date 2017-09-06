#include "mutilobjetctrecognitionbasedonqt.h"

MutilObjetctRecognitionBasedOnQt::MutilObjetctRecognitionBasedOnQt(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);  
	m_qTimer = new QTimer(this);
	connect(m_qTimer, SIGNAL(timeout()), this, SLOT(showImage()));  // ʱ�䵽����ȡ��ǰ����ͷ��Ϣ

	if (m_bCap == true)
	{
		//----Qt5�����������  
		//QTextCodec *codec = QTextCodec::codecForName("GB18030");

		//---��ʾ����ƶ�ʱ��ʵʱλ��   
		statusLabel = new QLabel();
		statusLabel->setText(codec->toUnicode("��ǰλ��:"));
		statusLabel->setFixedWidth(100);

		/*---��ʾ���λ��
		mousePointLabel = new QLabel();
		mousePointLabel->setText("");
		mousePointLabel->setFixedWidth(100);*/

		//---��״̬�����ӿؼ�  
		/*statusBar()->addPermanentWidget(statusLabel);
		statusBar()->addPermanentWidget(mousePointLabel);*/

		//---���õ�ǰ��������׷�٣�Ĭ��Ϊfalse��false��ʾ��׷��  
		setMouseTracking(true);
	}

	
}

MutilObjetctRecognitionBasedOnQt::~MutilObjetctRecognitionBasedOnQt()
{

}

void MutilObjetctRecognitionBasedOnQt::showImage()
{

	m_matImg = myKinect.colorUpdate();
	cv::resize(m_matImg, m_matImg, m_sizeImg);
	cvtColor(m_matImg, m_matImg, CV_RGB2GRAY);
	cvtColor(m_matImg, m_matImg, CV_GRAY2RGB);
	
	m_matCurrentImg = m_matImg.clone();
	m_matImg=objectRecognition.run(matToHalcon.IplImageToHImage(m_matImg), m_matImg);
	//m_capture >> m_matImg;
	m_capQImg = Mat2QImage(m_matImg);
	ui.Cap->setPixmap(QPixmap::fromImage(m_capQImg));  // ��ͼƬ��ʾ��label��  
}

void MutilObjetctRecognitionBasedOnQt::on_openButton_clicked()
{
	/*m_capture.open(0);
	if (!m_capture.isOpened())
	{
		on_closeButton_clicked();

	}
	else
	{
	    ui.lineEdit->setText("0");
		m_vecStrExistence.clear();
		m_qTimer->start(33);
		m_bDrawing = false;
		m_bCap == true;

	}*/
	objectRecognition.initialize();
	HRESULT hr = myKinect.colorInitializeDefaultSensor();
	hr = myKinect.colorInitializeDefaultSensor();
	if (SUCCEEDED(hr))
	{
		m_matImg = myKinect.colorUpdate();
		cv::resize(m_matImg, m_matImg, m_sizeImg);
		cvtColor(m_matImg, m_matImg, CV_RGB2GRAY);
		cvtColor(m_matImg, m_matImg, CV_GRAY2RGB);
		m_vecStrExistence.clear();
		m_bDrawing = false;
		m_bCap = true;
		m_dInitialSimilariy = objectRecognition.minScore();
		QString m_qstr = QString("%1").arg(m_dInitialSimilariy);
		ui.lineEdit->setText(m_qstr);
		m_qTimer->start(33);
	}
	else
	{
		on_closeButton_clicked();
	}
}

void MutilObjetctRecognitionBasedOnQt::on_closeButton_clicked()
{
	m_bDrawing = false;
	m_qTimer->stop();
	m_matImg.release();
	ui.Cap->clear();
	ui.Image->clear();
	m_vecStrExistence.clear();
	m_capture.release();
	myKinect.clear();
	m_bCap = false;
	objectRecognition.clear();
}

void MutilObjetctRecognitionBasedOnQt::on_loadButton_clicked()
{

	QString filename = QFileDialog::getOpenFileName(this,
		tr("Open Image"), ".", tr("Image File (*.jpg *.png *.bmp)"));
	QTextCodec *code = QTextCodec::codecForName("gb18030");
	std::string name = code->fromUnicode(filename).data();//filename.toAscii().data()  
	m_matSrcImg = cv::imread(name);
	bool  m_bNameExistence = false;
	for (int i = 0; i < m_vecStrExistence.size(); i++)
	{
		if (m_vecStrExistence[i] == name)
		{
			m_bNameExistence = true;
			QMessageBox::information(this, QString::fromLocal8Bit("������ʾ"), QString::fromLocal8Bit("ģ���Ѵ��ڣ�"));
			return;
		}		
	}
	if (m_bNameExistence == false&&!m_matSrcImg.empty())
	{
		m_vecStrExistence.push_back(name);
		m_srcQImg = Mat2QImage(m_matSrcImg);
		ui.Image->setPixmap(QPixmap::fromImage(m_srcQImg));  // ��ͼƬ��ʾ��label�� 
		
		QMessageBox::information(this, QString::fromLocal8Bit("������ʾ"), QString::fromLocal8Bit("ģ����سɹ�"));
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("������ʾ"), QString::fromLocal8Bit("ģ�����ʧ��"));

	}
	
}


void MutilObjetctRecognitionBasedOnQt::on_removeButton_clicked()
{
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Open Image"), ".", tr("Image File (*.jpg *.png *.bmp)"));
	QTextCodec *code = QTextCodec::codecForName("gb18030");
	std::string name = code->fromUnicode(filename).data();//filename.toAscii().data()  
	m_matSrcImg = cv::imread(name);
	bool m_bNameExistence = false;
	vector<string>::iterator it;
	for (it = m_vecStrExistence.begin(); it != m_vecStrExistence.end();)
	{
		
		if (*it == name)
		{
			it = m_vecStrExistence.erase(it);
			m_bNameExistence = true;
		}
		else
		{
			++it;
		}
	}
	if (m_bNameExistence == true)
	{
		ui.Image->clear();
		//m_strRemove = name.substr(0,name.find("%"));
	
		QMessageBox::information(this, QString::fromLocal8Bit("������ʾ"), QString::fromLocal8Bit("ģ��ɾ���ɹ�"));
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("������ʾ"), QString::fromLocal8Bit("ģ��δɾ��"));

	}
	
}

void MutilObjetctRecognitionBasedOnQt::on_okButton_clicked()
{
	QString 	m_qStr;
	m_qStr = ui.lineEdit->text();
	double m_dSimilarity = m_qStr.toDouble();
	if (m_dSimilarity > 1||m_dSimilarity<0)
	{
		m_dSimilarity = 0.65;
	}
	
	QString m_qstr = QString("%1").arg(m_dSimilarity);
	ui.lineEdit->setText(m_qstr);
	objectRecognition.updateMinScore(m_dSimilarity);
}

void MutilObjetctRecognitionBasedOnQt::on_clearButton_clicked()
{
	if(m_bCap == true)
	{
		m_vecStrExistence.clear();
		objectRecognition.clear();
	}
	
}

void MutilObjetctRecognitionBasedOnQt::mousePressEvent(QMouseEvent *e)
{
	if (m_bCap == true)
	{
		//----Qt5�����������  
		//QTextCodec *codec = QTextCodec::codecForName("GB18030");
		m_qPoint = ui.Cap->mapFromGlobal(cursor().pos());
		//----QMouseEvent���ṩ��x()��y()�ɻ�ȡ�����Դ��ڵ�λ��  
		QString str = "(" + QString::number(m_qPoint.x()) + ", " + QString::number(m_qPoint.y()) + ")";

		//---������  
		if (Qt::LeftButton == e->button() && mousePosition())
		{
			statusBar()->showMessage(codec->toUnicode("���:") + str);
			m_pointOriginalPosition = Point(m_qPoint.x(), m_qPoint.y());
			//��ʼ����ʼ���ο�  
			m_recRectangle = Rect(m_qPoint.x() , m_qPoint.y(), 0, 0);
			m_bDrawing = true;
		}
		//---����Ҽ�  
		if (Qt::RightButton == e->button())
		{
			statusBar()->showMessage(codec->toUnicode("�Ҽ�:") + str);
		}
		//---����м�  
		if (Qt::MidButton == e->button())
		{
			statusBar()->showMessage(codec->toUnicode("�м�:") + str);
		}
	}
	
}

void MutilObjetctRecognitionBasedOnQt::mouseMoveEvent(QMouseEvent *e)
{
	if (m_bCap == true)
	{
		m_qPoint = ui.Cap->mapFromGlobal(cursor().pos());
		if (m_bDrawing&&mousePosition())
		{
			//��MIN�õ����ϵ���Ϊ���ο����ʼ���꣬������������������ʱֻ����һ���������  
			m_recRectangle.x = m_pointOriginalPosition.x;
			m_recRectangle.y = m_pointOriginalPosition.y;

			m_recRectangle.width = abs(m_recRectangle.x - m_qPoint.x());
			m_recRectangle.height = abs(m_recRectangle.y - m_qPoint.y());
			//��ֹ�������򳬳�ͼ��ķ�Χ  
			m_recRectangle &= Rect(0, 0, m_matImg.cols, m_matImg.rows);
			rectangle(m_matImg, m_recRectangle, Scalar(255, 0, 0), 1);  //�����ο�
			sprintf(m_charCurrrentPosition, "(%d,%d)", m_qPoint.x(), m_qPoint.y());
			putText(m_matImg, m_charCurrrentPosition, Point(m_qPoint.x(), m_qPoint.y()), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 255));//ֻ��ʵʱ��ʾ����ƶ������� 

			m_capQImg = Mat2QImage(m_matImg);
			ui.Cap->setPixmap(QPixmap::fromImage(m_capQImg));  // ��ͼƬ��ʾ��label��

		}
	}
	
}

void MutilObjetctRecognitionBasedOnQt::mouseReleaseEvent(QMouseEvent *e)
{
	m_qPoint = ui.Cap->mapFromGlobal(cursor().pos());
	if (m_bCap == true && mousePosition())
	{	
		//----QMouseEvent���ṩ��x()��y()�ɻ�ȡ�����Դ��ڵ�λ��  
		QString str = "(" + QString::number(m_qPoint.x()) + ", " + QString::number(m_qPoint.y()) + ")";
		statusBar()->showMessage(codec->toUnicode("���λ��:") + str, 3000);

		m_icurrentRoi_X = m_recRectangle.x;
		m_icurrentRoi_Y = m_recRectangle.y;
		m_icurrentRoi_Width = m_recRectangle.width;
		m_icurrentRoi_Height = m_recRectangle.height;
		if (m_icurrentRoi_Width > 0 && m_icurrentRoi_Height > 0 && m_icurrentRoi_Width*m_icurrentRoi_Height > 100)
		{
			rectangle(m_matCurrentImg, m_recRectangle, Scalar(255, 0, 0), 1);  //�����ο�
			m_srcQImg = Mat2QImage(m_matCurrentImg);
			ui.Image->setPixmap(QPixmap::fromImage(m_srcQImg));  // ��ͼƬ��ʾ��label�� 
		

			sprintf(m_charDstImg, m_strDstImg, m_iCurrentImgId);
			imwrite(m_charDstImg, m_matCurrentImg);
			
			objectRecognition.upDateNewData(m_charDstImg, m_icurrentRoi_X, m_icurrentRoi_Y, m_icurrentRoi_Width, m_icurrentRoi_Height, m_iCurrentImgId);
			m_iCurrentImgId++;
			QMessageBox::information(this, QString::fromLocal8Bit("������ʾ"), QString::fromLocal8Bit("ģ����ӳɹ�"));

		}
	
		m_bDrawing = false;
	}
	
}

void MutilObjetctRecognitionBasedOnQt::mouseDoubleClickEvent(QMouseEvent *e)
{
	/*if (m_bCap == true)
	{

	}*/
}

QImage  MutilObjetctRecognitionBasedOnQt::Mat2QImage(cv::Mat cvImg)
{
	QImage qImg;
	if (cvImg.channels() == 3)                             //3 channels color image
	{

		cv::cvtColor(cvImg, cvImg, CV_BGR2RGB);
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_RGB888);
	}
	else if (cvImg.channels() == 1)                    //grayscale image
	{
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_Indexed8);
	}
	else
	{
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_RGB888);
	}

	return qImg;

}

bool MutilObjetctRecognitionBasedOnQt::mousePosition()
{
	if (m_qPoint.x() > 0 && m_qPoint.y() > 0 && m_qPoint.x()< m_iPoint_X&&m_qPoint.y() < m_iPoint_Y)
	{
		return true;
	}
	else
	{
		return false;
	}
}
