//=============================================================================
/**
 *  @file    TimerList.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#include "TimerList.h"

CTimerList::CTimerList()
{
}

CTimerList::~CTimerList()
{
	clear();
}

int CTimerList::is_empty()
{
	return (nodes.empty() == true) ? 1 : 0;
}

const CTimeValue CTimerList::earlist_time()
{
	if( is_empty() ){
		return CTimeValue::max_time;
	}
	return nodes.front()->get_exprie();
}

int CTimerList::schedule(CEventHandler *pEH,const void *act,const CTimeValue &delay,const CTimeValue &interval)
{
    CTimerNode *pNode = new CTimerNode();

	pNode->set(pEH,delay,interval,act,(int)pNode);

	reschedule(pNode);	

	return pNode->get_timer_id();
}


int CTimerList::reset_interval(CEventHandler *pEH, const CTimeValue &interval)
{
	std::list<CTimerNode*>::iterator it = nodes.begin();
	while( it != nodes.end() ){
		if( (*it)->get_event_handler() == pEH ){
			(*it)->set_interval(interval);
			return 0;
		}
	}
	return -1;
}

int CTimerList::reset_interval(int id, const CTimeValue &interval)
{
	if( id == 0 ) return -1;

	CTimerNode *pNode = (CTimerNode*)id;

	pNode->set_interval(interval);

	return 0;
}

int CTimerList::cancel(CEventHandler *pEH)
{
	std::list<CTimerNode*>::iterator it = nodes.begin();

	while( it != nodes.end() ){
		if( (*it)->get_event_handler() == pEH ){
			CTimerNode *pNode = *it;
			delete pNode;
			nodes.erase(it);
			return 0;
		}
		it ++;
	}

    return -1;
}

int CTimerList::cancel(int id)
{
	std::list<CTimerNode*>::iterator it = nodes.begin();

	while( it != nodes.end() ){
		if( (*it)->get_timer_id() == id ){
			CTimerNode *pNode = *it;
			delete pNode;
			nodes.erase(it);
			return 0;
		}
		it ++;
	}

    return -1;
}

void CTimerList::clear()
{
	std::list<CTimerNode*>::iterator it = nodes.begin();

	while( it != nodes.end() ){
		CTimerNode* pNode = *it;
		delete pNode;
		it ++;
	}

	nodes.clear();
}

int CTimerList::expire(const CTimeValue &cur_time)
{
	int number_of_timer_expried = 0;

	if( is_empty() ){
		return number_of_timer_expried;
	}
	while( earlist_time() < cur_time ){		
		CTimerNode *pNode = nodes.front();
		nodes.pop_front();
		CEventHandler *pEH = pNode->get_event_handler();
		void *pArg = pNode->get_act();
		pEH->handle_timeout(cur_time,pArg);
		if( pNode->get_interval() > CTimeValue::zero_time ){
			pNode->set_expire(pNode->get_exprie() + pNode->get_interval());
			reschedule(pNode);
		} else {
			delete pNode;
		}
		number_of_timer_expried ++;
	}

	return number_of_timer_expried;
}

int CTimerList::expire()
{
	if( !is_empty() ){
		return expire(OS::gettimeofday());
	}
	return 0;
}

int CTimerList::calculate_timeout(CTimeValue *timeout)
{
	CTimeValue cur_time = OS::gettimeofday();
	if( earlist_time() > cur_time ){
		*timeout = earlist_time() - cur_time;
	} else {
		*timeout = CTimeValue::zero_time;
	}
	return 0;
}

int CTimerList::reschedule(CTimerNode *pNode)
{
	std::list<CTimerNode*>::iterator it = nodes.begin();

	while( it != nodes.end() ){
		if( (*it)->get_event_handler() == pNode->get_event_handler() ){
			CTimerNode *pTmp = *it;
			delete pTmp;
			nodes.erase(it);
			break;
		}
		it ++;
	}

	it = nodes.begin();
	while( (it!=nodes.end()) && ((*it)->get_exprie()<pNode->get_exprie()) ){
		it ++;
	}

	nodes.insert(it,pNode);

	return 0;
}



