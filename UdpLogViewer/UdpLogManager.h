#pragma once

#include "NetUdpSession.h"
#include "NetServiceRunner.h"

class IUdpLogManagerEvent 
{
public:
	virtual void onIUdpLogManagerEvent_LogReceived(const std::string &address, int remotePort, const std::string &buffer ) = 0;
};

class UdpLogManager : public INetUdpSessionEvent
{
public:
	UdpLogManager(void);
	~UdpLogManager(void);

	void destroy();

	void listen(int port);

	void setEvent(IUdpLogManagerEvent *evt) 
	{
		evtTarget_ = evt;
	}

	virtual void onINetUdpSessionEvent_Received( CNetUdpSession *session, const std::string &remoteAddr, int remotePort, const std::string &buffer );

	IUdpLogManagerEvent *evtTarget_;
	CNetServiceRunner runner_;
	boost::shared_ptr< CNetUdpSession > udpSession_;
};
