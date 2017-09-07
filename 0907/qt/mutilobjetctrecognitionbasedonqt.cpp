#include "mutilobjetctrecognitionbasedonqt.h"

MutilObjetctRecognitionBasedOnQt::MutilObjetctRecognitionBasedOnQt(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this); 


	m_qTimer = new QTimer(this);
	m_qCurrentTimer = new QTimer(this);
	m_qCurrentTimer->start(33);
	connect(m_qTimer, SIGNAL(timeout()), this, SLOT(runDemo()));  // 时间到，读取当前摄像头信息
	connect(m_qCurrentTimer, SIGNAL(timeout()), this, SLOT(updateTime()));
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
	flip(m_matImg, m_matImg, -1);
	m_matImg = ImageEnhancement.SimplestCB(m_matImg, 1);
	m_matImg = objectRecognition.run(matToHalcon.IplImageToHImage(m_matImg), m_matImg);
	
	//m_capture >> m_matImg;
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
			objectRecognition.initialize();
			m_bCap = true;
			m_capture >> m_matImg;
			m_iPoint_X = m_matImg.cols;
			m_iPoint_Y = m_matImg.rows;
			cvtColor(m_matImg, m_matImg, CV_RGB2GRAY);
			cvtColor(m_matImg, m_matImg, CV_GRAY2RGB);
			cv::resize(m_matImg, m_matImg, m_Size);
			flip(m_matImg, m_matImg, -1);
			GaussianBlur(m_matImg, m_matImg, Size(3, 3), 0);
			m_matImg = ImageEnhancement.SimplestCB(m_matImg, 1);
			m_capQImg = Mat2QImage(m_matImg);
			ui.Cap->setPixmap(QPixmap::fromImage(m_capQImg));  // 将图片显示到label上 
			initialize();
			
		}
		m_bCap = true;
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
	m_bCap = false;
	objectRecognition.clear();
}

void MutilObjetctRecognitionBasedOnQt::on_loadButton_clicked()
{

	QString m_qstrFilename = QFileDialog::getOpenFileName(this,
		tr("Open Image"), ".", tr("Image File (*.jpg *.png *.bmp)"));
	QTextCodec *code = QTextCodec::codecForName("gb18030");
	std::string m_strImgName = code->fromUnicode(m_qstrFilename).data();//filename.toAscii().data()  
	m_matSrcImg = cv::imread(m_strImgName);
	bool  m_bNameExistence = false;
	for (int i = 0; i < m_vecStrExistence.size(); i++)
	{
		if (m_vecStrExistence[i] == m_strImgName)
		{
			m_bNameExistence = true;
			QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板已存在！"));
			return;
		}		
	}
	if (m_bNameExistence == false&&!m_matSrcImg.empty())
	{
		m_vecStrExistence.push_back(m_strImgName);
		m_srcQImg = Mat2QImage(m_matSrcImg);
		ui.Image->setPixmap(QPixmap::fromImage(m_srcQImg));  // 将图片显示到label上 
		
		QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板加载成功"));
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板加载失败"));

	}
	
}


void MutilObjetctRecognitionBasedOnQt::on_removeButton_clicked()
{
	QString m_qstrFilename = QFileDialog::getOpenFileName(this,
		tr("Open Image"), ".", tr("Image File (*.jpg *.png *.bmp)"));
	QTextCodec *code = QTextCodec::codecForName("gb18030");
	std::string m_strImgName = code->fromUnicode(m_qstrFilename).data();//filename.toAscii().data()  
	m_matSrcImg = cv::imread(m_strImgName);
	bool m_bNameExistence = false;
	vector<string>::iterator it;
	for (it = m_vecStrExistence.begin(); it != m_vecStrExistence.end();)
	{
		
		if (*it == m_strImgName)
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
		m_strRemove = m_strImgName.substr(0, m_strImgName.find("."));
		m_strRemove = m_strRemove + ".shm";

	
		QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板删除成功"));
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板未删除"));

	}
	
}

