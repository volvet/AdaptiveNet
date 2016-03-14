//=============================================================================
/**
 *  @file    TimerList.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#ifndef _TIMERLIST_H_
#define _TIMERLIST_H_


#include <list>
#include "TimerQueue.h"


class CTimerList : public CTimerQueue
{
public:
	CTimerList();
	virtual ~CTimerList();

    virtual int is_empty(void);
	virtual const CTimeValue earlist_time(void);
	virtual int schedule(CEventHandler *pEH,const void *act,const CTimeValue &delay,const CTimeValue &interval);
	virtual int reset_interval(CEventHandler *pEH,const CTimeValue &interval);
	virtual int reset_interval(int id,const CTimeValue &interval);
	virtual int cancel(CEventHandler *pEH);
	virtual int cancel(int id);
	virtual int expire(const CTimeValue &cur_time);
	virtual int expire();
	virtual int calculate_timeout(CTimeValue *timeout);

protected:
	void    clear();	
	virtual int reschedule(CTimerNode *pNode);

private:
	std::list<CTimerNode*>   nodes;
};



#endif


