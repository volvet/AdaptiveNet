//=============================================================================
/**
 *  @file    SockIO.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#ifndef _SOCKIO_H_
#define _SOCKIO_H_


#include "Sock.h"
#include "TimeValue.h"


class CSocketIO : public CSocket
{
public:
	CSocketIO();
	virtual ~CSocketIO();

	int  recv(void *buf,size_t len,int flags,const CTimeValue *timeout = 0) const;
	int  recv(void *buf,size_t len,const CTimeValue *timeout = 0) const;
    int  recv(iovec iov[],size_t n,const CTimeValue *timeout = 0) const;
	int  recv(iovec *iov,const CTimeValue *timeout = 0) const;
	int  send(const void *buf,size_t len,int flags,const CTimeValue *timeout = 0) const;
	int  send(const void *buf,size_t len,const CTimeValue *timeout = 0) const;
	int  send(iovec iov[],size_t n,const CTimeValue *timeout = 0) const;
	int  send(iovec *iov,const CTimeValue *timeout = 0) const;
};



#endif













