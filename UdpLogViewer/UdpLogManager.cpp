#include "stdafx.h"
#include "UdpLogManager.h"

UdpLogManager::UdpLogManager(void) : evtTarget_(NULL)
{

}

UdpLogManager::~UdpLogManager(void)
{
	destroy();
}

void UdpLogManager::destroy() 
{
	if(udpSession_)
	{
		udpSession_->close();
	}
	runner_.close();
}

void UdpLogManager::listen(int port) 
{
	udpSession_ = boost::shared_ptr<CNetUdpSession>( new CNetUdpSession(runner_.io_service()) );
	udpSession_->listen(port);
	udpSession_->setEvent(this);
}


void UdpLogManager::onINetUdpSessionEvent_Received( CNetUdpSession *session, const std::string &address, int port, const std::string &buffer )
{
	if(evtTarget_)
		evtTarget_->onIUdpLogManagerEvent_LogReceived(address, port, buffer);
}