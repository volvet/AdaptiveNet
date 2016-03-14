//=============================================================================
/**
 *  @file    Sock.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#ifndef _SOCK_H_
#define _SOCK_H_

#include "IpcSap.h"
#include "InetAddr.h"

class CSocket : public CIpcSap
{
public:
	CSocket();
	CSocket(int type,int protocol_family,int protocol = 0,int reuse_addr = 0);
	virtual ~CSocket();

	int set_option(int level,int option,void *optval,int optlen) const;
	int get_option(int level,int option,void *optval,int *optlen) const;
	int close();
	int get_local_addr(CInetAddr &addr);
	int get_remote_addr(CInetAddr &addr);
	int open(int type,int protocol_family,int protocol,int reuse_addr);
};


#endif

