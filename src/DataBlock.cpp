//=============================================================================
/**
 *  @file    DataBlock.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#include "DataBlock.h"



CDataBlock::CDataBlock(unsigned int datalen, unsigned int headlen)
{
	_buf = NULL;
	_buflen = 0;
	_cur = 0;
	_curlen = 0;

	init(datalen,headlen);
}

CDataBlock::~CDataBlock()
{
	if( _buf ){
		delete _buf;
		_buf = NULL;
	}
}

void CDataBlock::init(unsigned int datalen, unsigned int headlen)
{
	if( _buflen != datalen + headlen ){
		if( _buf ){
			delete _buf;
			_buf = NULL;
		}
		_buf = new unsigned char[datalen + headlen];
		_buflen = datalen + headlen;
	}

	_cur = headlen;
	_curlen = 0;
}

unsigned char * CDataBlock::get_buf() const
{
	if( _buf ){
		return _buf + _cur;
	}
	return NULL;
}

unsigned int CDataBlock::get_length() const
{
	return _curlen;
}

int CDataBlock::advance(unsigned int len)
{
	if( len > _curlen ){
		return -1;
	}
	_cur += len;
	_curlen -= len;
	return 0;
}

int CDataBlock::back(unsigned int len)
{
	if( len > _cur ){
		return -1;
	}
	_cur -= len;
	_curlen += len;
	return 0;
}

int CDataBlock::expand(unsigned int len)
{
	if( len + _cur + _curlen > _buflen ){
		return -1;
	}
	_curlen += len;
	return 0;
}



CMessageBlock::CMessageBlock(unsigned int len)
{
	if( len < 256 ){
		len = 256;
	}
	_b_ptr = NULL;
	_r_ptr = NULL;
	_w_ptr = NULL;
	_r_ptr = NULL;
	init(len);
}

CMessageBlock::~CMessageBlock()
{
	if( _b_ptr ){
		delete _b_ptr;
		_b_ptr = NULL;
		_r_ptr = NULL;
		_w_ptr = NULL;
		_r_ptr = NULL;
	}
}

void CMessageBlock::init(unsigned int len)
{
	if( _b_ptr ){
		delete _b_ptr;
		_b_ptr = NULL;
	}
	_b_ptr = new unsigned char[len];
	_r_ptr = _b_ptr;
	_w_ptr = _b_ptr;
	_e_ptr = _b_ptr + len;
}

void CMessageBlock::init(const CDataBlock &data)
{
	init(data.get_length());
	memcpy(write_ptr(),data.get_buf(),data.get_length());
	advance_write_ptr(data.get_length());
}

unsigned char * CMessageBlock::read_ptr() const
{
	return _r_ptr;
}

unsigned char * CMessageBlock::write_ptr() const
{
	return _w_ptr;
}

unsigned int CMessageBlock::get_length() const
{
	return (unsigned int)(_w_ptr - _r_ptr);
}

unsigned int CMessageBlock::get_space() const
{
	return (unsigned int)(_e_ptr - _w_ptr);
}

int CMessageBlock::advance_read_ptr(unsigned int step)
{
	if( _w_ptr < _r_ptr + step ){
		return -1;
	}
	_r_ptr += step;
	return 0;
}

int CMessageBlock::advance_write_ptr(unsigned int step)
{
	if( _e_ptr < _w_ptr + step ){
		return -1;
	}
	_w_ptr += step;
	return 0;
}






