//=============================================================================
/**
 *  @file    Addr.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#include "Addr.h"


const CAddr CAddr::sap_any(AF_ANY,-1);

CAddr::CAddr(int type, int size)
{
	base_set(type,size);
}

CAddr::~CAddr()
{
}

int CAddr::get_type() const
{
	return this->_addr_type;
}

void CAddr::set_type(int type)
{
	this->_addr_type = type;
}

int CAddr::get_size() const
{
	return this->_addr_size;
}

void CAddr::set_size(int size)
{
	this->_addr_size = size;
}

void CAddr::base_set(int type, int size)
{
	this->_addr_type = type;
	this->_addr_size = size;
}

int CAddr::operator ==(const CAddr &sap) const
{
	return ( (sap._addr_type == this->_addr_type) &&
		(sap._addr_size == this->_addr_size) );
}

int CAddr::operator !=(const CAddr &sap) const
{
	return !(this->operator == (sap));
}



