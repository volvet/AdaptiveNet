//=============================================================================
/**
 *  @file    EventHandler.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#include "EventHandler.h"



CEventHandler::CEventHandler(CReactor *r,int p,EVENT_MASK m) :
	_reactor(r),
	_priority(p),
	_mask(m)
{	
}

CEventHandler::~CEventHandler()
{
	if( this->_reactor != 0 ){
		// TODO
	}
}

AN_HANDLE CEventHandler::get_handle() const
{
	return AN_INVALID_HANDLE;
}

void CEventHandler::set_handle(AN_HANDLE h)
{
	return;
}

int CEventHandler::priority() const
{
	return this->_priority;
}

void CEventHandler::priority(int p)
{
	this->_priority = p;
}

EVENT_MASK CEventHandler::get_event_mask(void) const
{
	return this->_mask;
}

void CEventHandler::set_event_mask(EVENT_MASK mask)
{
	this->_mask = mask;
}

CReactor *CEventHandler::reactor() const
{
	return this->_reactor;
}

void CEventHandler::reactor(CReactor *r)
{
	this->_reactor = r;
}

int CEventHandler::handle_input(AN_HANDLE fd)
{
	return 0;
}

int CEventHandler::handle_output(AN_HANDLE fd)
{
	return 0;
}

int CEventHandler::handle_exception(AN_HANDLE fd)
{
	return 0;
}

int CEventHandler::handle_timeout(const CTimeValue &cur_time,void *pArg)
{
	return 0;
}

int CEventHandler::handle_close(AN_HANDLE fd)
{
	return 0;
}

int CEventHandler::handle_exit(AN_HANDLE fd)
{
	return 0;
}

int CEventHandler::handle_signal(AN_HANDLE fd)
{
	return 0;
}

int CEventHandler::handle_qos(AN_HANDLE fd)
{
	return 0;
}

int CEventHandler::handle_group_qos(AN_HANDLE fd)
{
	return 0;
}

int CEventHandler::resume_handle(AN_HANDLE fd)
{
	return 0;
}


