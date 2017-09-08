#include "mutilobjetctrecognitionbasedonqt.h"

MutilObjetctRecognitionBasedOnQt::MutilObjetctRecognitionBasedOnQt(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this); 
	ui.minScorelineEdit->setText("0");
	ui.maxOverLaplineEdit->setText("0");
	initialize();
	objectRecognition.initialize();

	m_qTimer = new QTimer(this);
	m_qCurrentTimer = new QTimer(this);
	m_qCurrentTimer->start(33);
	
	connect(m_qTimer, SIGNAL(timeout()), this, SLOT(runDemo()));  // 时间到，读取当前摄像头信息
	connect(m_qTimer, SIGNAL(timeout()), this, SLOT(minAreaAndMaxArea()));
	connect(m_qCurrentTimer, SIGNAL(timeout()), this, SLOT(updateData()));
	//QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("初始化完毕"));
}

MutilObjetctRecognitionBasedOnQt::~MutilObjetctRecognitionBasedOnQt()
{

}

void MutilObjetctRecognitionBasedOnQt::runDemo()
{
	m_dCurrentFps = clock();
	m_capture >> m_matImg;
	cvtColor(m_matImg, m_matImg, CV_RGB2GRAY);
	cvtColor(m_matImg, m_matImg, CV_GRAY2RGB);
	cv::resize(m_matImg, m_matImg, m_Size);
	GaussianBlur(m_matImg, m_matImg, Size(3, 3), 0);
	m_matImg=imageEnhancement(m_matImg);
	m_matImg = imageTransform(m_matImg);
	m_matImg = objectRecognition.run(matToHalcon.IplImageToHImage(m_matImg), m_matImg);
	m_capQImg = Mat2QImage(m_matImg);
	ui.Cap->setPixmap(QPixmap::fromImage(m_capQImg));  // 将图片显示到label上  
	m_dCurrentFps = clock() - m_dCurrentFps;
}

void MutilObjetctRecognitionBasedOnQt::on_openButton_clicked()
{

	 if (m_bCap == true)
	 {
		 return;
	 }
	 else if (m_bCap == false)
	{
		
		m_capture.open(0);
		if (!m_capture.isOpened())
		{
			on_closeButton_clicked();
		}
		else
		{
			
			m_bCap = true;
			m_capture >> m_matImg;
			
			cvtColor(m_matImg, m_matImg, CV_RGB2GRAY);
			cvtColor(m_matImg, m_matImg, CV_GRAY2RGB);
			cv::resize(m_matImg, m_matImg, m_Size);
			m_iPoint_X = m_matImg.cols;
			m_iPoint_Y = m_matImg.rows;
			m_matImg = imageEnhancement(m_matImg);
			m_matImg = imageTransform(m_matImg);
			GaussianBlur(m_matImg, m_matImg, Size(3, 3), 0);
			m_capQImg = Mat2QImage(m_matImg);
			ui.Cap->setPixmap(QPixmap::fromImage(m_capQImg));  // 将图片显示到label上 

			m_qTimer->start(100);
			m_bDrawing = false;
			m_bCap = true;
			/*initialize();*/
			
		}
		m_bCap = true;
	}
	
	
}

void MutilObjetctRecognitionBasedOnQt::on_closeButton_clicked()
{
	if (m_bCap == true)
	{
		m_bDrawing = false;
		m_qTimer->stop();
		m_matImg.release();
		ui.Cap->clear();
		ui.Image->clear();
		m_vecStrExistence.clear();
		m_capture.release();
		m_bCap = false;
		objectRecognition.clear();
		m_dCurrentFps = 0;
		ui.minScorelineEdit->setText("0");
		ui.maxOverLaplineEdit->setText("0");
	}
	
}


void MutilObjetctRecognitionBasedOnQt::on_clearButton_clicked()
{
	if (m_bCap == true)
	{
		ui.Image->clear();
		m_vecStrExistence.clear();
		objectRecognition.clear();
		QString m_qstr = QString("%1").arg(m_dInitialMinScore);
		ui.minScorelineEdit->setText(m_qstr);
		m_qstr = QString("%1").arg(m_dInitialMaxLap);
		ui.maxOverLaplineEdit->setText(m_qstr);
		m_qstr.clear();
	}

}

