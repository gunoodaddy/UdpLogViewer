#include "StdAfx.h"
#include "UpgradeManager.h"
#include "NetworkEngine/DefferedCaller.h"
#if defined(Q_WS_WIN)
#include <Shellapi.h>
#endif

#ifdef Q_WS_WIN
#define PROGRAM_UPGRADER_FILE_NAME	"SimplePatcher.exe"
#define REMOTE_UPGRADE_VERSION_URL	"https://raw.github.com/gunoodaddy/release/master/UdpLogViewer/version_win.txt"
#define REMOTE_UPGRADE_PATCH_URL	"https://raw.github.com/gunoodaddy/release/master/UdpLogViewer/patch_win.zip"
#elif Q_WS_MAC
#define PROGRAM_UPGRADER_FILE_NAME	"SimplePatcher"
#define REMOTE_UPGRADE_VERSION_URL	"https://raw.github.com/gunoodaddy/release/master/UdpLogViewer/version_mac.txt"
#define REMOTE_UPGRADE_PATCH_URL	"https://raw.github.com/gunoodaddy/release/master/UdpLogViewer/patch_mac.zip"
#else
#define PROGRAM_UPGRADER_FILE_NAME	""
#define REMOTE_UPGRADE_VERSION_URL	""
#define REMOTE_UPGRADE_PATCH_URL	""
#error "this platform not support"
#endif


#define DEFAULT_UPGRADE_CHECK_SECOND		20
#define DEFAULT_UPGRADE_FILE_NAME			"patch.zip"

CUpgradeManager::CUpgradeManager(void) : upgradeState_(FirstRunState), currentTimerSecond_(0), patchFileDownloaded_(false)
{
	start();
}

CUpgradeManager::~CUpgradeManager(void)
{
}

void CUpgradeManager::run()
{
	QTimer timer;
	timer.setInterval( 1000 );
	timer.moveToThread( this );
	connect( &timer, SIGNAL(timeout()), this, SLOT(onTimer()), Qt::DirectConnection );
	timer.start();

	nam_ = new QNetworkAccessManager();
	nam_->moveToThread( this );
	connect( nam_, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)), Qt::DirectConnection );

	exec();
	
	delete nam_;
}

void CUpgradeManager::close( void )
{
	quit();
}

bool CUpgradeManager::isAvailableUpgrade( void )
{
	if( ! patchFileDownloaded_ )
		return false;

	if( compareVersion( std::string(VERSION_TEXT), remoteVersion_ ) >= 0 )
		return false;

	return true;
}

void CUpgradeManager::doUpgradeNow( void )
{
	if( ! patchFileDownloaded_ )
		return;

	gotoState( WaitForUnlimited );

	QString path = qApp->applicationDirPath() + QDir::separator() + PROGRAM_UPGRADER_FILE_NAME;

	// Run Upgrader
#if defined(Q_WS_WIN)
	QString parameter = QFileInfo( QCoreApplication::applicationFilePath() ).fileName();
	parameter += " ";
	parameter += QString::fromStdWString(PROGRAM_SEMAPHORE_KEY);

	ShellExecute(NULL, L"open", path.toStdWString().c_str(), parameter.toStdWString().c_str(), L"", SW_SHOWNORMAL);
#else
	// TODO : other platform(MacOS) launch program.
	assert( false );
#endif

	// Program exit..
	qApp->exit(0);
}

void CUpgradeManager::stopVersionCheck( void )
{
	mutex_.lock();
	upgradeState_ = true;
	currentTimerSecond_ = 0;
	upgradeState_ = WaitForUnlimited;
	mutex_.unlock();
}

void CUpgradeManager::gotoState( int state )
{
	mutex_.lock();
	upgradeState_ = state;
	mutex_.unlock();
}

void CUpgradeManager::gotoNextState( void )
{
	mutex_.lock();
	upgradeState_++;
	mutex_.unlock();
}

