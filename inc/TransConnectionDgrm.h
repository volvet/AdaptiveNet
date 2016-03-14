//=============================================================================
/**
 *  @file    TransConnectionDgrm.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#ifndef _TRANSCONNECTIONDGRM_H_
#define _TRANSCONNECTIONDGRM_H_

#include "TransAcceptor.h"
#include "TransAcceptorDgrm.h"
#include "ClientDgrm.h"
#include "Timer.h"

#define   TRANS_KEEP_ALIVE_TIME          10000
#define   TRANS_KEEP_ALIVE_TIMEOUT        8
#define   TRANS_WAITACK_TIMEOUT           3



class CTransConnectionDgrm : public ITransConnection , public ITimerSink
{
public:	
	virtual ~CTransConnectionDgrm();

	enum {
		TRANS_CONNECTION_STATUS_DISCONNECTED = 0,
		TRANS_CONNECTION_STATUS_START = 1,
		TRANS_CONNECTION_STATUS_WAITACK = 2,
		TRANS_CONNECTION_STATUS_CONNECTED = 3,
	};

	virtual int setsink(ITransConnectionSink *sink) 
	{ 
		_sink = sink;
		return 0;
	}

protected:
	CTransConnectionDgrm(ITransConnectionSink *sink);
	CDataBlock * build_data_pdu(CDataBlock *data);
	CDataBlock * build_syn_pdu(void);
	CDataBlock * build_ack_pdu(void);
	CDataBlock * build_keep_alive_pdu(void);

protected:
	ITransConnectionSink   *_sink;
	unsigned short          _status;
	unsigned short          _curseq;
	unsigned short          _expect_seq;
	unsigned short          _tick_count;
	bool                    _bsend;
	bool                    _breceive;
	CNetTimer               *_timer;	
};

class CTransConnectionDgrmPassive : public CTransConnectionDgrm
{
public:
	CTransConnectionDgrmPassive(CTransAcceptorDgrm *acceptor,ITransConnectionSink *sink,const CInetAddr &addr);
	virtual ~CTransConnectionDgrmPassive();


	// ITimerSink
	virtual int on_timer(void *arg,ITimer *timer);

	// ITransConnection
	virtual int connect(const CInetAddr &addr);
	virtual int disconnect(int reason = 0);
	virtual int send(CDataBlock &data);
	virtual int open();
	virtual int close();
	virtual int setopt(int option,void *param);
	virtual int getopt(int option,void *param);

	int     handle_input(CDataBlock &data);	
	int     disconnect_by_acceptor(int reason);	
	int     check_remote_addr(const CInetAddr &addr);

protected:
	void    handle_input_ackpdu(unsigned char *buf,int len);

protected:
	CTransAcceptorDgrm  *_acceptor;
	CInetAddr            _remote_addr;
};

class CTransConnectionDgrmInitiative : public CTransConnectionDgrm , public IClientDgrmSink
{
public:
	CTransConnectionDgrmInitiative(ITransConnectionSink *sink);
	virtual ~CTransConnectionDgrmInitiative();

	// ITimerSink
	virtual int on_timer(void *arg,ITimer *timer);

	// ITransConnection
	virtual int connect(const CInetAddr &addr);
	virtual int disconnect(int reason = 0);
	virtual int send(CDataBlock &data);
	virtual int open();
	virtual int close();
	virtual int setopt(int option,void *param);
	virtual int getopt(int option,void *param);

	// IClientDgrmSink
	virtual int handle_input(CDataBlock &data,CInetAddr &addr);
	virtual int handle_close(int errcode);

protected:
	CClientDgrm _client_dgrm;
};



#endif


