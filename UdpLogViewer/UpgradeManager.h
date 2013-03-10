/*                                                                                                                                           
* Copyright (c) 2012, Eunhyuk Kim(gunoodaddy) 
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*   * Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*   * Neither the name of Redis nor the names of its contributors may be used
*     to endorse or promote products derived from this software without
*     specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "Singleton.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

#define UpgradeManagerPtr()		CSingleton<CUpgradeManager>::Instance()

class CUpgradeManager;

class IUpgradeEvent
{
public:
	virtual void onIUpgradeEvent_NewVersion( CUpgradeManager *self, const std::string &version, const std::string &patchContents ) = 0;
};

class CUpgradeManager : public QThread
{
	Q_OBJECT

public:
	CUpgradeManager(void);
	~CUpgradeManager(void);

public:

	enum State{
		FirstRunState,
		InitState,
		DownloadVersionInfo,
		DownloadingVersionInfo,
		DownloadCompleteVersionInfo,
		DownloadPatchFile,
		DownloadingPatchFile,
		DownloadCompletePatchFile,
		WaitConfirmFromUser,
		WaitForUnlimited,
		ErrorState,
	};
	void registerObserver( IUpgradeEvent *obs )
	{
		observers_.remove( obs );
		observers_.push_back( obs );
	}

	void unregisterObserver( IUpgradeEvent *obs )
	{
		observers_.remove( obs );
	}

	void close( void );

	void doUpgradeNow( void );
	void stopVersionCheck( void );

	bool isAvailableUpgrade( void );

	inline bool stringTokenizer(const std::string &strSrc, const std::string& strDelimiter, std::vector<std::string> &strList)
	{
		size_t offsetPrev = 0;
		size_t offsetNext = 0;
		while( true )   
		{   
			offsetNext = strSrc.find(strDelimiter, offsetPrev);
			if(std::string::npos == offsetNext )
			{
				if(offsetPrev != 0)
				{
					std::string sub = strSrc.substr(offsetPrev);
					strList.push_back(sub);
				}
				break;
			}

			std::string sub = strSrc.substr(offsetPrev, offsetNext - offsetPrev);
			strList.push_back(sub);

			offsetPrev = offsetNext + strDelimiter.length();
		}

		return strList.size() ? true : false;
	}

	inline bool parseVersionString( const std::string &version, int &major, int &minor, int &revision )
	{
		std::vector<std::string> strList;
		if( ! stringTokenizer( version, ".", strList ) )
			return false;

		if( strList.size() < 3 )
			return false;

		major = atoi(strList[0].c_str());
		minor = atoi(strList[1].c_str());
		revision = atoi(strList[2].c_str());
		return true;
	}

	inline int compareVersion( const std::string &version1, const std::string &version2 )
	{
		int major1, minor1, rev1;
		int major2, minor2, rev2;
		if( !parseVersionString( version1, major1, minor1, rev1 ) )
			return 0;	// exception
		if( !parseVersionString( version2, major2, minor2, rev2 ) )
			return 0;	// exception

		if( major1 == major2 && minor1 == minor2 && rev1 == rev2 )	// most case
			return 0;

		if( major1 > major2 )
			return 1;
		else if( major1 < major2 )
			return -1;

		if( minor1 > minor2 )
			return 1;
		else if( minor1 < minor2 )
			return -1;

		if( rev1 > rev2 )
			return 1;
		else if( rev1 < rev2 )
			return -1;

		return 0;
	}

	bool currentVersionContents( std::string &version, std::string &patchContents )
	{
		mutex_.lock();
		if( remoteVersion_.empty() )
		{
			mutex_.unlock();
			return false;
		}
		version = remoteVersion_;
		patchContents = patchContents_;
		mutex_.unlock();
		return true;
	}

protected slots:
	void finished(QNetworkReply *reply);
	void downloadProgress(qint64 done, qint64 total);
	void onTimer( void );

private:
	void run();
	void gotoState( int state );
	void gotoNextState( void );
	void doJob( void );
	void processVersionInfoFile( void );
	void processPatchFile( void );

	void fireObserver_NewVersion( const std::string &version, const std::string &patchContents )
	{
		std::list<IUpgradeEvent *> observers = observers_;
		for( std::list<IUpgradeEvent *>::iterator it = observers.begin(); it != observers.end(); it++ )
		{
			(*it)->onIUpgradeEvent_NewVersion( this, version, patchContents );
		}
	}

private:
	// obsevers
	std::list<IUpgradeEvent *> observers_;
	
	int upgradeState_;
	int currentTimerSecond_;

	QNetworkAccessManager *nam_;
	QNetworkReply *currentReply_;
	std::string remoteVersion_;
	std::string patchContents_;
	bool patchFileDownloaded_;
	QMutex mutex_;
};
