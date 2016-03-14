//=============================================================================
/**
 *  @file    SockStream.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================



#include "SockStream.h"



CSocketStream::CSocketStream()
{
}

CSocketStream::~CSocketStream()
{
	close();
}


int CSocketStream::close_reader()
{
	return OS::shutdown(get_handle(),AN_SD_RECEIVE);
}

int CSocketStream::close_writer()
{
	return OS::shutdown(get_handle(),AN_SD_SEND);
}

int CSocketStream::close()
{
#ifdef _WIN32_
	close_writer();
#endif
	return CSocketIO::close();
}

int CSocketStream::open()
{
	return CSocketIO::open(AF_INET,SOCK_STREAM,0,0);
}




