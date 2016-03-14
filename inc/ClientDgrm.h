//=============================================================================
/**
 *  @file    ClientDgrm.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================

#ifndef _CLIENTDGRM_H_
#define _CLIENTDGRM_H_

#include "DataBlock.h"
#include "EventHandler.h"
#include "Reactor.h"
#include "InetAddr.h"
#include "SockDgrm.h"

#define  CLIENT_DGRM_MAX_RECV_LEN      4096

class IClientDgrmSink
{
public:
	virtual int handle_input(CDataBlock &data,CInetAddr &addr) = 0;
	virtual int handle_close(int errcode) = 0;
	virtual ~IClientDgrmSink() { }
};

class CClientDgrm : public CEventHandler
{
public:
	enum {
		NOT_OPENED = 0,
		OPENED = 1,
		CONNECTED = 2
	};

	CClientDgrm(IClientDgrmSink *sink = NULL,CReactor *reactor = NULL);
	virtual ~CClientDgrm();

	virtual AN_HANDLE get_handle() const;
	virtual void set_handle(AN_HANDLE h);
	virtual int  handle_input(AN_HANDLE h = AN_INVALID_HANDLE);
	virtual int  handle_close(AN_HANDLE h = AN_INVALID_HANDLE);

	int  open(const CInetAddr &local);
	int  close();
	int  connect(const CInetAddr &remote_addr);
	int  send(CDataBlock &data);
	int  sendto(CDataBlock &data,const CInetAddr &to);

private:
	IClientDgrmSink  * _sink;
	CSocketDgrm        _socket_dgrm;
	int                _status;
	unsigned char      _recvbuf[CLIENT_DGRM_MAX_RECV_LEN];
};






#endif



