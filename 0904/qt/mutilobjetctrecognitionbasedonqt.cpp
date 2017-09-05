#include "mutilobjetctrecognitionbasedonqt.h"

MutilObjetctRecognitionBasedOnQt::MutilObjetctRecognitionBasedOnQt(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);  
	m_qTimer = new QTimer(this);
	connect(m_qTimer, SIGNAL(timeout()), this, SLOT(showImage()));  // 时间到，读取当前摄像头信息

	if (m_bCap == true)
	{
		//----Qt5解决中文乱码  
		QTextCodec *codec = QTextCodec::codecForName("GB18030");

		//---显示鼠标移动时的实时位置   
		statusLabel = new QLabel();
		statusLabel->setText(codec->toUnicode("当前位置:"));
		statusLabel->setFixedWidth(100);

		/*---显示鼠标位置
		mousePointLabel = new QLabel();
		mousePointLabel->setText("");
		mousePointLabel->setFixedWidth(100);*/

		//---在状态栏增加控件  
		/*statusBar()->addPermanentWidget(statusLabel);
		statusBar()->addPermanentWidget(mousePointLabel);*/

		//---设置当前窗体对鼠标追踪，默认为false，false表示不追踪  
		setMouseTracking(true);
	}
	
}

MutilObjetctRecognitionBasedOnQt::~MutilObjetctRecognitionBasedOnQt()
{

}

void MutilObjetctRecognitionBasedOnQt::showImage()
{

	m_matImg = myKinect.colorUpdate();
	m_matCurrentImg = m_matImg.clone();
	m_matImg=objectRecognition.run(matToHalcon.IplImageToHImage(m_matImg), m_matImg);
	//m_capture >> m_matImg;
	m_capQImg = Mat2QImage(m_matImg);
	ui.Cap->setPixmap(QPixmap::fromImage(m_capQImg));  // 将图片显示到label上  
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
		m_vecStrExistence.clear();
		m_bDrawing = false;
		m_bCap == true;
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
	m_bCap == false;
}

void MutilObjetctRecognitionBasedOnQt::on_loadButton_clicked()
{

	QString filename = QFileDialog::getOpenFileName(this,
		tr("Open Image"), ".", tr("Image File (*.jpg *.png *.bmp)"));
	QTextCodec *code = QTextCodec::codecForName("gb18030");
	std::string name = code->fromUnicode(filename).data();//filename.toAscii().data()  
	m_matSrcImg = cv::imread(name);
	int m_bNameExistence = false;
	for (int i = 0; i < m_vecStrExistence.size(); i++)
	{
		if (m_vecStrExistence[i] == name)
		{
			m_bNameExistence = true;
			return;
		}		
	}
	if (m_bNameExistence == false)
	{
		m_vecStrExistence.push_back(name);
		m_srcQImg = Mat2QImage(m_matSrcImg);
		ui.Image->setPixmap(QPixmap::fromImage(m_srcQImg));  // 将图片显示到label上 
	}
	
}


void MutilObjetctRecognitionBasedOnQt::on_removeButton_clicked()
{

}

void MutilObjetctRecognitionBasedOnQt::mousePressEvent(QMouseEvent *e)
{
	//----Qt5解决中文乱码  
	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	m_qPoint = ui.Cap->mapFromGlobal(cursor().pos());
	//----QMouseEvent类提供的x()和y()可获取鼠标相对窗口的位置  
	QString str = "(" + QString::number(m_qPoint.x()) + ", " + QString::number(m_qPoint.y()) + ")";

	//---点击左键  
	if (Qt::LeftButton == e->button())
	{
		statusBar()->showMessage(codec->toUnicode("左键:") + str);
		m_pointOriginalPosition = Point(m_qPoint.x() + m_iAdd_X, m_qPoint.y() + m_iAdd_Y);
		//初始化起始矩形框  
		m_recRectangle = Rect(m_qPoint.x() + m_iAdd_X, m_qPoint.y() + m_iAdd_Y, 0, 0);
		m_bDrawing = true;
	}
	//---点击右键  
	if (Qt::RightButton == e->button())
	{
		statusBar()->showMessage(codec->toUnicode("右键:") + str);
	}
	//---点击中键  
	if (Qt::MidButton == e->button())
	{
		statusBar()->showMessage(codec->toUnicode("中键:") + str);
	}
}