void CUpgradeManager::doJob( void )
{
	static int prevState;

	if( prevState != upgradeState_ )
	{
		//qDebug() << "CUpgradeManager::doJob >> STATE CHANGED : " << upgradeState_;
		prevState = upgradeState_;
	}

	switch( upgradeState_ )
	{
	case FirstRunState:
		gotoState( DownloadVersionInfo );	// at the first, run right now once!
		break;
	case InitState:
		if( currentTimerSecond_ >= DEFAULT_UPGRADE_CHECK_SECOND )
		{
			gotoNextState();
			// go through next state
		}
		else
			break;
	case DownloadVersionInfo:
		currentReply_ = nam_->get(QNetworkRequest(QUrl(REMOTE_UPGRADE_VERSION_URL)));
		connect( currentReply_, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)), Qt::DirectConnection );
		gotoNextState();
		break;
	case DownloadingVersionInfo:
		// waiting for finishing download
		break;
	case DownloadCompleteVersionInfo:
		processVersionInfoFile();
		break;
	case DownloadPatchFile:
		currentReply_ = nam_->get(QNetworkRequest(QUrl(REMOTE_UPGRADE_PATCH_URL)));
		connect( currentReply_, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)), Qt::DirectConnection );
		gotoNextState();
		break;
	case DownloadingPatchFile:
		// waiting for finishing download
		break;
	case DownloadCompletePatchFile:
		processPatchFile();
		break;
	case WaitConfirmFromUser:
		// waiting for user confirm ( User must call doUpgradeNow or stopVersionCheck )
		break;
	case WaitForUnlimited:
		// waiting for unlimited... 
		break;
	case ErrorState:
		// TODO : UpgradeManager need more handling???
		mutex_.lock();
		currentTimerSecond_ = 0;
		mutex_.unlock();
		gotoState( InitState );
		break;
	}
}

void CUpgradeManager::onTimer( void )
{
	mutex_.lock();
	currentTimerSecond_++;
	mutex_.unlock();
	doJob();
}

void CUpgradeManager::processVersionInfoFile( void )
{
	mutex_.lock();

	int lineCnt = 0;
	while( 1 )
	{
		char buf[1024] = {0, };
		qint64 lineLength = currentReply_->readLine(buf, sizeof(buf));
		if( lineLength < 0 )
			break;

		if( lineCnt == 0 ) 
		{
			remoteVersion_.assign( buf, lineLength );
			QString tempStr(remoteVersion_.c_str());
			remoteVersion_ = tempStr.trimmed().toStdString();
			patchContents_ = "";
		}
		else 
		{
			std::string temp( buf, lineLength );
			patchContents_ += temp;
		}
		lineCnt++;
	}

	QString tempStr(patchContents_.c_str());
	patchContents_ = tempStr.trimmed().toStdString();

	mutex_.unlock();

	if( compareVersion( std::string(VERSION_TEXT), remoteVersion_ ) < 0 )
		gotoNextState();
	else
	{
		gotoState( ErrorState );
	}
}


void CUpgradeManager::processPatchFile( void )
{
	QByteArray res = currentReply_->readAll();

	QString path = qApp->applicationDirPath() + QDir::separator() + DEFAULT_UPGRADE_FILE_NAME;

	QFile f( path );
	if( !f.open( QIODevice::WriteOnly ) )
	{
		gotoState( ErrorState );
		return;
	}

	QDataStream out(&f);
	int ret = out.writeRawData( res.data(), res.size() );
	if( ret != res.size() )
	{
		gotoState( ErrorState );
		return;
	}
	f.close();

	patchFileDownloaded_ = true;

	CDefferedCaller::singleShot( boost::bind( &CUpgradeManager::fireObserver_NewVersion, this, remoteVersion_, patchContents_) );

	gotoNextState();
}


void CUpgradeManager::finished(QNetworkReply *reply)
{
	// error handling
	if(reply->error() != QNetworkReply::NoError)  
	{
		gotoState( ErrorState );
		
		qDebug() << reply->errorString();
		return;
	}

	gotoNextState();
}


void CUpgradeManager::downloadProgress(qint64 done, qint64 total)
{
	if( total == done )
		qDebug() << "CUpgradeManager::dataReadProgress complete" << done << total;
}
