#pragma once

#include <vector>
#include <QtGui>
#include <QNetworkInterface>

class Util
{
public:
	Util(void);
	~Util(void);

	static bool stringTokenizer(const std::string &strSrc, const std::string& strDelimiter, std::vector<std::string> &strList)
	{
		if(strDelimiter.length() <= 0)
			return false;

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

	static std::string getMyIPAddress( void )
	{
		std::string ip;

		// TODO : my active ip address
		foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
		{
			if ( (!(interface.flags() & QNetworkInterface::IsLoopBack)) 
				&& interface.flags() & QNetworkInterface::IsRunning )
			{
				foreach(QNetworkAddressEntry entry, interface.addressEntries())
				{
					if( entry.ip().protocol() != QAbstractSocket::IPv4Protocol )
						continue;

					return entry.ip().toString().toStdString();
				}
				break;
			}
		}

		if( ip.empty() )
			return "127.0.0.1";
		return ip;
	}
};