void MutilObjetctRecognitionBasedOnQt::on_screenShootButton_clicked()
{
	if (m_bCap == true)
	{
		++m_iCurrentImgId;
		m_matImg.copyTo(m_matCurrentImg);
		m_currentTime = time(0);
		strftime(m_charCurrentTime, sizeof(m_charCurrentTime), "%Y-%m-%d %X %A ", localtime(&m_currentTime));
		putText(m_matCurrentImg, m_charCurrentTime, Point(10, 20), 1, 0.6, Scalar(0, 0, 255), 1, 8);

		m_tmpQImg = Mat2QImage(m_matCurrentImg);
		ui.Image->setPixmap(QPixmap::fromImage(m_tmpQImg));  // 将图片显示到label上 

		sprintf(m_charDstImgName, m_strDstImg, m_iCurrentImgId);
		imwrite(m_charDstImgName, m_matCurrentImg);
	}
}

void MutilObjetctRecognitionBasedOnQt::on_loadModelButton_clicked()
{
	QString m_qstrFilename = QFileDialog::getOpenFileName(this,
		tr("Open Model"), ".", tr("Image File (*.shm)"));
	QTextCodec *code = QTextCodec::codecForName("gb18030");
	std::string m_strloadModelName = code->fromUnicode(m_qstrFilename).data();//filename.toAscii().data()
	if (m_strloadModelName.size() > 0)
	{
		if (objectRecognition.loadModel(m_strloadModelName))
		{
			QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板导入成功"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板已存在"));
		}
	}
	else
	{
		return;
	}
	m_qstrFilename.clear();
}
void MutilObjetctRecognitionBasedOnQt::on_removeModelButton_clicked()
{
	QString m_qstrFilename = QFileDialog::getOpenFileName(this,
		tr("Open Image"), ".", tr("Image File (*.shm)"));
	QTextCodec *code = QTextCodec::codecForName("gb18030");
	std::string m_strRemoveModelName = code->fromUnicode(m_qstrFilename).data();//filename.toAscii().data()  
	if (m_strRemoveModelName.length() <= 0)
	{
		return;
	}
	else
	{
		if (objectRecognition.removdeModel(m_strRemoveModelName))
		{
			QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板已删除"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板未删除或不存在"));
		}
	}
	
}

void MutilObjetctRecognitionBasedOnQt::on_loadButton_clicked()
{

	QString m_qstrFilename = QFileDialog::getOpenFileName(this,
		tr("Open Image"), ".", tr("Image File (*.jpg *.png *.bmp)"));
	QTextCodec *code = QTextCodec::codecForName("gb18030");
	std::string m_strImgName = code->fromUnicode(m_qstrFilename).data();//filename.toAscii().data()  
	if (m_strImgName.length() <= 0)
	{
		return;
	}
	m_matSrcImg = cv::imread(m_strImgName);
	bool  m_bNameExistence = false;
	for (int i = 0; i < m_vecStrExistence.size(); i++)
	{
		if (m_vecStrExistence[i] == m_strImgName)
		{
			m_bNameExistence = true;
			QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("图片已存在！"));
			return;
		}
	}
	if (m_bNameExistence == false && !m_matSrcImg.empty())
	{
		m_vecStrExistence.clear();
		m_vecStrExistence.push_back(m_strImgName);
		cv::resize(m_matSrcImg, m_matSrcImg, m_Size);
		m_srcQImg = Mat2QImage(m_matSrcImg);
		ui.Image->setPixmap(QPixmap::fromImage(m_srcQImg));  // 将图片显示到label上 

		QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板加载成功"));
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板加载失败"));

	}

}




void MutilObjetctRecognitionBasedOnQt::mousePressEvent(QMouseEvent *e)
{
	if (m_bCap == true )
	{
		m_qPoint = ui.Image->mapFromGlobal(cursor().pos());
		//----QMouseEvent类提供的x()和y()可获取鼠标相对窗口的位置  
		QString str = "(" + QString::number(m_qPoint.x()) + ", " + QString::number(m_qPoint.y()) + ")";

		//---点击左键  
		if (Qt::LeftButton == e->button() && mousePosition())
		{
			//statusBar()->showMessage(codec->toUnicode("左键:") + str);
			m_pointOriginalPosition = Point(m_qPoint.x(), m_qPoint.y());
			//初始化起始矩形框  
			m_recRectangle = Rect(m_qPoint.x() , m_qPoint.y(), 0, 0);
			m_bDrawing = true;
		}
		//---点击右键  
		if (Qt::RightButton == e->button())
		{
			//statusBar()->showMessage(codec->toUnicode("右键:") + str);
		}
		//---点击中键  
		if (Qt::MidButton == e->button())
		{
			//statusBar()->showMessage(codec->toUnicode("中键:") + str);
		}
		str.clear();
	}
	
}

