//=============================================================================
/**
 *  @file    TimerQueue.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================

#ifndef _TIMERQUEUE_H_
#define _TIMERQUEUE_H_

#include "OS.h"
#include "TimeValue.h"
#include "EventHandler.h"

class CTimerNode
{
public:
	CTimerNode();
	CTimerNode(CEventHandler *pEH,CTimeValue &expire,CTimeValue &interval,const void *act,int id);	
	CTimerNode(const CTimerNode &node);
	virtual ~CTimerNode();	

	void set(CEventHandler *pEH,const CTimeValue &expire,const CTimeValue &interval,const void *act,int id);
	CEventHandler* get_event_handler(void)
	{
		return _pEH;
	}

	void set_event_handler(CEventHandler* pEH)
	{
		_pEH = pEH;
	}

	CTimeValue& get_exprie()
	{
		return _timer_expire;
	}

	void set_expire(const CTimeValue &expire)
	{
		_timer_expire = expire;
	}

	CTimeValue& get_interval()
	{
		return _timer_interval;
	}

	void set_interval(const CTimeValue &interval)
	{ 
		_timer_interval = interval;
	}

	void* get_act()
	{
		return _act;
	}

	void set_act(void *act)
	{
		_act = act;
	}

	int get_timer_id()
	{
		return _timer_id;
	}

	void set_timer_id(int id)
	{
		_timer_id = id;
	}

private:
	CEventHandler    *_pEH;
	CTimeValue       _timer_expire;
	CTimeValue       _timer_interval;
	void             *_act;
	int              _timer_id;
};


class CTimerQueue
{
public:
	CTimerQueue();
	virtual ~CTimerQueue();

	virtual int is_empty(void) = 0;
	virtual const CTimeValue earlist_time(void) = 0;
	virtual int schedule(CEventHandler *pEH,const void *act,const CTimeValue &delay,const CTimeValue &interval = CTimeValue::zero_time) = 0;	
	virtual int reset_interval(int id,const CTimeValue &interval) = 0;
	virtual int reset_interval(CEventHandler *pEH,const CTimeValue &interval) = 0;
	virtual int cancel(int id) = 0;
	virtual int cancel(CEventHandler *pEH) = 0;
	virtual int expire(const CTimeValue &cur_time) = 0;
	virtual int expire(void) = 0;
	virtual int calculate_timeout(CTimeValue *timeout) = 0;	

protected:
	virtual int reschedule(CTimerNode *pNode) = 0;
};


#endif



