//=============================================================================
/**
 *  @file    ReactorImpl.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#include "ReactorImpl.h"
#include "TimerList.h"



#ifdef _WIN32_

#define    MSG_WND_CLASS_NAME    "MsgWndClass"
#define    WM_SOCKET_NOTIFY      (WM_USER + 20)
#define    WM_SOCKET_SELECT      (WM_USER + 21)

int CWMReactor::_ws_inited = 0;

CWMReactor::CWMReactor(CTimerQueue *tq)
{
	_timer_queue = tq;
	_end_flag = 0;
	_message_sink = NULL;
}

CWMReactor::~CWMReactor()
{
	close();
}

int CWMReactor::run_event_loop()
{
	MSG msg;

	_end_flag = 0;
	while( ::GetMessage(&msg,NULL,0,0) && (!_end_flag) ){
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return 0;
}

int CWMReactor::end_event_loop()
{
	_end_flag = 1;
	return 0;
}

int CWMReactor::reset_event_loop()
{
	return -1;
}

int CWMReactor::open()
{
	if( WinSockStartup() != 0 ){
		return -1;
	}

	_message_sink = ::CreateWindow(MSG_WND_CLASS_NAME,NULL,0,0,0,0,0,NULL,NULL,GetModuleHandle(NULL),NULL);

	if( _message_sink == NULL ){
		close();
		return -1;			
	}

	::SetLastError(0);

	if( (::SetWindowLong(_message_sink,0,(LONG)this)==0) && (GetLastError()!=0) ){
		close();
		return -1;
	}

	if( _timer_queue == 0 ){
		_timer_queue = new CTimerList();
	}

	::SetTimer(_message_sink,0,USER_TIMER_MINIMUM,NULL);

	return 0;
}

int CWMReactor::close()
{
	if( _timer_queue ){
		delete _timer_queue;
		_timer_queue = NULL;
	}
    _handlers.clear();
	if( _message_sink != NULL ){
		::DestroyWindow(_message_sink);
		_message_sink = NULL;
	}

	WinSockCleanup();
	return 0;
}

void CWMReactor::timer_queue(CTimerQueue *tq)
{
	if( _timer_queue ){
		delete _timer_queue;
		_timer_queue = NULL;
	}
	_timer_queue = tq;
}

CTimerQueue* CWMReactor::timer_queue()
{
	return _timer_queue;
}


int CWMReactor::register_handler(CEventHandler *event_handler,EVENT_MASK mask)
{		
	if( (event_handler->get_handle()==AN_INVALID_HANDLE) || (mask==CEventHandler::NULL_MASK) ){
		return -1;
	}

	std::map<AN_HANDLE,CEventHandler*>::iterator it = _handlers.find(event_handler->get_handle());

	if( it!=_handlers.end() ){
		return -1;
	}

	event_handler->set_event_mask(mask);
	if( asy_select(event_handler->get_handle(),mask) != 0 ){
		return -1;
	}

	_handlers[event_handler->get_handle()] = event_handler;

	return 0;
}

int CWMReactor::remove_handler(CEventHandler *event_handler,EVENT_MASK mask)
{
	if( event_handler->get_handle() == AN_INVALID_HANDLE){
		return -1;
	}

	std::map<AN_HANDLE,CEventHandler*>::iterator it = _handlers.find(event_handler->get_handle());

	if( it == _handlers.end() ){
		return -1;
	}

	if( asy_select(event_handler->get_handle(),CEventHandler::NULL_MASK) != 0 ){
		// 
	}

	_handlers.erase(it);
	return event_handler->handle_close(event_handler->get_handle());
}

int CWMReactor::schedule_timer(CEventHandler *event_handler,const void *arg,const CTimeValue &delay,const CTimeValue &interval)
{
	return _timer_queue->schedule(event_handler,arg,delay,interval);
}

int CWMReactor::cancel_timer(CEventHandler *event_handler)
{
	return _timer_queue->cancel(event_handler);
}

int CWMReactor::notify(CEventHandler *event_handler,EVENT_MASK mask)
{
	if( event_handler->get_handle() == AN_INVALID_HANDLE ){
		return -1;
	}
	::PostMessage(_message_sink,WM_SOCKET_NOTIFY,(WPARAM)event_handler->get_handle(),mask);
	return 0;
}


LRESULT CWMReactor::MsgWndProc(HWND hWnd,UINT nMsg,WPARAM wParam,LPARAM lParam)
{
	CWMReactor *reactor = (CWMReactor*)GetWindowLong(hWnd,0);

	switch(nMsg)
	{
	case WM_SOCKET_NOTIFY:
		{
			reactor->expire();
			reactor->handle_event((AN_HANDLE)wParam,(EVENT_MASK)lParam,0);
			return 0;
		}
		break;		
	case WM_SOCKET_SELECT:
		{
            int err_code = WSAGETSELECTERROR(lParam);
			int lEvent = WSAGETSELECTEVENT(lParam);
			EVENT_MASK mask = 0;

			if( lEvent & FD_CONNECT ){
				mask |= CEventHandler::CONNECT_MASK;
			}
			if( lEvent & FD_READ ){
				mask |= CEventHandler::READ_MASK;
			}
			if( lEvent & FD_ACCEPT ){
				mask |= CEventHandler::ACCEPT_MASK;
			}
			if( lEvent & FD_WRITE ){
				mask |= CEventHandler::WRITE_MASK;
			}
			reactor->expire();
			reactor->handle_event((AN_HANDLE)wParam,mask,err_code);
			return 0;
		}
	case WM_TIMER:
		{			
			reactor->expire();
			return 0;
		}
	}

	return ::DefWindowProc(hWnd,nMsg,wParam,lParam);
}

int CWMReactor::WinSockStartup()
{
	if( _ws_inited ){
		return -1;
	}

	WSADATA   data;
	int       iResult = 0;

	iResult = WSAStartup(MAKEWORD(2,2), &data);
	if( iResult != 0 ){
		_ws_inited = 0;
		return -1;
	}

	WNDCLASS  wc;
	wc.style = 0;
	wc.lpfnWndProc = CWMReactor::MsgWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof(void*);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = MSG_WND_CLASS_NAME;

	_ws_inited = 1;

	if( ::RegisterClass(&wc) == 0 ){
		// failed 
		WinSockCleanup();
	}
	
	return 0;
}

int CWMReactor::WinSockCleanup()
{
	if( !_ws_inited ){
		return -1;
	}

	::UnregisterClass(MSG_WND_CLASS_NAME,GetModuleHandle(NULL));

	WSACleanup();    

	_ws_inited = 0;

	return 0;
}

int CWMReactor::asy_select(AN_HANDLE s,EVENT_MASK mask)
{
    long  lEvent = 0;

	if( mask != CEventHandler::NULL_MASK ){
		if( mask & CEventHandler::CONNECT_MASK )
			lEvent |=  FD_CONNECT;
		if( mask & CEventHandler::READ_MASK )
			lEvent |= FD_READ;
		if( mask & CEventHandler::WRITE_MASK )
			lEvent |= FD_WRITE;
		if( mask & CEventHandler::ACCEPT_MASK )
			lEvent |= FD_ACCEPT;		

		lEvent |= FD_CLOSE;
	}

	if( ::WSAAsyncSelect((SOCKET)s,_message_sink,WM_SOCKET_SELECT,lEvent) == SOCKET_ERROR ){
		return -1;
	}

	return 0;
}

CEventHandler * CWMReactor::search_event_handler(AN_HANDLE s)
{
	std::map<AN_HANDLE,CEventHandler*>::iterator it = _handlers.find(s);

	if( it == _handlers.end() ){
		return NULL;
	}
	return it->second;
}

int CWMReactor::expire()
{
	return _timer_queue->expire();
}

int CWMReactor::handle_event(AN_HANDLE s,EVENT_MASK mask,int err_code)
{
	int  rc = 0;
	CEventHandler *event_handler = search_event_handler(s);

	if( event_handler == NULL ){				
		return -1;
	}

	if( err_code && (!(mask&CEventHandler::CONNECT_MASK)) ){
		remove_handler(event_handler,CEventHandler::NULL_MASK);
		return -1;
	}

	if( mask & (CEventHandler::CONNECT_MASK|CEventHandler::ACCEPT_MASK|CEventHandler::READ_MASK) ){
		rc = event_handler->handle_input(s);
	}
	if( rc != 0 ){
		remove_handler(event_handler,CEventHandler::NULL_MASK);
		return rc;
	}
	if( mask & (CEventHandler::CONNECT_MASK|CEventHandler::WRITE_MASK) ){
		rc = event_handler->handle_output(s);
	}
	if( rc != 0 ){
		remove_handler(event_handler,CEventHandler::NULL_MASK);
	}
	return rc;
}

#else

#define REGSIGMAX      64 
#define FDMAX          64

_syscall1(int,_sysctl,struct __sysctl_args*,args);

CLSReactor::CLSReactor(CTimerQueue *tq)
{
	_timer_queue = tq;
	_end_flag = 0;
	_max_fd = 0;
}

CLSReactor::~CLSReactor()
{
	close();
}

int CLSReactor::open()
{
	set_reg_signal_max(REGSIGMAX);
	_max_fd = set_fd_max(FDMAX);
	
    printf("SIGRTMIN value is %d\n",SIGRTMIN);
	printf("SIGRTMAX value is %d\n",SIGRTMAX);
	printf("SIGIO value is %d\n",SIGIO);
	if(	::sigemptyset(&_signal_set) == -1 ){
		return -1;
	}
	if( ::sigaddset(&_signal_set,SIGRTMIN) == -1 ){
		return -1;
	}
	if( ::sigaddset(&_signal_set,SIGIO) == -1 ){
		return -1;
	}
	if( ::sigprocmask(SIG_BLOCK,&_signal_set,NULL) == -1 ){
		return -1;
	}
	if( _timer_queue == NULL ){
		_timer_queue = new CTimerList();
	}

	return 0;
}

int CLSReactor::close()
{
	if( _timer_queue ){
		delete _timer_queue;
		_timer_queue = 0;
	}
	_handlers.clear();
	return 0;
}

int CLSReactor::register_handler(CEventHandler *event_handler,EVENT_MASK mask)
{
    printf("CLSReactor::register_handler , registe fd is %d\n",event_handler->get_handle());
	
	if( event_handler->get_handle() == AN_INVALID_HANDLE ){
		return -1;
	}
	std::map<AN_HANDLE,CEventHandler*>::iterator it = _handlers.find(event_handler->get_handle());
	if( it!=_handlers.end() ){
		return -1;
	}
	event_handler->set_event_mask(mask);
	if(	set_rtsig_fd(event_handler->get_handle()) != 0 ){
		return -1;
	}
	_handlers[event_handler->get_handle()] = event_handler;

	if( (mask & CEventHandler::CONNECT_MASK) == 0 ){
		// need check ?
	}
	return 0;
}


int CLSReactor::remove_handler(CEventHandler *event_handler,EVENT_MASK mask)
{
	if( event_handler->get_handle() == AN_INVALID_HANDLE ){
		return -1;
	}
	std::map<AN_HANDLE,CEventHandler*>::iterator it = _handlers.find(event_handler->get_handle());
	if( it == _handlers.end() ){
		return -1;
	}
	
	int flags = ::fcntl(event_handler->get_handle(),F_GETFL);
	flags &= ~O_ASYNC;
	::fcntl(event_handler->get_handle(),F_SETFL,flags);

	_handlers.erase(it);	
	return event_handler->handle_close();
}

int CLSReactor::run_event_loop()
{
	_end_flag = 0;

	while( !_end_flag ){
		CTimeValue cur_time = OS::gettimeofday();
		CTimeValue earlist_time = _timer_queue->earlist_time();
		CTimeValue timeout;
		if( cur_time < earlist_time ){
			if( earlist_time == CTimeValue::max_time ){
				timeout.set(0,1);
			} else {
				timeout = earlist_time - cur_time;
			}
		}
		if(	handle_event(timeout) != 0 ){
			printf("where error ?\n");
			return -1;
		}
		expire();
	}

	return 0;
}

int CLSReactor::end_event_loop()
{
	_end_flag = 1;
	return 0;
}

int CLSReactor::reset_event_loop()
{
	return -1;
}

void CLSReactor::timer_queue(CTimerQueue *tq)
{
	if( _timer_queue ){
		delete _timer_queue;
		_timer_queue = NULL;
	}
	_timer_queue = tq;
}

CTimerQueue* CLSReactor::timer_queue()
{
	return _timer_queue;
}

int CLSReactor::schedule_timer(CEventHandler *event_handler,const void *arg,const CTimeValue &delay,const CTimeValue &interval)
{
	return _timer_queue->schedule(event_handler,arg,delay,interval);
}

int CLSReactor::cancel_timer(CEventHandler *event_handler)
{
	return _timer_queue->cancel(event_handler);
}

int CLSReactor::notify(CEventHandler *event_handler,EVENT_MASK mask)
{
	return -1;
}

CEventHandler* CLSReactor::search_event_handler(AN_HANDLE s)
{
	std::map<AN_HANDLE,CEventHandler*>::iterator it = _handlers.find(s);
	if( it == _handlers.end() ){
		return NULL;
	}
	return it->second;
}

int CLSReactor::set_reg_signal_max(int max_sig)
{
	__sysctl_args  args;
	int ctl_name[] = { CTL_KERN,KERN_RTSIGMAX };
	int oldval = 0;
    size_t  oldval_len = sizeof(oldval);
	
	args.name = ctl_name;
	args.nlen = sizeof(ctl_name)/sizeof(ctl_name[0]);
	args.oldval = &oldval;
	args.oldlenp = &oldval_len;
	args.newval = NULL;
	args.newlen = 0;

	if( _sysctl(&args) == -1 ){
		return -1;
	}

	printf("current signal max is %d\n",oldval);

    if( max_sig > oldval ){
		args.oldval = NULL;
		args.oldlenp = 0;
		args.newval = &max_sig;
		args.newlen = sizeof(max_sig);
		if( _sysctl(&args) == -1 ){
			return -1;
		}
    } else {
		max_sig = oldval;
	}

	printf("after call,now signal max is %d\n",max_sig);
	return max_sig;
}

int CLSReactor::set_fd_max(int max_fd)
{
	rlimit  rl;

	memset(&rl,0,sizeof(rl));
	if( ::getrlimit(RLIMIT_NOFILE,&rl) == -1 ){
		return -1;
	}
    if( rl.rlim_cur == RLIM_INFINITY ){
		return -1;
	}
    
    printf("current max fd is %d\n",static_cast<int>(rl.rlim_cur));
	if( max_fd > static_cast<int>(rl.rlim_cur) ){
		memset(&rl,0,sizeof(rl));
		rl.rlim_cur = max_fd;
		if( ::setrlimit(RLIMIT_NOFILE,&rl) == -1 ){
			return -1;
		}
	} else {
		max_fd = static_cast<int>(rl.rlim_cur);
	}

    printf("after call,now fd max is %d\n",max_fd);
	return max_fd;
}

int CLSReactor::set_rtsig_fd(AN_HANDLE s)
{
	int flags = 0;
	
	if( ::fcntl(s,F_SETOWN,getpid()) == -1 ){
		return -1;
	}	
	flags = ::fcntl(s,F_GETFL,0);
    flags |= O_NONBLOCK|FASYNC|O_RDWR;
	if( ::fcntl(s,F_SETFL,flags) == -1 ){
		return -1;
	}

	if( ::fcntl(s,F_SETSIG,SIGRTMIN) == -1 ){
		return -1;
	}	

	//if( ::fcntl(s,F_SETAUXFL,O_ONESIGFD) == -1 ){
	//	return -1;
	//}

	return 0;
}

int CLSReactor::expire()
{
	return _timer_queue->expire();
}

int CLSReactor::handle_event(const CTimeValue &timeout)
{
	struct timespec ts;
	struct timespec *pts = NULL;
	siginfo_t   siginfo;
	int  rc = 0;

	if( timeout != CTimeValue::max_time ){
		ts.tv_sec = timeout.sec();
		ts.tv_nsec = timeout.usec() * 1000;
		pts = &ts;
	}
	rc = ::sigtimedwait(&_signal_set,&siginfo,pts);
	if( rc == SIGIO ){
		printf("Really received SIGIO signal!!!\n");
		return 0;
	}	
	if( rc == -1 ){
		if( (errno==EINTR) || (errno==EAGAIN) ){
			return 0;
		}
		return -1;
	}
	if( rc != SIGRTMIN ){
		printf("Error signal?\n");
		return -1;
	}
	CEventHandler *event_handler = search_event_handler(siginfo.si_fd);
	if( event_handler == NULL ){
		printf("fd not registered!\n");
		return 0;
	}
	if( siginfo.si_band & (POLLERR|POLLHUP|POLLNVAL) ){
		printf("POLLERR\n");
		remove_handler(event_handler,CEventHandler::NULL_MASK);
		return 0;
	}

	rc = 0;
	if( siginfo.si_band & POLLIN ){
		if( event_handler->get_event_mask() & (CEventHandler::READ_MASK|CEventHandler::ACCEPT_MASK|CEventHandler::CONNECT_MASK) ){
			rc = event_handler->handle_input(siginfo.si_fd);
		}
	}
	if( (rc==0)	&& (siginfo.si_band&POLLOUT) ){
		if( event_handler->get_event_mask() & (CEventHandler::WRITE_MASK|CEventHandler::CONNECT_MASK) ){
			rc = event_handler->handle_output(siginfo.si_fd);
		}
	}
	if( rc!= 0 ){
		printf("handle event error!\n");
		remove_handler(event_handler,CEventHandler::NULL_MASK);
	}
	
	return 0;
}

#endif




