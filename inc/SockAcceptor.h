//=============================================================================
/**
 *  @file    SockAcceptor.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#ifndef _SOCKACCEPTOR_H_
#define _SOCKACCEPTOR_H_


#include "Sock.h"
#include "SockStream.h"

#define   AN_DEFAULT_BACKLOG      5

class  CSocketAcceptor : public CSocket
{
public:
	CSocketAcceptor();	
	CSocketAcceptor(const CInetAddr &local,int backlog = AN_DEFAULT_BACKLOG);
	virtual ~CSocketAcceptor();

	int open(const CInetAddr &local,int backlog = AN_DEFAULT_BACKLOG);
	int close();
	int accept(CSocketStream &new_stream,CInetAddr *remote_addr = 0);

};




#endif


















