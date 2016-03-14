//=============================================================================
/**
 *  @file    IpcSap.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================

#ifndef _IPCSAP_H_
#define _IPCSAP_H_

#include "OS.h"


class CIpcSap
{
public:
	enum {
		AN_NONBLOCK = 0
	};

	virtual ~CIpcSap();

	int        control(int cmd,void *arg) const;
	int        enable(int value) const;
	int        disable(int value) const;
	AN_HANDLE  get_handle() const;
	void       set_handle(AN_HANDLE h);

protected:
	CIpcSap(void);

private:
	AN_HANDLE  _handle;
};



#endif

