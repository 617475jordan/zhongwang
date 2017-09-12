#include "mutilobjetctrecognitionbasedonqt.h"

MutilObjetctRecognitionBasedOnQt::MutilObjetctRecognitionBasedOnQt(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this); 
	ui.minScorelineEdit->setText("0");
	ui.maxOverLaplineEdit->setText("0");
	initialize();
	objectRecognition.initialize();

	ui.mixComboBox->addItem("Original");
	ui.mixComboBox->addItem("Image Transform");
	ui.mixComboBox->addItem("imageEnhance");
	ui.mixComboBox->addItem("GaussianBlur");
	ui.showComboBox->addItem("Orignial");

	m_qStrOriginal = ui.mixComboBox->currentText();
	m_StrOriginal = m_qStrOriginal.toStdString();
	m_qTimer = new QTimer(this);
	m_qCurrentTimer = new QTimer(this);
	m_qCurrentTimer->start(33);
	
	connect(m_qTimer, SIGNAL(timeout()), this, SLOT(runDemo()));  // 时间到，读取当前摄像头信息
	connect(m_qCurrentTimer, SIGNAL(timeout()), this, SLOT(updateData()));
	connect(m_qCurrentTimer, SIGNAL(timeout()), this, SLOT(on_mixComboBox_clicked()));
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
	m_matImg = adjustMatSize(m_matImg);
	m_matImg = imageGaussar(m_matImg);
	m_matImg = imageEnhancement(m_matImg);
	m_matImg = imageTransform(m_matImg);
	m_matImg.copyTo(m_matScreenShootImg);
	objectRecognition.run( m_matImg);
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
			m_matImg= adjustMatSize(m_matImg);
			m_iPoint_X = m_matImg.cols;
			m_iPoint_Y = m_matImg.rows;
			m_matImg = imageGaussar(m_matImg);
			m_matImg = imageEnhancement(m_matImg);
			m_matImg = imageTransform(m_matImg);
			m_matImg.copyTo(m_matScreenShootImg);
			
			m_capQImg = Mat2QImage(m_matImg);
			ui.Cap->setPixmap(QPixmap::fromImage(m_capQImg));  // 将图片显示到label上 

			m_qTimer->start(100);
			m_bDrawing = false;
			m_bCap = true;
			
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
		ui.mixComboBox->clear();
		ui.mixComboBox->addItem("Original");
		ui.showComboBox->clear();
		ui.showComboBox->addItem("Original");
		m_bimagetransform = false, m_bimageEnhance = false;
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
		m_matScreenShootImg.copyTo(m_matCurrentImg);
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
	std::string m_strloadModelName = codec->fromUnicode(m_qstrFilename).data();//filename.toAscii().data()
	if (m_strloadModelName.size() > 0)
	{
		if (objectRecognition.loadExternalModel(m_strloadModelName))
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
	m_strloadModelName.clear();
	m_qstrFilename.clear();
}
void MutilObjetctRecognitionBasedOnQt::on_removeModelButton_clicked()
{
	QString m_qstr = ui.modelComboBox->currentText();
	String m_str = m_qstr.toStdString();

	if (objectRecognition.deleteModel(m_str))
	{
		QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板已删除"));
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板未删除或不存在"));
	}
	m_qstr.clear();
	m_str.clear();

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
		m_matSrcImg = adjustMatSize(m_matSrcImg);
		m_srcQImg = Mat2QImage(m_matSrcImg);
		ui.Image->setPixmap(QPixmap::fromImage(m_srcQImg));  // 将图片显示到label上 

		QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板加载成功"));
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("模板加载失败"));
	}
	m_strImgName.clear();
	m_qstrFilename.clear();
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
			//sprintf(m_charDstImgName, m_strDstImg, m_iCurrentImgId);
			objectRecognition.addModel(m_matCurrentImg, m_icurrentRoi_X, m_icurrentRoi_Y, m_icurrentRoi_Width, m_icurrentRoi_Height);
			
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

	m_vecModelName = objectRecognition.updateModelPath();
	m_iUpdate = m_vecModelName.size();
	if (m_iOrignial != m_iUpdate)
	{
		m_iOrignial = m_iUpdate;
		ui.modelComboBox->clear();
		for (int i = 0; i < m_vecModelName.size(); i++)
		{
			m_qstr = QString::fromStdString(m_vecModelName[i]);
			ui.modelComboBox->addItem(m_qstr);
			m_qstr.clear();

		}
	}
}

