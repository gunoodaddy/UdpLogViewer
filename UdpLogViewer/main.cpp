#include "stdafx.h"
#include "UdpLogViewer.h"
#include <QtGui/QApplication>
#include "GlobalEvent.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

#if defined(Q_WS_WIN)

	HANDLE semaphore;
	semaphore = CreateSemaphore(NULL, 1, 1, L"gunoodaddy-0813-sharedpainter");
	BOOL alreadyExist = (GetLastError() == ERROR_ALREADY_EXISTS);
	if(alreadyExist)
	{
		QString f = qApp->applicationDirPath() + QDir::separator() + "MULTI_INSTANCE.opt";
		if( ! QFileInfo( f ).isFile() )
		{
			::MessageBoxA( NULL, "This program is running already.", PROGRAM_TEXT, MB_OK );
			return -1;
		}
	}
#endif

	a.installEventFilter(GlobalEventPtr());
	UdpLogViewer w;
	w.show();
	return a.exec();
}
