//=============================================================================
/**
 *  @file    IpcSap.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================

#include "IpcSap.h"



CIpcSap::CIpcSap(void) :
	_handle(AN_INVALID_HANDLE)
{
}

CIpcSap::~CIpcSap()
{
}

int CIpcSap::control(int cmd,void *arg) const
{
	return OS::ioctl(_handle,cmd,arg);
}

int CIpcSap::enable(int value) const
{	
	switch(value)
	{
	case AN_NONBLOCK:
#ifdef _WIN32_
		{
			u_long nonblock = 1;
	        return OS::ioctl(_handle,FIONBIO,&nonblock);
		}
#else  // _LINUX_
		{
			int flag = ::fcntl(_handle,F_GETFL,0);
			if ( flag == -1 ){
				return -1;
			}
			flag |= O_NONBLOCK;
			if( ::fcntl(_handle,F_SETFL,0) == -1 ){
				return -1;
			}             
		}  		
#endif
		break;
	default:
		return -1;
	}
	return 0;
}

int CIpcSap::disable(int value) const
{	
	switch(value)
	{
	case AN_NONBLOCK:
#ifdef _WIN32_
		{
			u_long nonblock = 0;
		    return OS::ioctl(_handle,FIONBIO,&nonblock);
		}
#else // _LINUX_
		{
			int flag = ::fcntl(_handle,F_GETFL,0);
			if ( flag == -1 ){
				return -1;
			}
			flag &= ~O_NONBLOCK;
			if( ::fcntl(_handle,F_SETFL,0) == -1 ){
				return -1;
			}    
		}
#endif
		break;
	default:
		return -1;
	}
	return 0;

}

AN_HANDLE CIpcSap::get_handle() const
{
	return this->_handle;
}

void CIpcSap::set_handle(AN_HANDLE h)
{
	this->_handle = h;
}
