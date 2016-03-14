//=============================================================================
/**
 *  @file    SockDgrm.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#ifndef _SOCKDGRM_H_
#define _SOCKDGRM_H_


#include "SockIO.h"

class CSocketDgrm : public CSocketIO
{
public:
	CSocketDgrm();
	CSocketDgrm(const CInetAddr &local);
	virtual ~CSocketDgrm();

	int open();
	int open(const CInetAddr &local);
	int send(const void *buf,int len,const CInetAddr &addr,int flags = 0) const;	
	int send(const void *buf,int len,int flags = 0) const;
	int recv(void *buf,int len,CInetAddr &addr,int flags = 0) const;
	int connect(const CInetAddr &remote_addr);
};





#endif





