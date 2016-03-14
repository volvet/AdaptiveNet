
//=============================================================================
/**
 *  @file    Reactor.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#ifndef _REACTOR_H_
#define _REACTOR_H_


#include "EventHandler.h"
#include "TimerQueue.h"

class CReactorImpl
{
public:
	virtual int run_event_loop(void) = 0;
	virtual int end_event_loop(void) = 0;
	virtual int reset_event_loop(void) = 0;
	virtual int open() = 0;
	virtual int close() = 0;
	virtual void timer_queue(CTimerQueue *tq) = 0;
    virtual CTimerQueue* timer_queue(void) = 0;
	virtual int  register_handler(CEventHandler *event_handler,EVENT_MASK mask) = 0;
	virtual int  remove_handler(CEventHandler *event_handler,EVENT_MASK mask = CEventHandler::NULL_MASK) = 0;
	virtual int  schedule_timer(CEventHandler *event_handler,const void *arg,const CTimeValue &delay,const CTimeValue &interval) = 0;
	virtual int  cancel_timer(CEventHandler *event_handler) = 0;
	virtual int  notify(CEventHandler *event_handler,EVENT_MASK mask) = 0;
};

class CReactor
{
public:	
	virtual ~CReactor();

    static CReactor* instance(void);
	static void close_singleton(void);
	CReactorImpl* implementation(void);
	void implementation(CReactorImpl* impl);

	virtual int  run_event_loop(void);
	virtual int  end_event_loop(void);
	virtual int  reset_event_loop(void);

	virtual int  open();
	virtual int  close();
	virtual void  timer_queue(CTimerQueue *tq);
	virtual CTimerQueue* timer_queue(void);
	virtual int  register_handler(CEventHandler *event_handler,EVENT_MASK mask);
	virtual int  remove_handler(CEventHandler *event_handler);
	virtual int  schedule_timer(CEventHandler *event_handler,const void *arg,const CTimeValue &delay,const CTimeValue &interval = CTimeValue::zero_time);
	virtual int  cancel_timer(CEventHandler *event_handler);
	virtual int  notify(CEventHandler *event_handler,EVENT_MASK mask);

protected:
	CReactor(CReactorImpl *impl = 0);
	static CReactor  *_reactor;
	CReactorImpl  *_implementation;	
};







#endif