void MutilObjetctRecognitionBasedOnQt::on_mixComboBox_clicked()
{
	m_qStrUpdate = ui.mixComboBox->currentText();
	m_StrUpdate= m_qStrUpdate.toStdString();
	if (m_StrUpdate == m_StrOriginal)
	{
		return;
	}
	if (m_StrUpdate == "Original")
	{
		ui.showComboBox->clear();
		ui.showComboBox->addItem("Orignial");
	}
	else if (m_StrUpdate == "Image Transform")
	{
		imagetransform();
	}
	else if (m_StrUpdate == "imageEnhance")
	{
		imageEnhance();
	}
	else if (m_StrUpdate == "GaussianBlur")
	{
		imagegaussar();
	}
	m_StrOriginal = m_StrUpdate;
	m_qStrOriginal = m_qStrUpdate;

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
	m_dInitialMinScore = objectRecognition.currentMinScore();

	if (m_dInitialMinScore > 1 || m_dInitialMinScore < 0.001)
	{
		m_dInitialMinScore = 0.95;
	}
	m_qstr = QString("%1").arg(m_dInitialMinScore);
	ui.minScorelineEdit->setText(m_qstr);
	m_qstr.clear();

	m_dInitialMaxLap = objectRecognition.currentMaxOverLap();
	if (m_dInitialMaxLap > 1 || m_dInitialMaxLap < 0.001)
	{
		m_dInitialMaxLap = 0.15;
	}
	m_qstr = QString("%1").arg(m_dInitialMaxLap);
	ui.maxOverLaplineEdit->setText(m_qstr);
	m_qstr.clear();
	m_vecStrExistence.clear();
	modelFiles();
}

void MutilObjetctRecognitionBasedOnQt::imagetransform()
{
	ui.showComboBox->clear();
	
	ui.showComboBox->addItem("Orignial");
	ui.showComboBox->addItem("RightToLeft");
	ui.showComboBox->addItem("AboveToBelow");
	ui.showComboBox->addItem("Other");
	ui.showComboBox->setCurrentText(m_qimagetransform);
}

void MutilObjetctRecognitionBasedOnQt::imageEnhance()
{
	ui.showComboBox->clear();
	ui.showComboBox->addItem("Orignial");
	ui.showComboBox->addItem("Histogram");
	ui.showComboBox->addItem("Laplace");
	ui.showComboBox->addItem("Log");
	ui.showComboBox->addItem("Gamma");
	ui.showComboBox->addItem("Auto_White_Balance");
	ui.showComboBox->addItem("nightEnhance");
	ui.showComboBox->setCurrentText(m_qimageEnhance);
}

void MutilObjetctRecognitionBasedOnQt::imagegaussar()
{
	ui.showComboBox->clear();
	ui.showComboBox->addItem("Orignial");
	ui.showComboBox->addItem("3*3");
	ui.showComboBox->addItem("5*5");
	ui.showComboBox->addItem("9*9");
	ui.showComboBox->addItem("13*13");
	ui.showComboBox->addItem("17*17");
	ui.showComboBox->addItem("25*25");
	ui.showComboBox->setCurrentText(m_qimageGaussar);
}

void MutilObjetctRecognitionBasedOnQt::on_minScoreOkButton_clicked()
{
	QString 	m_qstr;
	m_qstr = ui.minScorelineEdit->text();
	double m_dMinScore = m_qstr.toDouble();
	if (m_dMinScore > 1 || m_dMinScore < 0.001)
	{
		m_dMinScore = 0.95;
	}
	m_qstr.clear();
	m_qstr = QString("%1").arg(m_dMinScore);
	ui.minScorelineEdit->setText(m_qstr);
	objectRecognition.updateMinScore(m_dMinScore);
	m_qstr.clear();
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
	m_qstr.clear();
	m_qstr = QString("%1").arg(m_dMaxOverLap);
	ui.maxOverLaplineEdit->setText(m_qstr);
	objectRecognition.updateMaxOverLap(m_dMaxOverLap);
	m_qstr.clear();
}



cv::Mat MutilObjetctRecognitionBasedOnQt::imageEnhancement(Mat m_mat)
{
	QString    m_qstr = ui.mixComboBox->currentText();
	string     m_srmixcombox = m_qstr.toStdString();
	if (m_srmixcombox == "imageEnhance")
	{
		m_qimageEnhance = ui.showComboBox->currentText();
		m_bimageEnhance = true;
		String m_strCurrentImg = m_qimageEnhance.toStdString();
		m_strimageEnhance = m_strCurrentImg;
		return currentImageEnhance(m_mat, m_strCurrentImg);
	}
	else
	{
		return currentImageEnhance(m_mat, m_strimageEnhance);
	}
	
}

cv::Mat MutilObjetctRecognitionBasedOnQt::imageTransform(Mat m_mat)
{
	QString    m_qstr = ui.mixComboBox->currentText();
	string     m_srmixcombox = m_qstr.toStdString();
	if (m_srmixcombox == "Image Transform")
	{
		m_qimagetransform = ui.showComboBox->currentText();
		m_bimagetransform = true;
		String m_strCurrent = m_qimagetransform.toStdString();
		m_strimagetransform = m_strCurrent;
		m_mat = currentImageTransform(m_mat, m_strCurrent);
		m_strCurrent.clear();	
	}
	else
	{
		m_mat = currentImageTransform(m_mat, m_strimagetransform);
	}
	
	return m_mat;	
}

