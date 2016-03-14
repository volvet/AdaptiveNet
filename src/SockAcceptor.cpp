//=============================================================================
/**
 *  @file    SockAcceptor.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================

#include "SockAcceptor.h"



CSocketAcceptor::CSocketAcceptor()
{
}

CSocketAcceptor::CSocketAcceptor(const CInetAddr &local, int backlog)
{
	open(local,backlog);
}

CSocketAcceptor::~CSocketAcceptor()
{
	close();
}

int CSocketAcceptor::open(const CInetAddr &local, int backlog)
{
	if( CSocket::open(AF_INET,SOCK_STREAM,0,0) != 0 ){
		return -1;
	}

	if( OS::bind(get_handle(),(const sockaddr*)local.get_addr(),local.get_size()) != 0 ){
		close();
		return -1;
	}

	if( OS::listen(get_handle(),backlog) != 0 ){
		close();
		return -1;
	}

	return 0;
}

int CSocketAcceptor::close()
{
	return CSocket::close();
}

int CSocketAcceptor::accept(CSocketStream &new_stream, CInetAddr *remote_addr)
{
	sockaddr *addr = NULL;
	int   *addrlen = NULL;
	int   len = 0;

	if( remote_addr != NULL ){
		addr = (sockaddr*)remote_addr->get_addr();
		len = remote_addr->get_size();
		addrlen = &len;
	}	

	new_stream.set_handle( OS::accept(get_handle(),addr,addrlen) );

	if( new_stream.get_handle() != AN_INVALID_HANDLE ){
		if( remote_addr != NULL ){
			remote_addr->set_size(len);
		}
		return 0;
	}

	return -1;
}