void MutilObjetctRecognitionBasedOnQt::on_minScoreOkButton_clicked()
{
	QString 	m_qStr;
	m_qStr = ui.minScorelineEdit->text();
	double m_dMinScore = m_qStr.toDouble();
	if (m_dMinScore > 1||m_dMinScore<0)
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
	if (m_dMaxOverLap > 1 || m_dMaxOverLap < 0)
	{
		m_dMaxOverLap = 0.15;
	}

	m_qstr = QString("%1").arg(m_dMaxOverLap);
	ui.maxOverLaplineEdit->setText(m_qstr);
	objectRecognition.updateMaxOverLap(m_dMaxOverLap);
	m_qstr.clear();
}

void MutilObjetctRecognitionBasedOnQt::on_clearButton_clicked()
{
	if(m_bCap == true)
	{
		ui.Image->clear();
		m_vecStrExistence.clear();
		objectRecognition.clear();
		QString m_qstr = QString("%1").arg(m_dInitialMinScore);
		ui.minScorelineEdit->setText(m_qstr);
		m_qstr = QString("%1").arg(m_dInitialMinScore);
		ui.maxOverLaplineEdit->setText(m_qstr);
		m_qstr.clear();
	}
	
}

void MutilObjetctRecognitionBasedOnQt::on_loadModelButton_clicked()
{
	QString m_qstrFilename = QFileDialog::getOpenFileName(this,
		tr("Open Model"), ".", tr("Image File (*.shm)"));
	QTextCodec *code = QTextCodec::codecForName("gb18030");
	std::string m_loadModelName = code->fromUnicode(m_qstrFilename).data();//filename.toAscii().data()
	if (m_loadModelName.size() > 0)
	{
		if (objectRecognition.loadModel(m_loadModelName))
		{
			QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板导入成功"));
		}
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板未导入成功"));
	}
	m_qstrFilename.clear();
}

void MutilObjetctRecognitionBasedOnQt::updateTime()
{
	m_currentTime = time(0);
	strftime(m_charCurrentTime, sizeof(m_charCurrentTime), "%Y/%m/%d %X %A ", localtime(&m_currentTime));
	QString m_qstr = QString(m_charCurrentTime);
	ui.timeLabel->setText(m_qstr);
	m_qstr.clear();
	m_qstr = QString("%1").arg(m_dCurrentFps);
	ui.currentFpsLabel->setText(m_qstr);
	m_qstr.clear();
}

void MutilObjetctRecognitionBasedOnQt::on_screenShootButton_clicked()
{
	++m_iCurrentImgId;
	m_matImg.copyTo(m_matCurrentImg);
	m_tmpQImg = Mat2QImage(m_matCurrentImg);
	ui.Image->setPixmap(QPixmap::fromImage(m_tmpQImg));  // 将图片显示到label上 
	sprintf(m_charDstImgName, m_strDstImg, m_iCurrentImgId);
	imwrite(m_charDstImgName, m_matCurrentImg);
	
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
			m_fileStorage<<"m_iCurrentImgId" << m_iCurrentImgId;
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
	if (m_qPoint.x() > 0 && m_qPoint.y() > 0 && m_qPoint.x()< m_iPoint_X&&m_qPoint.y() < m_iPoint_Y)
	{
		return true;
	}
	else
	{
		return false;
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
	m_qstr = QString("%1").arg(m_dInitialMinScore);
	ui.minScorelineEdit->setText(m_qstr);
	m_dInitialMaxLap = objectRecognition.maxOverLap();
	m_qstr = QString("%1").arg(m_dInitialMaxLap);
	ui.maxOverLaplineEdit->setText(m_qstr);

	m_qTimer->start(100);
	m_vecStrExistence.clear();
	m_bDrawing = false;
	m_bCap = true;
}