void MutilObjetctRecognitionBasedOnQt::mouseMoveEvent(QMouseEvent *e)
{
	m_matTmpImg.release();
	m_matCurrentImg.copyTo(m_matTmpImg);
	if (m_bCap == true)
	{
		m_qPoint = ui.Image->mapFromGlobal(cursor().pos());
		if (m_bDrawing&&mousePosition())
		{
			//----QMouseEvent类提供的x()和y()可获取鼠标相对窗口的位置  
			QString m_qstr = "(" + QString::number(m_qPoint.x()) + ", " + QString::number(m_qPoint.y()) + ")";
			statusBar()->showMessage(codec->toUnicode("鼠标位置:") + m_qstr, 3000);
			//用MIN得到左上点作为矩形框的起始坐标，如果不加这个，画矩形时只能向一个方向进行  
			m_recRectangle.x = m_pointOriginalPosition.x;
			m_recRectangle.y = m_pointOriginalPosition.y;

			m_recRectangle.width = abs(m_recRectangle.x - m_qPoint.x());
			m_recRectangle.height = abs(m_recRectangle.y - m_qPoint.y());
			//防止矩形区域超出图像的范围  
			m_recRectangle &= Rect(0, 0, m_matTmpImg.cols, m_matTmpImg.rows);
			rectangle(m_matTmpImg, m_recRectangle, m_scalarColor, 1);  //画矩形框
			sprintf(m_charCurrrentPosition, "(%d,%d)", m_qPoint.x(), m_qPoint.y());
			putText(m_matImg, m_charCurrrentPosition, Point(m_qPoint.x(), m_qPoint.y()), FONT_HERSHEY_SIMPLEX, 0.5, m_scalarColor);//只是实时显示鼠标移动的坐标 

			m_capQImg = Mat2QImage(m_matTmpImg);
			ui.Image->setPixmap(QPixmap::fromImage(m_capQImg));  // 将图片显示到label上
			m_qstr.clear();
		}
	}
	
}

