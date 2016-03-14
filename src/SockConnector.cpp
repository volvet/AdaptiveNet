//=============================================================================
/**
 *  @file    SockConnector.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#include "SockConnector.h"


CSocketConnector::CSocketConnector()
{
}

CSocketConnector::~CSocketConnector()
{
}


int CSocketConnector::connect(CSocketStream &new_stream, const CInetAddr &remote_addr)
{
	if( open_stream(new_stream) != 0 ){
		return -1;
	}

	return OS::connect(new_stream.get_handle(),(const sockaddr*)remote_addr.get_addr(),remote_addr.get_size());
}

int CSocketConnector::connect_nonblock(CSocketStream &new_stream,const CInetAddr &remote_addr)
{
	if( open_stream(new_stream) != 0 ){
		return -1;
	}

	new_stream.enable(CIpcSap::AN_NONBLOCK);

	return OS::connect(new_stream.get_handle(),(const sockaddr*)remote_addr.get_addr(),remote_addr.get_size());
}

int CSocketConnector::complete(CSocketStream & new_stream)
{
	return 0;
}

int CSocketConnector::open_stream(CSocketStream &new_stream)
{
	if( new_stream.get_handle() == AN_INVALID_HANDLE ){
		return new_stream.open();
	}
	return 0;
}


