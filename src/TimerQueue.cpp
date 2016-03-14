//=============================================================================
/**
 *  @file    TimerQueue.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================

#include "TimerQueue.h"



CTimerNode::CTimerNode()
{
	_pEH = NULL;
	_act = NULL;
}

CTimerNode::CTimerNode(CEventHandler *pEH,CTimeValue &expire,CTimeValue &interval,const void *act,int id)
{
	set(pEH,expire,interval,act,id);
}

CTimerNode::CTimerNode(const CTimerNode &node)
{
	set(node._pEH,node._timer_expire,node._timer_interval,node._act,node._timer_id);
}

CTimerNode::~CTimerNode()
{
}

void CTimerNode::set(CEventHandler *pEH, const CTimeValue &expire,const CTimeValue &interval,const void *act, int id)
{
	_pEH = pEH;
	_timer_expire = expire;
	_timer_interval = interval;
	_act = (void*)act;
	_timer_id = id;
}

CTimerQueue::CTimerQueue()
{
}

CTimerQueue::~CTimerQueue()
{
}

