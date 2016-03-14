//=============================================================================
/**
 *  @file    SockConnector.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#ifndef _SOCKCONNECTOR_H_
#define _SOCKCONNECTOR_H_

#include "SockStream.h"


class CSocketConnector
{
public:
	CSocketConnector();
	virtual ~CSocketConnector();

	int   connect(CSocketStream &new_stream,const CInetAddr& remote_addr);
	int   connect_nonblock(CSocketStream &new_stream,const CInetAddr& remote_addr);
	int   complete(CSocketStream &new_stream);

protected:
	int   open_stream(CSocketStream &new_stream);

};


#endif