void MutilObjetctRecognitionBasedOnQt::mouseReleaseEvent(QMouseEvent *e)
{
	m_matTmpImg.release();
	m_matCurrentImg.copyTo(m_matTmpImg);
	m_qPoint = ui.Image->mapFromGlobal(cursor().pos());
	if (m_bCap == true&& mousePosition())
	{	
		m_icurrentRoi_X = m_recRectangle.x;
		m_icurrentRoi_Y = m_recRectangle.y;
		m_icurrentRoi_Width = m_recRectangle.width;
		m_icurrentRoi_Height = m_recRectangle.height;
		double m_AreaSum = m_icurrentRoi_Width*m_icurrentRoi_Height;
		if (m_icurrentRoi_Width > 0 && m_icurrentRoi_Height > 0 && m_AreaSum > 10 &&m_AreaSum<50000 )
		{
			sprintf(m_charDstImgName, m_strDstImg, m_iCurrentImgId);
			objectRecognition.upDateNewData(m_charDstImgName, m_icurrentRoi_X, m_icurrentRoi_Y, m_icurrentRoi_Width, m_icurrentRoi_Height, m_iCurrentImgId);
			
			//QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板添加成功"));
			QMessageBox message(QMessageBox::NoIcon, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板添加成功"), QMessageBox::Yes /*| QMessageBox::No,*/, NULL);
			rectangle(m_matTmpImg, m_recRectangle, m_scalarColor, 1);  //画矩形框
			m_srcQImg = Mat2QImage(m_matTmpImg);
			ui.Image->setPixmap(QPixmap::fromImage(m_srcQImg));  // 将图片显示到label上 

			
			if (message.exec() == QMessageBox::Yes)
			{
				ui.Image->setPixmap(QPixmap::fromImage(m_tmpQImg));  // 将图片显示到label上 
			}
			m_fileStorage.open(m_strFileStorage, FileStorage::WRITE);
			//m_fileStorage<<"m_iCurrentImgId" << m_iCurrentImgId;
			m_fileStorage.release();
		}
		else
		{
			ui.Image->setPixmap(QPixmap::fromImage(m_tmpQImg));  // 将图片显示到label上 
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
	if (m_qPoint.x() < 0 || m_qPoint.y() < 0 || m_qPoint.x()> m_iPoint_X || m_qPoint.y() > m_iPoint_Y)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void MutilObjetctRecognitionBasedOnQt::updateData()
{
	m_currentTime = time(0);
	strftime(m_charCurrentTime, sizeof(m_charCurrentTime), "%Y-%m-%d %X %A ", localtime(&m_currentTime));
	QString m_qstr = QString(m_charCurrentTime);
	ui.timeLabel->setText(m_qstr);
	m_qstr.clear();
	m_qstr = QString("%1").arg(m_dCurrentFps);
	ui.currentFpsLabel->setText(m_qstr);
	m_qstr.clear();
	m_vecModelName.clear();
	ui.modelComboBox->clear();
	m_vecModelName = objectRecognition.updateModel();
	for (int i = 0; i < m_vecModelName.size(); i++)
	{
		m_qstr = QString::fromStdString(m_vecModelName[i]);
		ui.modelComboBox->addItem(m_qstr);
		m_qstr.clear();
	}
}

void MutilObjetctRecognitionBasedOnQt::initialize()
{
	m_fileStorage.open(m_strFileStorage, FileStorage::READ);
	if (!m_fileStorage.isOpened())
	{
		m_iCurrentImgId = 0;
	}
	else
	{
		m_fileStorage["m_iCurrentImgId"] >> m_iCurrentImgId;
		m_fileStorage.release();
	}
	QString m_qstr;
	m_dInitialMinScore = objectRecognition.minScore();

	if (m_dInitialMinScore > 1 || m_dInitialMinScore < 0.001)
	{
		m_dInitialMinScore = 0.95;
	}
	m_qstr = QString("%1").arg(m_dInitialMinScore);
	ui.minScorelineEdit->setText(m_qstr);
	m_qstr.clear();

	m_dInitialMaxLap = objectRecognition.maxOverLap();
	if (m_dInitialMaxLap > 1 || m_dInitialMaxLap < 0.001)
	{
		m_dInitialMaxLap = 0.15;
	}
	m_qstr = QString("%1").arg(m_dInitialMaxLap);
	ui.maxOverLaplineEdit->setText(m_qstr);
	m_qstr.clear();
	m_vecStrExistence.clear();
	

	ui.imageEnhanceComboBox->addItem("Orignial");
	ui.imageEnhanceComboBox->addItem("Histogram");
	ui.imageEnhanceComboBox->addItem("Laplace");
	ui.imageEnhanceComboBox->addItem("Log");
	ui.imageEnhanceComboBox->addItem("Gamma");
	ui.imageEnhanceComboBox->addItem("Auto_White_Balance");

	ui.transformComboBox->addItem("Orignial");
	ui.transformComboBox->addItem("RightToLeft");
	ui.transformComboBox->addItem("AboveToBelow");
	ui.transformComboBox->addItem("Other");
	
}

void MutilObjetctRecognitionBasedOnQt::on_minScoreOkButton_clicked()
{
	QString 	m_qStr;
	m_qStr = ui.minScorelineEdit->text();
	double m_dMinScore = m_qStr.toDouble();
	if (m_dMinScore > 1 || m_dMinScore < 0.001)
	{
		m_dMinScore = 0.95;
	}

	QString m_qstr = QString("%1").arg(m_dMinScore);
	ui.minScorelineEdit->setText(m_qstr);
	objectRecognition.updateMinScore(m_dMinScore);
}

void MutilObjetctRecognitionBasedOnQt::on_maxOverLapOkButton_clicked()
{
	QString 	m_qstr;
	m_qstr = ui.maxOverLaplineEdit->text();
	double m_dMaxOverLap = m_qstr.toDouble();
	if (m_dMaxOverLap > 1 || m_dMaxOverLap < 0.001)
	{
		m_dMaxOverLap = 0.15;
	}

	m_qstr = QString("%1").arg(m_dMaxOverLap);
	ui.maxOverLaplineEdit->setText(m_qstr);
	objectRecognition.updateMaxOverLap(m_dMaxOverLap);
	m_qstr.clear();
}
cv::Mat MutilObjetctRecognitionBasedOnQt::imageEnhancement(Mat m_mat)
{
	QString m_qstr = ui.imageEnhanceComboBox->currentText();
	String m_strCurrent = m_qstr.toStdString();
	if (m_strCurrent == "Orignial")
	{
		m_strCurrent.clear();
		return m_mat;
	}
	else if (m_strCurrent == "Histogram")
	{
		Mat m_matHistogram[3];
		split(m_mat, m_matHistogram);
		for (int i = 0; i < 3; i++)
		{
			equalizeHist(m_matHistogram[i], m_matHistogram[i]);
		}
		merge(m_matHistogram, 3, m_mat);
		m_strCurrent.clear();
		return m_mat;
	}
	else if (m_strCurrent=="Laplace")
	{
		Mat m_matLaplace;
		Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);
		filter2D(m_mat, m_matLaplace, CV_8UC3, kernel);
		m_strCurrent.clear();
		return m_matLaplace;
	}
	else if (m_strCurrent == "Log")
	{
		Mat m_matLog(m_mat.size(), CV_32FC3);
		for (int i = 0; i < m_mat.rows; i++)
		{
			for (int j = 0; j < m_mat.cols; j++)
			{
				m_matLog.at<Vec3f>(i, j)[0] = log(1 + m_mat.at<Vec3b>(i, j)[0]);
				m_matLog.at<Vec3f>(i, j)[1] = log(1 + m_mat.at<Vec3b>(i, j)[1]);
				m_matLog.at<Vec3f>(i, j)[2] = log(1 + m_mat.at<Vec3b>(i, j)[2]);
			}
		}
		//归一化到0~255    
		normalize(m_matLog, m_matLog, 0, 255, CV_MINMAX);
		//转换成8bit图像显示    
		convertScaleAbs(m_matLog, m_matLog);
		m_strCurrent.clear();
		return m_matLog;

	}
	else if (m_strCurrent == "Gamma")
	{
		Mat m_matGamma(m_mat.size(), CV_32FC3);
		for (int i = 0; i < m_mat.rows; i++)
		{
			for (int j = 0; j < m_mat.cols; j++)
			{
				m_matGamma.at<Vec3f>(i, j)[0] = (m_mat.at<Vec3b>(i, j)[0])*(m_mat.at<Vec3b>(i, j)[0])*(m_mat.at<Vec3b>(i, j)[0]);
				m_matGamma.at<Vec3f>(i, j)[1] = (m_mat.at<Vec3b>(i, j)[1])*(m_mat.at<Vec3b>(i, j)[1])*(m_mat.at<Vec3b>(i, j)[1]);
				m_matGamma.at<Vec3f>(i, j)[2] = (m_mat.at<Vec3b>(i, j)[2])*(m_mat.at<Vec3b>(i, j)[2])*(m_mat.at<Vec3b>(i, j)[2]);
			}
		}
		//归一化到0~255    
		normalize(m_matGamma, m_matGamma, 0, 255, CV_MINMAX);
		//转换成8bit图像显示    
		convertScaleAbs(m_matGamma, m_matGamma);
		m_strCurrent.clear();
		return m_matGamma;

	}
	else if (m_strCurrent == "Auto_White_Balance")
	{
		imageEnhancing imageEnhancing;
		m_mat = imageEnhancing.auto_White_Balance(m_mat, 1);
		m_strCurrent.clear();
		return m_mat;
	}
	else
	{
		m_strCurrent.clear();
		return m_mat;
	}
}

cv::Mat MutilObjetctRecognitionBasedOnQt::imageTransform(Mat m_mat)
{
	QString m_qstr = ui.transformComboBox->currentText();
	String m_strCurrent = m_qstr.toStdString();
	if(m_strCurrent == "Orignial")
	{
		m_qstr.clear();
		return m_mat;
	}
	else if (m_strCurrent == "RightToLeft")
	{
		remapImage remapImage;
		m_mat = remapImage.imageRemap(m_mat);
	}
	else  if (m_strCurrent == "AboveToBelow")
	{
		flip(m_mat, m_mat, -1);
	}
	else if (m_strCurrent == "Other")
	{
		remapImage remapImage;
		m_mat = remapImage.imageRemap(m_mat);
		flip(m_mat, m_mat, -1);
	}
	m_qstr.clear();
	return m_mat;
	
}


