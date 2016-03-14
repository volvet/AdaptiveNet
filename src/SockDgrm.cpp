//=============================================================================
/**
 *  @file    SockDgrm.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#include "SockDgrm.h"


CSocketDgrm::CSocketDgrm()
{
}

CSocketDgrm::CSocketDgrm(const CInetAddr &local)
{
	open(local);
}

CSocketDgrm::~CSocketDgrm()
{
}

int CSocketDgrm::open(const CInetAddr &local)
{
	if( CSocket::open(AF_INET,SOCK_DGRAM,0,0) != 0 ){
		return -1;
	}

	if( OS::bind(get_handle(),(const sockaddr*)local.get_addr(),local.get_size()) != 0 ){
		return -1;
	}
	return 0;
}

int CSocketDgrm::open()
{
	return CSocket::open(AF_INET,SOCK_DGRAM,0,0);
}

int CSocketDgrm::send(const void *buf, int len, const CInetAddr &addr, int flags) const
{
	return OS::sendto(get_handle(),buf,len,flags,(const sockaddr*)addr.get_addr(),addr.get_size());
}

int CSocketDgrm::send(const void *buf,int len,int flags) const
{
	return CSocketIO::send(buf,len,flags);
}

int CSocketDgrm::recv(void *buf, int len, CInetAddr &addr, int flags) const
{
	int size = addr.get_size();
	int rc = OS::recvfrom(get_handle(),buf,len,flags,(sockaddr*)addr.get_addr(),&size);
	addr.set_size(size);
	return rc;
}

int CSocketDgrm::connect(const CInetAddr &remote_addr)
{
	return OS::connect(get_handle(),(const sockaddr*)remote_addr.get_addr(),remote_addr.get_size());
}




