#include "mutilobjetctrecognitionbasedonqt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MutilObjetctRecognitionBasedOnQt w;
	w.show();
	return a.exec();
}
