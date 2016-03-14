
//=============================================================================
/**
 *  @file    Reactor.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================

#include "Reactor.h"
#include "ReactorImpl.h"

CReactor* CReactor::_reactor = 0;

CReactor::CReactor(CReactorImpl *impl)	
{
	this->_implementation = impl;	
	if( implementation() == 0 ){
#ifdef _WIN32_
		implementation(new CWMReactor());
#else
		implementation(new CLSReactor());
#endif
	}
}

CReactor::~CReactor()
{
	if( implementation() != 0 ){
		delete _implementation;	
		_implementation = 0;
	}
}

CReactor* CReactor::instance()
{
	if( 0 == CReactor::_reactor ){
		CReactor::_reactor = new CReactor();
	}
	return CReactor::_reactor;
}

void CReactor::close_singleton()
{
	if( CReactor::_reactor ){
		delete CReactor::_reactor;
		CReactor::_reactor = 0;
	}
}

CReactorImpl* CReactor::implementation()
{
	return this->_implementation;
}

void CReactor::implementation(CReactorImpl *impl)
{
	if( this->_implementation ){
		delete this->_implementation;
		this->_implementation = 0;
	}
	this->_implementation = impl;
}

int CReactor::run_event_loop()
{
	return this->_implementation->run_event_loop();
}

int CReactor::end_event_loop()
{
	return this->_implementation->end_event_loop();
}

int CReactor::reset_event_loop()
{
	return this->_implementation->reset_event_loop();
}

int CReactor::open()
{
	return this->_implementation->open();
}

int CReactor::close()
{
	return this->_implementation->close();
}

void CReactor::timer_queue(CTimerQueue *tq)
{
	return this->_implementation->timer_queue(tq);
}

CTimerQueue* CReactor::timer_queue()
{
	return this->_implementation->timer_queue();
}

int CReactor::register_handler(CEventHandler *event_handler,EVENT_MASK mask)
{	
	return this->_implementation->register_handler(event_handler,mask);
}

int CReactor::remove_handler(CEventHandler *event_handler)
{	
	return this->_implementation->remove_handler(event_handler,CEventHandler::NULL_MASK);
}

int CReactor::schedule_timer(CEventHandler *event_handler, const void *arg, const CTimeValue &delay, const CTimeValue &interval)
{
	return this->_implementation->schedule_timer(event_handler,arg,delay,interval);
}

int CReactor::cancel_timer(CEventHandler *event_handler)
{
	return this->_implementation->cancel_timer(event_handler);
}

int CReactor::notify(CEventHandler *event_handler, EVENT_MASK mask)
{
	return this->_implementation->notify(event_handler,mask);
}






