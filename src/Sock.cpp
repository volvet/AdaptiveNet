//=============================================================================
/**
 *  @file    Sock.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#include "Sock.h"


CSocket::CSocket()
{
}

CSocket::CSocket(int type,int protocol_family,int protocol,int reuse_addr)
{
	open(type,protocol_family,protocol,reuse_addr);
}

CSocket::~CSocket()
{
}

int CSocket::set_option(int level, int option, void *optval, int optlen) const
{
	return OS::setsockopt(get_handle(),level,option,(const char*)optval,optlen);
}

int CSocket::get_option(int level, int option, void *optval, int *optlen) const
{
	return OS::getsockopt(get_handle(),level,option,(char*)optval,optlen);
}

int CSocket::open(int type, int protocol_family, int protocol, int reuse_addr)
{	
	close();

	set_handle( OS::socket(protocol_family,type,protocol) );
	if( get_handle() == AN_INVALID_HANDLE ){
		return -1;
	}

	if( protocol_family != PF_UNIX && reuse_addr ){
		int one = 1;
		if( set_option(SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one)) == -1 ){
			close();
			return -1;
		}
	}

	return 0;
}

int CSocket::close()
{
	if( get_handle() != AN_INVALID_HANDLE ){
		OS::closesocket(get_handle());
		set_handle(AN_INVALID_HANDLE);
	}
	return 0;
}

int CSocket::get_local_addr(CInetAddr &addr)
{
	int len = addr.get_size();
	sockaddr *name = (sockaddr*) addr.get_addr();

	if( OS::getsockname(get_handle(),name,&len) == -1 ){
		return -1;
	}

	addr.set_size(len);
	return 0;
}

int CSocket::get_remote_addr(CInetAddr &addr)
{
	int len = addr.get_size();
	sockaddr *name = (sockaddr*) addr.get_addr();

	if( OS::getpeername(get_handle(),name,&len) == -1 ){
		return -1;
	}

	addr.set_size(len);
	return 0;
}


