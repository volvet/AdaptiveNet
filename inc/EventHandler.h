//=============================================================================
/**
 *  @file    EventHandler.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================

#ifndef _EVENTHANDLER_H_
#define _EVENTHANDLER_H_

#include "OS.h"

typedef u_long EVENT_MASK;

class CReactor;

class CEventHandler
{
public:	
	virtual ~CEventHandler();

	enum {
		LO_PRIORITY = 0,
        HI_PRIORITY = 10
	};

	enum {
		NULL_MASK = 0,
		READ_MASK = (1 << 0),
		WRITE_MASK = (1 << 1),
		EXCEPT_MASK = (1 << 2),
		ACCEPT_MASK = (1 << 3),
		CONNECT_MASK = (1 << 4),
		TIMER_MASK = (1 << 5),
		QOS_MASK = (1 << 6),
		GROUP_QOS_MASK = (1 << 7),
		SINGAL_MASK = (1 << 8),
		ALL_EVENT_MASK = READ_MASK |
		                 WRITE_MASK |
						 EXCEPT_MASK |
						 ACCEPT_MASK |
						 CONNECT_MASK |
						 TIMER_MASK |
						 QOS_MASK |
						 GROUP_QOS_MASK |
						 SINGAL_MASK ,
		RWE_MASK = READ_MASK |
		           WRITE_MASK |
				EXCEPT_MASK,
		DONT_CALL = (1 << 9)						 
	};

	virtual AN_HANDLE get_handle(void) const;
	virtual void      set_handle(AN_HANDLE h);
	virtual int       priority(void) const;
	virtual void      priority(int p);
	virtual void        set_event_mask(EVENT_MASK mask);
	virtual EVENT_MASK  get_event_mask(void) const;
	virtual CReactor* reactor(void) const;
    virtual void      reactor(CReactor *r);

	virtual int       handle_input(AN_HANDLE fd = AN_INVALID_HANDLE);
	virtual int       handle_output(AN_HANDLE fd = AN_INVALID_HANDLE);
	virtual int       handle_exception(AN_HANDLE fd = AN_INVALID_HANDLE);
	virtual int       handle_timeout(const CTimeValue &cur_time,void *arg = NULL);
	virtual int       handle_close(AN_HANDLE fd = AN_INVALID_HANDLE);
    virtual int       handle_exit(AN_HANDLE fd = AN_INVALID_HANDLE);
	virtual int       handle_signal(AN_HANDLE fd = AN_INVALID_HANDLE);
	virtual int       resume_handle(AN_HANDLE fd = AN_INVALID_HANDLE);
	virtual int       handle_qos(AN_HANDLE fd = AN_INVALID_HANDLE);
	virtual int       handle_group_qos(AN_HANDLE fd = AN_INVALID_HANDLE);	


protected:
	CEventHandler(CReactor *r = 0,int p = CEventHandler::LO_PRIORITY,EVENT_MASK mask = NULL_MASK);

private:
    CReactor  *_reactor;
	int       _priority;
	EVENT_MASK  _mask;
};


#endif




