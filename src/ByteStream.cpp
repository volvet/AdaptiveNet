//=============================================================================
/**
 *  @file    ByteStream.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#include "ByteStream.h"



CByteStream::CByteStream(unsigned char *buf, int len,int pos)
{
	attach(buf,len,pos);
}

CByteStream::~CByteStream()
{
}

void CByteStream::attach(unsigned char *buf, int len, int pos)
{
	this->_buf = buf;
	this->_buflen = len;
	this->_pos = pos;
}


void CByteStream::write(void *data, int len)
{
	if( len == 0 ) return;

	if( get_space() < len ) return;

	memcpy(_buf + _pos,data,len);

	_pos += len;
}

void CByteStream::read(void *data, int len)
{
	if( len == 0  ) return;

	if( get_space() < len ) return;

	memcpy(data, _buf + _pos,len);

	_pos += len;
}


CByteStream & CByteStream::operator <<(unsigned char value)
{
	write(&value,sizeof(unsigned char));
	return *this;
}

CByteStream & CByteStream::operator <<(char value)
{
	write(&value,sizeof(char));
	return *this;
}

CByteStream & CByteStream::operator <<(unsigned short value)
{
	value = htons(value);
	write(&value,sizeof(unsigned short));
	return *this;
}


CByteStream & CByteStream::operator <<(short value)
{
	value = htons(value);
	write(&value,sizeof(short));
	return *this;
}

CByteStream & CByteStream::operator <<(unsigned int value)
{
	value = htonl(value);
	write(&value,sizeof(unsigned int));
	return *this;
}

CByteStream & CByteStream::operator <<(int value)
{
	value = htonl(value);
	write(&value,sizeof(int));
	return *this;
}

CByteStream & CByteStream::operator <<(uint64 value)
{
	value = OS::htonll(value);
	write(&value,sizeof(uint64));
	return *this;
}

CByteStream & CByteStream::operator << (int64 value)
{
	value = OS::htonll(value);
	write(&value,sizeof(int64));
	return *this;
}


CByteStream & CByteStream::operator >> (unsigned char &value)
{
	read(&value,sizeof(unsigned char));
	return *this;
}

CByteStream & CByteStream::operator >> (char & value)
{
	read(&value,sizeof(char));
	return *this;
}

CByteStream & CByteStream::operator >> (unsigned short &value)
{
	read(&value,sizeof(unsigned short));
	value = ntohs(value);
	return *this;
}

CByteStream & CByteStream::operator >> (short & value)
{
	read(&value,sizeof(short));
	value = ntohs(value);
	return *this;
}

CByteStream & CByteStream::operator >> (unsigned int &value)
{
	read(&value,sizeof(unsigned int));
	value = ntohl(value);
	return *this;
}

CByteStream & CByteStream::operator >> (int & value)
{
	read( &value, sizeof(int) );
	value = ntohl(value);
	return *this;
}

CByteStream & CByteStream::operator >> (uint64 & value)
{
	read( &value, sizeof(uint64) );
	value = OS::ntohll(value);
	return *this;
}

CByteStream & CByteStream::operator >> (int64 & value)
{
	read(&value,sizeof(int64));
	value = OS::ntohll(value);
	return *this;
}




