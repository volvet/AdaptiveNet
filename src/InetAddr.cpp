//=============================================================================
/**
 *  @file    InetAddr.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================

#include "InetAddr.h"



CInetAddr::CInetAddr() :
	CAddr(AF_INET,sizeof(sockaddr_in))
{
	memset(&_inet_addr,0,sizeof(sockaddr_in));
	this->_inet_addr.sin_family = AF_INET;
}

CInetAddr::CInetAddr(const CInetAddr &addr)
{
	this->set(addr);
}

CInetAddr::CInetAddr(const sockaddr_in *addr)
{
	this->set(addr);
}

CInetAddr::CInetAddr(u_short port,u_long addr)
{
	this->set(port,addr);
}

CInetAddr::CInetAddr(u_short port,LPCSTR hostname)
{
	this->set(port,hostname);
}

CInetAddr::CInetAddr(LPCSTR addr)
{
	this->set(addr);
}

CInetAddr::~CInetAddr()
{
}

int CInetAddr::set(const CInetAddr &addr)
{
	base_set(addr.get_type(),addr.get_size());
	if( get_type() == AF_ANY ){
		memset(&_inet_addr,0,sizeof(sockaddr_in));
	} else {
		memcpy(&_inet_addr,&addr._inet_addr,sizeof(sockaddr_in));
	}
	return 0;
}

int CInetAddr::set(const sockaddr_in *addr)
{
	base_set(AF_INET,sizeof(sockaddr_in));
	memcpy(&_inet_addr,addr,sizeof(sockaddr_in));
	_inet_addr.sin_family = AF_INET;
	return 0;
}

int CInetAddr::set(u_short port,u_long addr)
{
	base_set(AF_INET,sizeof(sockaddr_in));
	memset(&_inet_addr,0,sizeof(sockaddr_in));
	_inet_addr.sin_family = AF_INET;
	_inet_addr.sin_addr.s_addr = htonl(addr);
	_inet_addr.sin_port = htons(port);
	return 0;
}

int CInetAddr::set(u_short port, LPCSTR hostname)
{
	u_long ip = INADDR_NONE;

	base_set(AF_INET,sizeof(sockaddr_in));
	memset(&_inet_addr,0,sizeof(sockaddr_in));
	_inet_addr.sin_family = AF_INET;

	if( hostname ){
		ip = inet_addr(hostname);
	}

	if( ip == INADDR_NONE ){
		return -1;
	}

	_inet_addr.sin_addr.s_addr = ip;
	_inet_addr.sin_port = htons(port);
	
	return 0;
}

int CInetAddr::set(LPCSTR addr)
{
	char t[256];
	char *pstr;

	if ( addr == NULL ){
		return -1;
	}
	strncpy(t,addr,256);
	pstr = strchr(t,':');

	if( pstr != NULL ){
		*pstr = '\0';
		++ pstr;
		if( strspn(pstr,"1234567890") == strlen(pstr) ){
			u_short port = static_cast<u_short>(atoi(pstr));
			set(port,t);
		} else {
			return -1;
		}
	} else {
		if( strspn(t,"1234567890") == strlen(t) ){
			u_short port = static_cast<u_short>(atoi(t));
			set(port,(u_long)INADDR_ANY);
		} else {
			return -1;
		}
	}

	return 0;
}

int CInetAddr::set_port(u_short port)
{
	_inet_addr.sin_port = htons(port);
	return 0;
}

const sockaddr_in * CInetAddr::get_addr() const
{
	return &_inet_addr;
}

u_short CInetAddr::get_port() const
{
	return ntohs(_inet_addr.sin_port);
}


LPCSTR CInetAddr::get_host_addr() const
{
	return inet_ntoa(_inet_addr.sin_addr);
}

u_long CInetAddr::get_ip_addr() const
{
	return ntohl(_inet_addr.sin_addr.s_addr);
}


int CInetAddr::operator ==(const CInetAddr &sap)
{
	return _inet_addr.sin_port == sap._inet_addr.sin_port && 
		memcmp(&_inet_addr.sin_addr,&sap._inet_addr.sin_addr,sizeof(_inet_addr.sin_addr)) == 0;
}

int CInetAddr::operator !=(const CInetAddr &sap)
{
	return !operator==(sap);
}



