//=============================================================================
/**
 *  @file    TransAcceptorDgrm.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#ifndef _TRANSACCEPTORDGRM_H_
#define _TRANSACCEPTORDGRM_H_


#include <list>
#include "TransAcceptor.h"
#include "ClientDgrm.h"


#define TRANS_CONNECTION_HASH_SIZE     256



class CTransAcceptorDgrm : public ITransAcceptor , public IClientDgrmSink
{
public:
	CTransAcceptorDgrm(ITransAcceptorSink *sink);
	virtual ~CTransAcceptorDgrm();

	//ITransAccept
	virtual int start_listen(const CInetAddr &addr);
	virtual int stop_listen(int reason = 0);

	//IClientDgrmSink
	virtual int handle_input(CDataBlock &data,CInetAddr &addr);
	virtual int handle_close(int errcode);

	int   open();
	int   close();
	int   send(CDataBlock &data,const CInetAddr &addr);
	int   handle_connect(ITransConnection *con);	
	
	friend class CTransConnectionDgrmPassive;

protected:
	unsigned int hashcode_from_inetaddr(const CInetAddr &addr);
	CTransConnectionDgrmPassive * search_connection(const CInetAddr &addr);
	void remove_connection(const CInetAddr &addr);

private:
	CClientDgrm    _client_dgrm;
	ITransAcceptorSink *_sink;
	std::list<CTransConnectionDgrmPassive*>   _conlist[TRANS_CONNECTION_HASH_SIZE];
};



#endif



