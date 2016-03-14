//=============================================================================
/**
 *  @file    TransAcceptor.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#ifndef _TRANSACCEPTOR_H_
#define _TRANSACCEPTOR_H_

#include "OS.h"
#include "InetAddr.h"
#include "DataBlock.h"

class ITransConnection;

class ITransAcceptorSink
{
public:
	virtual int handle_connect(ITransConnection *con) = 0;
	virtual ~ITransAcceptorSink() { }
};

class ITransAcceptor
{
public:
	virtual int start_listen(const CInetAddr &addr) = 0;
	virtual int stop_listen(int reason = 0) = 0;
	virtual ~ITransAcceptor() { }
};

class ITransConnectionSink
{
public:
	virtual int handle_input(CDataBlock &data) = 0;
	virtual int handle_output() = 0;
	virtual int handle_connect(int reason = 0) = 0;
	virtual int handle_disconnect(int reason = 0) = 0;
	virtual ~ITransConnectionSink() = 0;
};

class ITransConnection 
{
public:
	virtual int connect(const CInetAddr &addr) = 0;
	virtual int disconnect(int reason = 0) = 0;
	virtual int send(CDataBlock &data) = 0;	
	virtual int open() = 0;
	virtual int close() = 0;
	virtual int setopt(int option,void *param) = 0;
	virtual int getopt(int option,void *param) = 0;	
	virtual int setsink(ITransConnectionSink *sink) = 0;
	virtual ~ITransConnection() { }
};




#endif











