//=============================================================================
/**
 *  @file    SockStream.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#ifndef _SOCKSTREAM_H_
#define _SOCKSTREAM_H_

#include "SockIO.h"

class CSocketStream : public CSocketIO
{
public:
	CSocketStream();
	virtual ~CSocketStream();

	int   close_reader();
	int   close_writer();
	int   open();
	int   close();
};






#endif


