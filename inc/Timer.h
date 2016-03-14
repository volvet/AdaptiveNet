//=============================================================================
/**
 *  @file    Timer.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#ifndef _TIMER_H_
#define _TIMER_H_

#include "TimeValue.h"
#include "EventHandler.h"
#include "Reactor.h"



class ITimer
{
public:	
	virtual int schedule(int ms,void *arg)  = 0;
	virtual int cancel() = 0;
	virtual ~ITimer() { }
};

class  ITimerSink
{
public:
	virtual int on_timer(void *arg,ITimer *timer) = 0; 
	virtual ~ITimerSink() { }
};


class CNetTimer : public ITimer, public CEventHandler
{
public:
	CNetTimer(ITimerSink *sink,CReactor *r = NULL);
	virtual ~CNetTimer();

	virtual int schedule(int ms,void *arg);
	virtual int handle_timeout(const CTimeValue &cur_time,void *arg);
	virtual int cancel();

private:
	ITimerSink   *_sink;
	int          _scheduled;
};


#endif



