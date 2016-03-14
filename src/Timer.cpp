//=============================================================================
/**
 *  @file    Timer.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================



#include "Timer.h"



CNetTimer::CNetTimer(ITimerSink *sink,CReactor *r) :
CEventHandler(r)
{
	_sink = sink;
	_scheduled = 0;
}

CNetTimer::~CNetTimer()
{
	cancel();
}

int CNetTimer::schedule(int ms, void *arg)
{
	CTimeValue t(ms/1000,(ms%1000)*1000);

	if( _scheduled ){
		cancel();
	}

	reactor()->schedule_timer(this,arg,OS::gettimeofday(),t);
	_scheduled = 1;
	return 0;
}

int CNetTimer::cancel()
{
	if( _scheduled ){
		reactor()->cancel_timer(this);
		_scheduled = 0;
	}
	return 0;
}

int CNetTimer::handle_timeout(const CTimeValue &cur_time, void *arg)
{
	return _sink->on_timer(arg,this);
}


