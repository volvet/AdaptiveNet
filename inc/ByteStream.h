//=============================================================================
/**
 *  @file    ByteStream.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#ifndef _BYTESTREAM_H_
#define _BYTESTREAM_H_


#include "OS.h"


class CByteStream
{
public:
	CByteStream(unsigned char *buf,int len,int pos = 0);
	virtual ~CByteStream();

	unsigned char * get_buf() const { return this->_buf; }
	int get_length() const { return this->_buflen; }
	int get_pos() const { return this->_pos; }
	int get_space() const { return ( _buflen - _pos ); }
	void  attach(unsigned char *buf,int len, int pos = 0);

	void  write(void *data, int len);	
	void  read(void *data, int len);

	CByteStream & operator << ( unsigned char  value );
	CByteStream & operator << ( char value );
	CByteStream & operator << ( unsigned short value );
	CByteStream & operator << ( short v );
	CByteStream & operator << ( unsigned int value );
	CByteStream & operator << ( int value );
	CByteStream & operator << ( uint64 value );
	CByteStream & operator << ( int64 value );	

	CByteStream & operator >> ( unsigned char & value );
	CByteStream & operator >> ( char & value );
	CByteStream & operator >> ( unsigned short & value );
	CByteStream & operator >> ( short  & value );
	CByteStream & operator >> ( unsigned int  & value );
	CByteStream & operator >> ( int & value );
	CByteStream & operator >> ( uint64 & value );
	CByteStream & operator >> ( int64 & value ); 


private:
	unsigned char *_buf;
	int    _buflen;
	int    _pos;
};




#endif