cv::Mat MutilObjetctRecognitionBasedOnQt::imageGaussar(Mat m_mat)
{
	QString    m_qstr = ui.mixComboBox->currentText();
	string     m_srmixcombox = m_qstr.toStdString();
	if (m_srmixcombox == "GaussianBlur")
	{
		m_qimageGaussar = ui.showComboBox->currentText();
		m_bimagetransform = true;
		String m_strCurrent = m_qimageGaussar.toStdString();
		m_strimageGaussar = m_strCurrent;
		m_mat = currentImageGaussar(m_mat, m_strCurrent);
		m_strCurrent.clear();
	}
	else
	{
		m_mat = currentImageGaussar(m_mat, m_strimageGaussar);
	}

	return m_mat;
}

void MutilObjetctRecognitionBasedOnQt::modelFiles()
{
	vector<string> m_vecStrFiles;
	m_vecStrFiles.clear();
	m_vecStrFiles.push_back("data");
	m_vecStrFiles.push_back("image");
	m_vecStrFiles.push_back("model");
	getfiles.addFile(m_vecStrFiles);
}

cv::Mat MutilObjetctRecognitionBasedOnQt::adjustMatSize(Mat m_mat)
{
	
		if (m_mat.cols >= m_mat.rows&&m_mat.cols>=m_iSystem_X)
		{
			m_dAdjustSizeThrshold = m_iSystem_X*1.0 / m_mat.cols;
			cv::resize(m_mat, m_mat, Size(m_dAdjustSizeThrshold*m_mat.cols, m_dAdjustSizeThrshold*m_mat.rows));
		}
		else  if (m_mat.rows >= m_mat.cols&&m_mat.rows >= m_iSystem_Y)
		{
			m_dAdjustSizeThrshold = m_iSystem_Y / m_mat.rows;
			cv::resize(m_mat, m_mat, Size(m_dAdjustSizeThrshold*m_mat.cols, m_dAdjustSizeThrshold*m_mat.rows));
		}
		return m_mat;

}

cv::Mat MutilObjetctRecognitionBasedOnQt::currentImageGaussar(Mat m_mat, string m_strModel)
{
	if (m_strModel == "Orignial")
	{
		m_mat;
	}
	else if (m_strModel == "3*3")
	{
		GaussianBlur(m_matImg, m_matImg, Size(3, 3), 0);
	}
	else if (m_strModel == "5*5")
	{
		GaussianBlur(m_matImg, m_matImg, Size(5, 5), 0);
	}
	
	else if (m_strModel == "9*9")
	{
		GaussianBlur(m_matImg, m_matImg, Size(9, 9), 0);

	}
	else if (m_strModel == "13*13")
	{
		GaussianBlur(m_matImg, m_matImg, Size(13, 13), 0);

	}
	else if (m_strModel == "17*17")
	{
		GaussianBlur(m_matImg, m_matImg, Size(17, 17), 0);

	}
	else if (m_strModel == "25*25")
	{
		GaussianBlur(m_matImg, m_matImg, Size(25, 25), 0);

	}
	return m_mat;
}

Mat MutilObjetctRecognitionBasedOnQt::currentImageEnhance(Mat m_mat, string m_strModel)
{
	imageEnhancing	imageEnhancing;
	if (m_strModel == "Orignial")
	{
		m_strModel.clear();
		return m_mat;
	}
	else if (m_strModel == "Histogram")
	{	
		m_mat=imageEnhancing.equalizehist(m_mat);
	}
	else if (m_strModel == "Laplace")
	{
		m_mat=imageEnhancing.laplaceEnhance(m_mat);
	}
	else if (m_strModel == "Log")
	{	
		m_mat = imageEnhancing.logEnhance(m_mat);
	}
	else if (m_strModel == "Gamma")
	{
		m_mat = imageEnhancing.gammaEnhance(m_mat);
	}
	else if (m_strModel == "Auto_White_Balance")
	{
		m_mat = imageEnhancing.auto_White_Balance(m_mat, 1);
		
	}
	else if (m_strModel == "nightEnhance")
	{
		m_mat = imageEnhancing.nightEnhance(m_mat);
	}
	m_strModel.clear();
	return m_mat;
}

cv::Mat MutilObjetctRecognitionBasedOnQt::currentImageTransform(Mat m_mat, string m_strModel)
{
	if (m_strModel == "Orignial")
	{
		m_strModel.clear();
		return m_mat;
	}
	else if (m_strModel == "RightToLeft")
	{
		remapImage remapImage;
		m_mat = remapImage.imageRemap(m_mat);
	}
	else  if (m_strModel == "AboveToBelow")
	{
		flip(m_mat, m_mat, -1);
	}
	else if (m_strModel == "Other")
	{
		remapImage remapImage;
		m_mat = remapImage.imageRemap(m_mat);
		flip(m_mat, m_mat, -1);
	}
	return m_mat;
}

