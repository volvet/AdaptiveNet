//=============================================================================
/**
 *  @file    SockIO.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#include "SockIO.h"



CSocketIO::CSocketIO()
{
}

CSocketIO::~CSocketIO()
{
}

int CSocketIO::recv(void *buf, size_t len, int flags, const CTimeValue *timeout) const
{
	return OS::recv(get_handle(),buf,len,flags,timeout);
}

int CSocketIO::recv(void *buf, size_t len, const CTimeValue *timeout) const
{
	return OS::recv(get_handle(),buf,len,timeout);
}

int CSocketIO::recv(iovec iov[], size_t n, const CTimeValue *timeout) const
{
	return OS::recv(get_handle(),iov,n,timeout);
}

int CSocketIO::recv(iovec *iov, const CTimeValue *timeout) const
{
	return -1;
}

int CSocketIO::send(const void *buf, size_t len, int flags, const CTimeValue *timeout) const
{
	return OS::send(get_handle(),buf,len,flags,timeout);
}

int CSocketIO::send(const void *buf, size_t len, const CTimeValue *timeout) const
{
	return OS::send(get_handle(),buf,len,timeout);
}

int CSocketIO::send(iovec iov[], size_t n, const CTimeValue *timeout) const
{
	return OS::send(get_handle(),iov,n,timeout);
}

int CSocketIO::send(iovec *iov, const CTimeValue *timeout) const
{
	return  -1;
}


