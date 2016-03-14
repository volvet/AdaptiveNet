//=============================================================================
/**
 *  @file    ReactorImpl.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#ifndef _REACTORIMPL_H_
#define _REACTORIMPL_H_


#include "Reactor.h"
#include <map>

#ifdef _WIN32_

class CWMReactor : public CReactorImpl
{
public:
	CWMReactor(CTimerQueue *tq = NULL);
	virtual ~CWMReactor();

	virtual int run_event_loop(void);
	virtual int end_event_loop(void);
	virtual int reset_event_loop(void);

	virtual int open();
	virtual int close();
	virtual void timer_queue(CTimerQueue *tq);
	virtual CTimerQueue* timer_queue();
	virtual int register_handler(CEventHandler *event_handler,EVENT_MASK mask);
	virtual int remove_handler(CEventHandler *event_handler,EVENT_MASK mask);
	virtual int schedule_timer(CEventHandler *event_handler,const void *arg,const CTimeValue &delay,const CTimeValue &interval);
	virtual int cancel_timer(CEventHandler *event_handler);
	virtual int notify(CEventHandler *event_handler,EVENT_MASK mask);

protected:
	static LRESULT CALLBACK MsgWndProc(HWND hWnd,UINT nMsg,WPARAM wParam,LPARAM lParam);
	static int WinSockStartup();
	static int WinSockCleanup();
	int  asy_select(AN_HANDLE s,EVENT_MASK mask);
	long   mask_to_sock_event(EVENT_MASK mask);
	EVENT_MASK  sock_event_to_mask(long lEvent);
	CEventHandler *search_event_handler(AN_HANDLE s);
	int  expire();
	int   handle_event(AN_HANDLE s,EVENT_MASK mask,int err_code);

private:
	std::map<AN_HANDLE,CEventHandler*>  _handlers;
	CTimerQueue   *_timer_queue;
	HWND          _message_sink;
	int           _end_flag;
	static int    _ws_inited;
};

#else // _LINUX_

class CLSReactor : public CReactorImpl
{
public:
	CLSReactor(CTimerQueue *tq = 0);
	virtual ~CLSReactor();

    virtual int run_event_loop(void);
	virtual int end_event_loop(void);
	virtual int reset_event_loop(void);

	virtual int open();
	virtual int close();
	virtual void timer_queue(CTimerQueue *tq);
	virtual CTimerQueue* timer_queue();
	virtual int register_handler(CEventHandler *event_handler,EVENT_MASK mask);
	virtual int remove_handler(CEventHandler *event_handler,EVENT_MASK mask);
	virtual int schedule_timer(CEventHandler *event_handler,const void *arg,const CTimeValue &delay,const CTimeValue &interval);
	virtual int cancel_timer(CEventHandler *event_handler);
	virtual int notify(CEventHandler *event_handler,EVENT_MASK mask);

protected:
	CEventHandler *search_event_handler(AN_HANDLE s);
    int  set_reg_signal_max(int max_sig);
	int  set_fd_max(int max_fd);
	int  set_rtsig_fd(AN_HANDLE s);
	int  handle_event(const CTimeValue &timeout);
	int  expire();
private:
	std::map<AN_HANDLE,CEventHandler*> _handlers;
	CTimerQueue   *_timer_queue;
	sigset_t     _signal_set;
	int  _end_flag;
    int  _max_fd;
};

#endif



#endif



