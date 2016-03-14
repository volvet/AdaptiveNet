//=============================================================================
/**
 *  @file    TransConnectionDgrm.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================

#include "TransConnectionDgrm.h"
#include "TransPdu.h"


CTransConnectionDgrm::CTransConnectionDgrm(ITransConnectionSink *sink)
{	
	_sink = sink;
	_curseq = (unsigned short)(&_curseq);
	_timer = NULL;
	_status = TRANS_CONNECTION_STATUS_DISCONNECTED;
	_bsend = false;
	_breceive = false;
	_expect_seq = 0;
	_tick_count = 0;
}

CTransConnectionDgrm::~CTransConnectionDgrm()
{	
}

CDataBlock * CTransConnectionDgrm::build_data_pdu(CDataBlock * data)
{
	if( data == NULL ) return NULL;

	CTransPduDgrmData dgrm_data( ++ _curseq );

	data->back(dgrm_data.get_length());

	CByteStream stream(data->get_buf(),data->get_length());

	dgrm_data.encode(stream);

	return data;
}

CDataBlock * CTransConnectionDgrm::build_syn_pdu()
{
	CTransPduDgrm  syn(_curseq,CTransPdu::TRANS_PDU_TYPE_DGRM_SYN);
	CDataBlock   *data = new CDataBlock(syn.get_length(),0);
	CByteStream  stream(data->get_buf(),data->get_length());

	syn.encode(stream);
	data->expand(syn.get_length());
	return data;
}

CDataBlock * CTransConnectionDgrm::build_ack_pdu()
{
	CTransPduDgrm  ack(_curseq,CTransPdu::TRANS_PDU_TYPE_DGRM_ACK);
	CDataBlock  *data = new CDataBlock(ack.get_length(),0);
	CByteStream stream(data->get_buf(),data->get_length());

	ack.encode(stream);
	data->expand(ack.get_length());
	return data;
}

CDataBlock * CTransConnectionDgrm::build_keep_alive_pdu()
{
	CTransPduDgrm keepalive(_curseq,CTransPdu::TRANS_PDU_TYPE_DGRM_KEEP_ALIVE);
	CDataBlock *data = new CDataBlock(keepalive.get_length(),0);
	CByteStream stream(data->get_buf(),data->get_length());

	keepalive.encode(stream);
	data->expand(keepalive.get_length());
	return data;
}






CTransConnectionDgrmPassive::CTransConnectionDgrmPassive(CTransAcceptorDgrm *acceptor,ITransConnectionSink *sink,const CInetAddr &addr) :
	CTransConnectionDgrm(sink)
{
	_acceptor = acceptor;
	_remote_addr = addr;
}

CTransConnectionDgrmPassive::~CTransConnectionDgrmPassive()
{
	close();
}

int CTransConnectionDgrmPassive::on_timer(void *arg,ITimer *timer)
{
	switch( _status )
	{
	case TRANS_CONNECTION_STATUS_WAITACK:
		{
			if( ++_tick_count > TRANS_WAITACK_TIMEOUT ){
				close();
				delete this;
				return 0;
			}

			CDataBlock *data = build_ack_pdu();
			_acceptor->send(*data,_remote_addr);
			delete data;
			_timer->cancel();
			_timer->schedule(_tick_count * 3000, NULL);
		}
		break;
	case TRANS_CONNECTION_STATUS_CONNECTED:
		{
			if( !_bsend ){
				CDataBlock *data = build_keep_alive_pdu();
				_acceptor->send(*data,_remote_addr);
				delete data;
			} else {
				_bsend = false;
			}

			if( ++ _tick_count > TRANS_KEEP_ALIVE_TIMEOUT ){
				if( !_breceive ){
					close();
					if( _sink ){
						_sink->handle_disconnect(0);
					}
					delete this;
					return 0;
				}
				_breceive = false;
				_tick_count = 0;
			}
		}
		break;
	default:
		break;
	}
	return 0;
}

int CTransConnectionDgrmPassive::connect(const CInetAddr &addr)
{
	return -1;
}

int CTransConnectionDgrmPassive::disconnect(int reason)
{
	close();
	return 0;
}

int CTransConnectionDgrmPassive::send(CDataBlock &data)
{
	_bsend = true;
	build_data_pdu(&data);
	return _acceptor->send(data,_remote_addr);
}

int CTransConnectionDgrmPassive::open()
{
	_status = TRANS_CONNECTION_STATUS_START;
	return 0;
}

int CTransConnectionDgrmPassive::close()
{
	if( _timer ){
		_timer->cancel();
		delete _timer;
		_timer = NULL;
	}

	if( _acceptor ){
		_acceptor->remove_connection(_remote_addr);
		_acceptor = NULL;
	}
	_status = TRANS_CONNECTION_STATUS_DISCONNECTED;
	return 0;
}

int CTransConnectionDgrmPassive::setopt(int option,void *param)
{
	return 0;
}

int CTransConnectionDgrmPassive::getopt(int option,void *param)
{
	return 0;
}


int CTransConnectionDgrmPassive::handle_input(CDataBlock &data)
{
	unsigned char *buf = data.get_buf();
	int    len = data.get_length();
	unsigned char type = CTransPdu::peek_type(buf);
	CTransPduDgrm   pdu;

	if( _status == TRANS_CONNECTION_STATUS_DISCONNECTED ){
		return 0;
	}

	if( len < pdu.get_length() ){
		printf("Invalid dgrm packet received!!!\n");
		return 0;
	}

	switch(type)
	{
	case CTransPdu::TRANS_PDU_TYPE_DGRM_SYN_NEED:
		{
			CDataBlock *syndata = build_syn_pdu();
			_acceptor->send(*syndata,_remote_addr);
			delete syndata;
		}
		break;
	case CTransPdu::TRANS_PDU_TYPE_DGRM_SYN:
		{
			CDataBlock *ackdata = build_ack_pdu();
			_acceptor->send(*ackdata,_remote_addr);
			delete ackdata;

			if( _status == TRANS_CONNECTION_STATUS_START ){
				_tick_count = 0;
				_status = TRANS_CONNECTION_STATUS_WAITACK;
				if( _timer == NULL ){
					_timer = new CNetTimer(this,CReactor::instance());
				} else {
					_timer->cancel();
				}
				_timer->schedule(1000,NULL);
			}
		}
		break;
	case CTransPdu::TRANS_PDU_TYPE_DGRM_ACK:
		{
			handle_input_ackpdu(buf,len);
		}
		break;
	case CTransPdu::TRANS_PDU_TYPE_DGRM_KEEP_ALIVE:
		{
			_breceive = true;
			handle_input_ackpdu(buf,len);
		}
		break;
	case CTransPdu::TRANS_PDU_TYPE_DGRM_DATA:
		{
			_breceive = true;
			handle_input_ackpdu(buf,len);

			if( _status != TRANS_CONNECTION_STATUS_CONNECTED ){
				CTransPduDgrmData   pdudata;
				CByteStream  stream(buf,len);

				pdudata.decode(stream);
				data.advance(pdudata.get_length());

				if( data.get_length() <= 0 ){
					printf("Invalid dgrm packet received!!!\n");
					return 0;
				}
				if( _sink ){
					_sink->handle_input(data);
				}
			}
		}
		break;
	default:
		printf("Invalid dgrm packet received!!!\n");
	}

	return 0;
}


void CTransConnectionDgrmPassive::handle_input_ackpdu(unsigned char *buf,int len)
{
	CTransPduDgrm pdu;
	if( _status == TRANS_CONNECTION_STATUS_WAITACK ){
		CByteStream stream(buf,len);
		pdu.decode(stream);
		_expect_seq = pdu.get_seq();
		_status = TRANS_CONNECTION_STATUS_CONNECTED;

		_tick_count = 0;
		if( _timer == NULL ){
			_timer = new CNetTimer(this,CReactor::instance());
		} else {
			_timer->cancel();
		}
		_timer->schedule(TRANS_KEEP_ALIVE_TIME,NULL);
		_acceptor->handle_connect(this);
		if( _sink ){
			_sink->handle_connect(0);
		}
	}
}

int CTransConnectionDgrmPassive::disconnect_by_acceptor(int reason)
{
	close();

	if( _sink ){
		_sink->handle_disconnect(reason);
	}

	delete this;

	return 0;
}

int CTransConnectionDgrmPassive::check_remote_addr(const CInetAddr & addr)
{
	return  _remote_addr.operator == (addr);
}




CTransConnectionDgrmInitiative::CTransConnectionDgrmInitiative(ITransConnectionSink *sink) : 
	CTransConnectionDgrm(sink),
	_client_dgrm(this)
{
}

CTransConnectionDgrmInitiative::~CTransConnectionDgrmInitiative()
{
	close();
}


int CTransConnectionDgrmInitiative::on_timer(void *arg,ITimer *timer)
{
	switch( _status )
	{
	case TRANS_CONNECTION_STATUS_DISCONNECTED:
		{
			if( _sink ){
				_sink->handle_connect(-1); // failed
			}
			_timer->cancel();
		}
		break;
	case TRANS_CONNECTION_STATUS_WAITACK:
		{
			if( ++_tick_count > 8 ){
				if( _sink ){
					_sink->handle_connect(-1); // failed
				}
				_timer->cancel();
				return 0;
			}

			CDataBlock *synpdu = build_syn_pdu();
			_client_dgrm.send(*synpdu);
			delete synpdu;
			_timer->cancel();
			_timer->schedule(_tick_count * 500, NULL);
		}
		break;
	case TRANS_CONNECTION_STATUS_CONNECTED:
		{
			if( !_bsend ){
				CDataBlock *keepalivepdu = build_keep_alive_pdu();
				_client_dgrm.send(*keepalivepdu);
				delete keepalivepdu;
			} else {
				_bsend = false;
			}

			if( ++_tick_count > TRANS_KEEP_ALIVE_TIMEOUT ){
				if( !_breceive ){
					//_timer->cancel();
					//if( _sink ){
					//	_sink->handle_disconnect(0);
					//}
					close();
					return 0;
				}
				_tick_count = 0;
				_breceive = false;
			}
		}
		break;
	}

	return 0;
}

int CTransConnectionDgrmInitiative::open()
{
	if( _sink == NULL ){
		return -1;
	}

	_status = TRANS_CONNECTION_STATUS_START;
	
	return 0;
}

int CTransConnectionDgrmInitiative::close()
{
	if( _timer ){
		_timer->cancel();
		delete _timer;
		_timer = NULL;
	}

	_client_dgrm.close();

	_status = TRANS_CONNECTION_STATUS_DISCONNECTED;

	return 0;
}


int CTransConnectionDgrmInitiative::connect(const CInetAddr &addr)
{
	close();

	if( _client_dgrm.connect(addr) != 0 ){
		if( _timer == NULL ){
			_timer = new CNetTimer(this,CReactor::instance());
		}
		_status = TRANS_CONNECTION_STATUS_DISCONNECTED;
		_timer->cancel();
		_timer->schedule(1,NULL);
		return 0;
	}

	CDataBlock *data = build_syn_pdu();
	_client_dgrm.send(*data);
	delete data;

	_status = TRANS_CONNECTION_STATUS_WAITACK;
	_tick_count = 0;
	if( _timer == NULL ){
		_timer = new CNetTimer(this,CReactor::instance());
	}

	_timer->cancel();
	_timer->schedule(500,NULL);

	return 0;
}

int CTransConnectionDgrmInitiative::disconnect(int reason)
{
	close();

	return 0;
}


int CTransConnectionDgrmInitiative::send(CDataBlock &data)
{
	_bsend = true;
	build_data_pdu(&data);

	return _client_dgrm.send(data);
}


int CTransConnectionDgrmInitiative::setopt(int option, void *param)
{
	return 0;
}

int CTransConnectionDgrmInitiative::getopt(int option,void *param)
{
	return 0;
}

int CTransConnectionDgrmInitiative::handle_input(CDataBlock &data,CInetAddr &addr)
{
	CTransPduDgrm  pdu;
	unsigned char *buf = data.get_buf();
	int len = data.get_length();

	if( len < pdu.get_length() ){
		printf("Invalid dgrm packet received!!!\n");
		return 0;
	}

	unsigned char type = CTransPdu::peek_type(buf);

	switch(type) 
	{
	case CTransPdu::TRANS_PDU_TYPE_DGRM_ACK:
		{
			if( _status == TRANS_CONNECTION_STATUS_WAITACK ){
				CDataBlock *ackdata = build_ack_pdu();
				_client_dgrm.send(*ackdata);
				delete ackdata;

				CByteStream stream(buf,len);
				pdu.decode(stream);

				_expect_seq = pdu.get_seq();
				_tick_count = 0;
				_status = TRANS_CONNECTION_STATUS_CONNECTED;
				if( _timer == NULL ){
					_timer = new CNetTimer(this,CReactor::instance());
				}
				_timer->schedule(TRANS_KEEP_ALIVE_TIME,NULL);
				if( _sink ){
					_sink->handle_connect(0);
				}
			}
		}
		break;
	case CTransPdu::TRANS_PDU_TYPE_DGRM_KEEP_ALIVE:
		{
			_breceive = true;
		}
		break;
	case CTransPdu::TRANS_PDU_TYPE_DGRM_DATA:
		{
			if( _status != TRANS_CONNECTION_STATUS_CONNECTED ){
				return 0;
			}
			_breceive = true;

			CTransPduDgrmData  datapdu;
			CByteStream stream(buf,len);

			datapdu.decode(stream);
			data.advance(datapdu.get_length());

			if( _sink ){
				_sink->handle_input(data);
			}
		}
		break;
	default:
		{
			printf("Invalid dgrm packet received!!!\n");
		}
		break;
	}

	return 0;
}

int CTransConnectionDgrmInitiative::handle_close(int errcode)
{
	if( _sink ){
		_sink->handle_disconnect(errcode);
	}

	return 0;
}



