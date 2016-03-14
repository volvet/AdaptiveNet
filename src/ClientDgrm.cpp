//=============================================================================
/**
 *  @file    ClientDgrm.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================

#include "ClientDgrm.h"


CClientDgrm::CClientDgrm(IClientDgrmSink *sink, CReactor *reactor) : 
	CEventHandler(reactor)
{
	_status = NOT_OPENED;
	_sink = sink;
}

CClientDgrm::~CClientDgrm()
{
	close();
}


AN_HANDLE CClientDgrm::get_handle() const
{
	return _socket_dgrm.get_handle();
}

void CClientDgrm::set_handle(AN_HANDLE h)
{
	return;
}

int CClientDgrm::handle_close(AN_HANDLE h)
{
	_status = NOT_OPENED;
	_socket_dgrm.close();
	if( _sink ){
		_sink->handle_close(0);
	}
	return 0;
}

int CClientDgrm::handle_input(AN_HANDLE h)
{
	if( get_handle() != h ){
		return -1;
	}

	CInetAddr addr;
	int bytes_read = _socket_dgrm.recv(_recvbuf,CLIENT_DGRM_MAX_RECV_LEN,addr);

	if( bytes_read < 0 ){
		printf("CClientDgrm::handle_input() : ???\n");
		return 0;
	}

	CDataBlock  data(bytes_read,0);
	memcpy(data.get_buf(),_recvbuf,bytes_read);
	data.expand(bytes_read);

	if( _sink ){
		_sink->handle_input(data,addr);
	}

	return 0;
}

int CClientDgrm::open(const CInetAddr &local)
{
	if( reactor() == NULL ){
		reactor(CReactor::instance());
	}

	if( _status != NOT_OPENED ){
		close();
	}

	if( _socket_dgrm.open(local)!= 0 ){
		return -1;
	}

	if( reactor()->register_handler(this,READ_MASK) != 0 ){
		close();
		return -1;
	}

	_status = OPENED;
	return 0;
}

int CClientDgrm::close()
{
	reactor()->remove_handler(this);
	//_socket_dgrm.close();
	//_status = NOT_OPENED;
	return 0;
}

int CClientDgrm::connect(const CInetAddr &remote_addr)
{
	if( _status != NOT_OPENED ){
		return -1;
	}

	CInetAddr local;

	if( open(local) != 0 ){
		return -1;
	}

	if( _socket_dgrm.connect(remote_addr) != 0 ){
		close();
		return -1;
	}

	_status = CONNECTED;
	return 0;
}

int CClientDgrm::send(CDataBlock &data)
{
	int bytes_send = 0;

	if( _status != CONNECTED ){
		return -1;
	}

	bytes_send = _socket_dgrm.send(data.get_buf(),data.get_length());

	if( bytes_send < data.get_length() ){
		printf("CClinetDgrm::send() : ??? \n");
	}
	return 0;
}


int CClientDgrm::sendto(CDataBlock &data, const CInetAddr &to)
{
	int bytes_send = 0;

	if( _status != OPENED ){
		return -1;
	}

	bytes_send = _socket_dgrm.send(data.get_buf(),data.get_length(),to);
	if( bytes_send < data.get_length() ){
		printf("CClinetDgrm::sendto() : ??? \n");
	}
	return 0;
}


