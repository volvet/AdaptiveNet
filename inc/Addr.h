//=============================================================================
/**
 *  @file    Addr.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#ifndef _ADDR_H_
#define _ADDR_H_

#include "OS.h"


#define  AF_ANY    -1

class CAddr
{
public:	
	CAddr(int type = AF_ANY,int size = -1);
	virtual ~CAddr();

	int get_type() const;
	void set_type(int type);
	int get_size() const;
	void set_size(int size);
    void base_set(int type,int size);

	int operator == (const CAddr &sap) const;
	int operator != (const CAddr &sap) const;

	static const CAddr sap_any;

protected:
	int _addr_type;
	int _addr_size;
};


#endif



