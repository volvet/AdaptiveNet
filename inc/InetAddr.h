//=============================================================================
/**
 *  @file    InetAddr.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#ifndef _INETADDR_H_
#define _INETADDR_H_


#include "Addr.h"


class CInetAddr : public CAddr
{
public:
	CInetAddr();
	CInetAddr(const CInetAddr &addr);
	CInetAddr(const sockaddr_in *addr);
	CInetAddr(u_short port,u_long addr = INADDR_ANY);
	CInetAddr(u_short port,LPCSTR hostname);
	CInetAddr(LPCSTR addr);
	virtual ~CInetAddr();

	int    set(const CInetAddr &addr);
	int    set(const sockaddr_in *addr);
	int    set(u_short port,u_long addr);
	int    set(u_short port,LPCSTR hostname);
	int    set(LPCSTR addr);
	int    set_port(u_short port);

	const sockaddr_in * get_addr(void) const;
	u_short get_port(void) const;
	LPCSTR  get_host_addr(void) const;
	u_long  get_ip_addr(void) const;

	int  operator == (const CInetAddr &sap);
	int  operator != (const CInetAddr &sap);
	int  operator < (const CInetAddr &sap);

private:
	sockaddr_in _inet_addr;
};


#endif



