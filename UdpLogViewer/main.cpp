#include "stdafx.h"
#include "UdpLogViewer.h"
#include <QtGui/QApplication>
#include "GlobalEvent.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.installEventFilter(GlobalEventPtr());
	UdpLogViewer w;
	w.show();
	return a.exec();
}