void MutilObjetctRecognitionBasedOnQt::mouseMoveEvent(QMouseEvent *e)
{
	m_qPoint = ui.Cap->mapFromGlobal(cursor().pos());
	//mousePointLabel->setText("(" + QString::number(e->x()) + ", " + QString::number(e->y()) + ")");
	if (m_bDrawing)
	{
		//用MIN得到左上点作为矩形框的起始坐标，如果不加这个，画矩形时只能向一个方向进行  
		m_recRectangle.x = m_pointOriginalPosition.x;
		m_recRectangle.y = m_pointOriginalPosition.y;

		m_recRectangle.width = abs(m_recRectangle.x - m_qPoint.x() - m_iAdd_X);
		m_recRectangle.height = abs(m_recRectangle.y - m_qPoint.y() - m_iAdd_Y);
		//防止矩形区域超出图像的范围  
		m_recRectangle &= Rect(0, 0, m_matImg.cols, m_matImg.rows);
		rectangle(m_matImg, m_recRectangle, Scalar(255, 0, 0), 1);  //画矩形框
		sprintf(m_charCurrrentPosition, "(%d,%d)", m_qPoint.x(), m_qPoint.y());
		putText(m_matImg, m_charCurrrentPosition, Point(m_qPoint.x(), m_qPoint.y()), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 255));//只是实时显示鼠标移动的坐标 
		
		m_capQImg = Mat2QImage(m_matImg);
		ui.Cap->setPixmap(QPixmap::fromImage(m_capQImg));  // 将图片显示到label上
		//sprintf(m_charCurrrentPosition, "(%d,%d)", x, y);
		//putText(m_matImg, m_charCurrrentPosition, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 255));//只是实时显示鼠标移动的坐标 
		//imshow(windowsName, m_matImg);

	}
}

void MutilObjetctRecognitionBasedOnQt::mouseReleaseEvent(QMouseEvent *e)
{
	//----Qt5解决中文乱码  
	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	
	m_qPoint = ui.Cap->mapFromGlobal(cursor().pos());
	//----QMouseEvent类提供的x()和y()可获取鼠标相对窗口的位置  
	QString str = "(" + QString::number(m_qPoint.x()) + ", " + QString::number(m_qPoint.y()) + ")";
	statusBar()->showMessage(codec->toUnicode("鼠标位置:") + str, 3000);


	//roi.release();
	
	m_icurrentRoi_X = m_recRectangle.x;
	m_icurrentRoi_Y = m_recRectangle.y;
	m_icurrentRoi_Width = m_recRectangle.width;
	m_icurrentRoi_Height = m_recRectangle.height;
	if (m_icurrentRoi_Width > 0 && m_icurrentRoi_Height > 0 && m_icurrentRoi_Width*m_icurrentRoi_Height > 100)
	{
		rectangle(m_matCurrentImg, m_recRectangle, Scalar(255, 0, 0), 1);  //画矩形框
		//roi = m_matCurrentImg(Rect(m_icurrentRoi_X, m_icurrentRoi_Y, m_icurrentRoi_Width, m_icurrentRoi_Height));
		m_srcQImg = Mat2QImage(m_matCurrentImg);
		ui.Image->setPixmap(QPixmap::fromImage(m_srcQImg));  // 将图片显示到label上 
		/*namedWindow("SignROI", WINDOW_AUTOSIZE);
		imshow("SignROI", roi);
		waitKey(1);	  */

		sprintf(m_charDstImg, m_strDstImg, m_iCurrentImgNum);
		imwrite(m_charDstImg, m_matCurrentImg);
		objectRecognition.upDateNewData(m_charDstImg, m_icurrentRoi_X, m_icurrentRoi_Y, m_icurrentRoi_Width, m_icurrentRoi_Height);
		m_iCurrentImgNum++;

	}
	//if (m_matCurrentImg.empty())
	//{
	//	ui.Image->setPixmap(QPixmap::fromImage(m_srcQImg));  // 将图片显示到label上 
	//}
	/*else
	{
		roi.release();
	}*/
	m_bDrawing = false;
}

void MutilObjetctRecognitionBasedOnQt::mouseDoubleClickEvent(QMouseEvent *e)
{

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