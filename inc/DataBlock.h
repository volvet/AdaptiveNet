//=============================================================================
/**
 *  @file    DataBlock.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================

#ifndef _DATABLOCK_H_
#define _DATABLOCK_H_

#include "OS.h"

class CDataBlock
{
public:
	CDataBlock(unsigned int datalen,unsigned int headlen);
	virtual ~CDataBlock();

	void   init(unsigned int datalen,unsigned int headlen);
	int    advance(unsigned int len);
	int    back(unsigned int len);
	int    expand(unsigned int len);
	unsigned char * get_buf(void) const;
	unsigned int    get_length(void) const;

private:
	unsigned char *_buf;
	unsigned int  _buflen;
	unsigned int  _cur;
	unsigned int  _curlen;
};

class CMessageBlock
{
public:
	CMessageBlock(unsigned int len);
	virtual ~CMessageBlock();

	void init(unsigned int len);
	void init(const CDataBlock &data);
	unsigned char *read_ptr(void) const;
	unsigned char *write_ptr(void) const;
	unsigned int get_length() const;
	unsigned int get_space() const;
	int  advance_read_ptr(unsigned int step);
	int  advance_write_ptr(unsigned int step);

private:
	unsigned char *_b_ptr;
	unsigned char *_r_ptr;
	unsigned char *_w_ptr;
	unsigned char *_e_ptr;	
};



#endif



