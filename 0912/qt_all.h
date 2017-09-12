#pragma once
#include <QtWidgets/QMainWindow>
#include <QtCore\qtimer.h>
#include <QtWidgets\qlabel.h>
#include <QtWidgets\QGridLayout>
#include <QtWidgets\QCheckBox>
#include <QtWidgets\QFileDialog>
#include <QtWidgets\QMessageBox>
#include <QtCore\QTextCodec>
#include <QtGui\QMouseEvent>
#include <QtWidgets\QGroupBox>  
#include <QtWidgets\QRadioButton>
#if _DEBUG 
#pragma comment(lib, "..\\lib\\Qt5Cored.lib")
#pragma comment(lib, "..\\lib\\Qt5Guid.lib")
#pragma comment(lib, "..\\lib\\Qt5Widgetsd.lib")

#else
#pragma comment(lib, "..\\lib\\Qt5Core.lib")
#pragma comment(lib, "..\\lib\\Qt5Gui.lib")
#pragma comment(lib, "..\\lib\\Qt5Widgets.lib")
#endif	