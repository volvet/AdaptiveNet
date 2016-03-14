//=============================================================================
/**
 *  @file    TransAcceptorDgrm.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================



#include "TransAcceptorDgrm.h"
#include "TransConnectionDgrm.h"
#include "TransPdu.h"
#include "ByteStream.h"



CTransAcceptorDgrm::CTransAcceptorDgrm(ITransAcceptorSink *sink) : _client_dgrm(this)	
{
	_sink = sink;
}


CTransAcceptorDgrm::~CTransAcceptorDgrm()
{
	close();
}

int CTransAcceptorDgrm::open()
{
	return 0;
}

int CTransAcceptorDgrm::close()
{
	int i;
	std::list<CTransConnectionDgrmPassive*>::iterator it;

	for( i=0;i<TRANS_CONNECTION_HASH_SIZE;i++ ){
		for( it=_conlist[i].begin();it!=_conlist[i].end();it++ ){
			(*it)->disconnect_by_acceptor(0);
		}
		_conlist[i].clear();
	}
	return 0;
}

int CTransAcceptorDgrm::start_listen(const CInetAddr &addr)
{
	_client_dgrm.close();

	return _client_dgrm.open(addr);	
}

int CTransAcceptorDgrm::stop_listen(int reason)
{
	close();
	_client_dgrm.close();

	return 0;
}

int CTransAcceptorDgrm::handle_input(CDataBlock &data,CInetAddr &addr)
{
	CTransConnectionDgrmPassive *con = search_connection(addr);

	if( con == NULL ){
		CTransPduDgrm  pdu;
		if( data.get_length() < (unsigned int)pdu.get_length() ){
			printf("Invalid dgrm packet received\n");
			return 0;
		}
		CByteStream stream(data.get_buf(),data.get_length());
		pdu.decode(stream);

		if( (pdu.get_type() == CTransPdu::TRANS_PDU_TYPE_DGRM_SYN) || (pdu.get_type() == CTransPdu::TRANS_PDU_TYPE_DGRM_SYN_NEED) ){
			con = new CTransConnectionDgrmPassive(this,NULL,addr);

			if( con->open() != 0 ){
				printf("Transfer Connection Create failed!!!\n");
				return 0;
			}

			con->handle_input(data);
			_conlist[hashcode_from_inetaddr(addr)].push_front(con);
			return 0;
		} else {
			printf("Invalid dgrm packet received\n");
			return 0;
		}
	} else {
		con->handle_input(data);
	}

	return 0;
}

int CTransAcceptorDgrm::handle_close(int errcode)
{
	close();
	return 0;
}

int CTransAcceptorDgrm::send(CDataBlock &data,const CInetAddr &addr)
{
	return _client_dgrm.sendto(data,addr);
}

unsigned int CTransAcceptorDgrm::hashcode_from_inetaddr(const CInetAddr &addr)
{
	const sockaddr_in *paddr = addr.get_addr();
	
	return paddr->sin_addr.s_addr % TRANS_CONNECTION_HASH_SIZE;
}

int CTransAcceptorDgrm::handle_connect(ITransConnection *con)
{
	return _sink->handle_connect(con);
}

CTransConnectionDgrmPassive * CTransAcceptorDgrm::search_connection(const CInetAddr &addr)
{
	int  hashcode = hashcode_from_inetaddr(addr);
	std::list<CTransConnectionDgrmPassive*>::iterator it;
	for( it=_conlist[hashcode].begin();it!=_conlist[hashcode].end();it++ ){
		if( (*it)->check_remote_addr(addr) ){
			return *it;
		}
	}
	return NULL;
}

void CTransAcceptorDgrm::remove_connection(const CInetAddr &addr)
{
	int hashcode = hashcode_from_inetaddr(addr);
	CTransConnectionDgrmPassive *con = search_connection(addr);

	_conlist[hashcode].remove(con);
}



