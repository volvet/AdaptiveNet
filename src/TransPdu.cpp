//=============================================================================
/**
 *  @file    TransPdu.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#include "TransPdu.h"


CTransPdu::CTransPdu(unsigned char type,unsigned char version)
{
	_type = type;
	_version = version;
}

CTransPdu::~CTransPdu()
{
}

int CTransPdu::encode(CByteStream &os)
{
	os << _version;
	os << _type;
	return 0;
}

int CTransPdu::decode(CByteStream &is)
{
	is >> _version;
	is >> _type;
	return 0;
}

int CTransPdu::get_length() const
{
	return sizeof(unsigned char) + sizeof(unsigned char);
}

unsigned char CTransPdu::peek_type(unsigned char *data)
{
	return data[1];
}


CTransPduStreamKeepAlive::CTransPduStreamKeepAlive(unsigned char version) : 
	CTransPdu(TRANS_PDU_TYPE_STREAM_KEEP_ALIVE,version)
{
}

CTransPduStreamKeepAlive::~CTransPduStreamKeepAlive()
{
}


CTransPduStreamData::CTransPduStreamData(unsigned short length,unsigned char *buf,unsigned char version) :
	CTransPdu(TRANS_PDU_TYPE_STREAM_DATA,version)
{
	_data = buf;
	_length = length;
}

CTransPduStreamData::~CTransPduStreamData()
{
}

int CTransPduStreamData::encode(CByteStream &os)
{
	CTransPdu::encode(os);

	os << _length;
	if( _data ){
		os.write(_data,_length);
	}
	return 0;
}

int CTransPduStreamData::decode(CByteStream &os)
{
	CTransPdu::decode(os);

	os >> _length;

	return 0;
}

int CTransPduStreamData::get_length() const
{
	return sizeof(_length) + CTransPdu::get_length();
}

unsigned short CTransPduStreamData::get_data_length() const
{
	return _length;
}


CTransPduDgrm::CTransPduDgrm(unsigned short seq,unsigned char type,unsigned char version) :
	CTransPdu(type,version)
{
	_seq = seq;
}

CTransPduDgrm::~CTransPduDgrm()
{
}

int CTransPduDgrm::encode(CByteStream &os)
{
	CTransPdu::encode(os);

	os << _seq;

	return 0;
}

int CTransPduDgrm::decode(CByteStream &is)
{
	CTransPdu ::decode(is);

	is >> _seq;

	return 0;
}

int CTransPduDgrm::get_length() const
{
	return sizeof(_seq) + CTransPdu::get_length();
}


CTransPduDgrmData::CTransPduDgrmData(unsigned short seq,unsigned short length,unsigned char *buf,unsigned char type,unsigned char version) : 
	CTransPduDgrm(seq,type,version)
{
	_data = buf;
	_length = length;
}

CTransPduDgrmData::~CTransPduDgrmData()
{
}


int CTransPduDgrmData::encode(CByteStream &os)
{
	CTransPduDgrm::encode(os);

	if( _data ){
		os.write(_data,_length);
	}
	return 0;
}




