//=============================================================================
/**
 *  @file    TransPdu.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#include "ByteStream.h"


class CTransPdu
{
public:
	enum {
		TRANS_PDU_VERSION = 0x1
	};

	enum {
		TRANS_PDU_TYPE_INVALID  = 0x0,
		TRANS_PDU_TYPE_STREAM_KEEP_ALIVE = 0x1,
		TRANS_PDU_TYPE_STREAM_DATA = 0x2,
		TRANS_PDU_TYPE_DGRM_SYN = 0x3,
		TRANS_PDU_TYPE_DGRM_ACK = 0x4,
		TRANS_PDU_TYPE_DGRM_FIN = 0x5,
		TRANS_PDU_TYPE_DGRM_KEEP_ALIVE = 0x6,
		TRANS_PDU_TYPE_DGRM_DATA = 0x7,
		TRANS_PDU_TYPE_DGRM_SYN_NEED = 0x8,
	};

	CTransPdu(unsigned char type = TRANS_PDU_TYPE_INVALID,unsigned char version = TRANS_PDU_VERSION);
	virtual ~CTransPdu();

	unsigned char get_type() const { return _type; }
	unsigned char get_version() const { return _version; }

	virtual int encode(CByteStream &os);
	virtual int decode(CByteStream &is);
	virtual int get_length() const;

	static unsigned char peek_type(unsigned char *data);

private:
	unsigned char _type;
	unsigned char _version;
};

class CTransPduStreamKeepAlive : public CTransPdu
{
public:
	CTransPduStreamKeepAlive(unsigned char version = TRANS_PDU_VERSION);
	virtual ~CTransPduStreamKeepAlive();

};

class CTransPduStreamData : public CTransPdu
{
public:
	CTransPduStreamData(unsigned short length = 0,unsigned char *buf = NULL,unsigned char version = TRANS_PDU_VERSION);
	virtual ~CTransPduStreamData();

	virtual int encode(CByteStream &os);
	virtual int decode(CByteStream &os);
	virtual int get_length() const;
	virtual unsigned short get_data_length() const;

private:
	unsigned char *_data;
	unsigned short   _length;
};

class CTransPduDgrm : public CTransPdu
{
public:
	CTransPduDgrm(unsigned short seq = 0,unsigned char type = TRANS_PDU_TYPE_DGRM_SYN,unsigned char version = TRANS_PDU_VERSION);
	virtual ~CTransPduDgrm();

	virtual int encode(CByteStream &os);
	virtual int decode(CByteStream &is);
	virtual int get_length() const;
	
	int get_seq() const { return _seq; }

private:
	unsigned short _seq;
};


class CTransPduDgrmData : public CTransPduDgrm
{
public:
	CTransPduDgrmData(unsigned short seq = 0,unsigned short length = 0,unsigned char *buf = NULL,unsigned char type = TRANS_PDU_TYPE_DGRM_DATA,
		unsigned char version = TRANS_PDU_VERSION);
	virtual ~CTransPduDgrmData();

	virtual int encode(CByteStream &os);
	//virtual int decode(CByteStream &is);
	//virtual int get_length() const;

private:
	unsigned char *_data;
	unsigned short _length;
};


