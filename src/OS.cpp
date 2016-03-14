//=============================================================================
/**
 *  @file    OS.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#include "OS.h"
#include "TimeValue.h"



#ifdef _WIN32_

const static ULONGLONG FILETIME_to_timval_skew = 0x19db1ded53e8000;

CTimeValue OS::gettimeofday()
{
	FILETIME tfile;
	ULARGE_INTEGER   _100ns;
	long s = 0,us = 0;

	::GetSystemTimeAsFileTime(&tfile);

	memcpy(&_100ns,&tfile,sizeof(ULARGE_INTEGER));

	_100ns.QuadPart -= FILETIME_to_timval_skew;

	s = (long) _100ns.QuadPart/((double)(10000 * 1000));
	us = (long) (_100ns.QuadPart%(10000 * 1000))/10;

	return CTimeValue(s,us);
}

int OS::mutex_init(AN_MUTEX *m)
{
	::InitializeCriticalSection(m);
	return 0;
}

int OS::mutex_lock(AN_MUTEX *m)
{
	::EnterCriticalSection(m);
	return 0;
}

int OS::mutex_trylock(AN_MUTEX *m)
{
	BOOL rc = FALSE;

	//rc = ::TryEnterCriticalSection(m);

	return (rc == TRUE) ? 0 : -1;
}

int OS::mutex_unlock(AN_MUTEX *m)
{
	::LeaveCriticalSection(m);
	return 0;
}

int OS::mutex_destroy(AN_MUTEX *m)
{
	::DeleteCriticalSection(m);
	return 0;
}

int OS::ioctl(AN_HANDLE h, int cmd, void *arg)
{
	AN_SOCKET s = (AN_SOCKET)h;

	if( (::ioctlsocket(s,cmd,(u_long*)arg)) == SOCKET_ERROR ){
		return -1;
	}
	return 0;
}

int OS::closesocket(AN_HANDLE s)
{
	if( ::closesocket((SOCKET)s) == SOCKET_ERROR ){
		return -1;
	}
	return 0;
}

int OS::setsockopt(AN_HANDLE s, int level, int optname, const char *optval, int optlen)
{
	if( ::setsockopt((SOCKET)s,level,optname,optval,optlen) == SOCKET_ERROR ){
		return -1;
	}
	return 0;
}

int OS::getsockopt(AN_HANDLE s, int level, int optname, char *optval, int *optlen)
{
	if( ::getsockopt((SOCKET)s,level,optname,optval,optlen) == SOCKET_ERROR ){
		return -1;
	}
	return 0;
}

int OS::getsockname(AN_HANDLE s, sockaddr *name, int *namelen)
{
	if( ::getsockname((SOCKET)s,name,namelen) == SOCKET_ERROR ){
		return -1;
	}
	return 0;
}

int OS::getpeername(AN_HANDLE s, sockaddr *name, int *namelen)
{
	if( ::getpeername((SOCKET)s,name,namelen) == SOCKET_ERROR ){
		return -1;
	}
	return 0;
}

int OS::recv(AN_HANDLE s, void *buf, size_t len, int flags, const CTimeValue *timeout)
{
	int rc = -1;
	if( timeout == 0 ){
		rc = ::recv((SOCKET)s,(char*)buf,len,flags);
		if( rc == SOCKET_ERROR ){
			if( ::WSAGetLastError() == WSAEWOULDBLOCK ){
				rc = 1;
			} 
		}
	} 
	return rc;
}

int OS::recv(AN_HANDLE s,iovec iov[], size_t n, const CTimeValue *timeout)
{
	int rc = -1;
	if( timeout == 0 ){
		DWORD flags = 0;
		DWORD numberOfBytesRecvd = 0;
		if( ::WSARecv((SOCKET)s,(LPWSABUF)iov,n,&numberOfBytesRecvd,&flags,0,0) == SOCKET_ERROR ){
			rc = -1;
		} else {
			rc = numberOfBytesRecvd;
		}
	}
	return rc;
}

int OS::send(AN_HANDLE s,const void *buf,size_t len,int flags,const CTimeValue *timeout)
{
	int rc = -1;
	if( timeout == 0 ){
		rc = ::send((SOCKET)s,(char*)buf,len,flags);
		if( rc == SOCKET_ERROR ){
			if( ::WSAGetLastError() == WSAEWOULDBLOCK ){
				rc = 1;
			} 
		}
	}
	return rc;
}

int OS::send(AN_HANDLE s, iovec iov[], size_t n, const CTimeValue *timeout)
{
	int rc = -1;
	if( timeout == 0 ){
		DWORD flags = 0;
		DWORD numberOfBytesSent = 0;
		if( ::WSASend((SOCKET)s,(LPWSABUF)iov,n,&numberOfBytesSent,flags,0,0) == SOCKET_ERROR ){
			rc = -1;
		} else {
			rc = numberOfBytesSent;
		}
	}
	return rc;
}

int OS::bind(AN_HANDLE s, const sockaddr *addr, int addrlen)
{
	if( ::bind((SOCKET)s,addr,addrlen) == SOCKET_ERROR ){
		return -1;
	}
	return 0;
}

int OS::sendto(AN_HANDLE s, const void *msg, int len, int flags, const sockaddr *to, int tolen)
{
	if( ::sendto((SOCKET)s,(const char*)msg,len,flags,to,tolen) == SOCKET_ERROR ){
		return -1;
	}
	return 0;
}

int OS::recvfrom(AN_HANDLE s, void *buf, int len, int flags, sockaddr *from, int *fromlen)
{
	if( ::recvfrom((SOCKET)s,(char*)buf,len,flags,from,fromlen) == SOCKET_ERROR ){
		return -1;
	}
	return 0;
}

int OS::shutdown(AN_HANDLE s, int how)
{
	if( ::shutdown((SOCKET)s,how) == SOCKET_ERROR ){
		return -1;
	}
	return 0;
}

int OS::listen(AN_HANDLE s, int backlog)
{
	if( ::listen((SOCKET)s,backlog) == SOCKET_ERROR ){
		return -1;
	}
	return 0;
}

AN_HANDLE OS::accept(AN_HANDLE s, sockaddr *addr, int *addrlen)
{
	sockaddr  tmp_addr;
	int       tmp_addrlen = 0;
	AN_HANDLE  h = AN_INVALID_HANDLE;
	if( addr == NULL ){
		addr = &tmp_addr;
		addrlen = &tmp_addrlen;
	}
	h = (AN_HANDLE)::accept((SOCKET)s,addr,addrlen);
	if( h == (AN_HANDLE)INVALID_SOCKET ){
		return AN_INVALID_HANDLE;
	}
	return h;
}

int OS::connect(AN_HANDLE s, const sockaddr *serv_addr, int addrlen)
{
	int rc = -1;
	if( ::connect((SOCKET)s,serv_addr,addrlen) == SOCKET_ERROR ){
		if( ::WSAGetLastError() == WSAEWOULDBLOCK ){
			rc = 1;
		}
		return rc;
	}
	return 0;
}

#else // _LINUX_

CTimeValue OS::gettimeofday()
{
    timeval tv;
    int    rc = 0;

	rc = ::gettimeofday(&tv,NULL);

	return CTimeValue(tv);
}

int OS::mutex_init(AN_MUTEX *m)
{
	return pthread_mutex_init(m,NULL);
}

int OS::mutex_lock(AN_MUTEX *m)
{
	return pthread_mutex_lock(m);
}

int OS::mutex_trylock(AN_MUTEX *m)
{
	//return pthread_mutex_trylock(m);
	return -1;
}

int OS::mutex_unlock(AN_MUTEX *m)
{
	return pthread_mutex_unlock(m);
}

int OS::mutex_destroy(AN_MUTEX *m)
{
	return pthread_mutex_destroy(m);
}

int OS::ioctl(AN_HANDLE h, int cmd, void *arg)
{
	return ::ioctl(h,cmd,arg);
}

int OS::closesocket(AN_HANDLE s)
{
	return ::close(s);
}

int OS::setsockopt(AN_HANDLE s, int level, int optname, const char *optval, int optlen)
{
	return ::setsockopt(s,level,optname,optval,optlen);
}

int OS::getsockopt(AN_HANDLE s, int level, int optname, char *optval, int *optlen)
{
	return ::getsockopt(s,level,optname,optval,(socklen_t*)optlen);
}

int OS::getsockname(AN_HANDLE s, sockaddr *name, int *namelen)
{
	return ::getsockname(s,name,(socklen_t*)namelen);
}

int OS::getpeername(AN_HANDLE s,sockaddr *name,int *namelen)
{
	return ::getpeername(s,name,(socklen_t*)namelen);
}

int OS::recv(AN_HANDLE s, void *buf, size_t len, int flags, const CTimeValue *timeout)
{
	int rc = -1;
	if( timeout == 0 ){
		rc = ::recv(s,buf,len,flags);
		if( rc == -1 ){
			if( errno == EAGAIN ){
				rc = 1;
			}
		} 
	}
	return rc;
}

int OS::recv(AN_HANDLE s, iovec iov[], size_t n, const CTimeValue *timeout)
{
	if( timeout == 0 ){
		return ::readv(s,iov,n);
	} 
	return -1;
}

int OS::send(AN_HANDLE s,const void *buf,size_t len,int flags,const CTimeValue *timeout)
{
	int rc = -1;
	if( timeout == 0 ){
		rc = ::send(s,buf,len,flags);
		if( rc == -1 ){
			if( errno == EAGAIN ){
				rc = 1;
			}
		}
	}
	return rc;
}

int OS::send(AN_HANDLE s,iovec iov[],size_t n,const CTimeValue *timeout)
{
	if( timeout == 0 ){
		return ::writev(s,iov,n);
	}
	return -1;
}

int OS::bind(AN_HANDLE s, const sockaddr *addr, int addrlen)
{
	return ::bind(s,addr,(socklen_t)addrlen);
}

int OS::sendto(AN_HANDLE s, const void *msg, int len, int flags, const sockaddr *to, int tolen)
{
	return ::sendto(s,msg,len,flags,to,(socklen_t)tolen);
}

int OS::recvfrom(AN_HANDLE s,void *buf,int len,int flags,sockaddr *from,int *fromlen)
{
	return ::recvfrom(s,buf,len,flags,from,(socklen_t*)fromlen);
}

int OS::shutdown(AN_HANDLE s, int how)
{
	return ::shutdown(s,how);
}

int OS::listen(AN_HANDLE s, int backlog)
{
	return ::listen(s,backlog);
}

AN_HANDLE OS::accept(AN_HANDLE s, sockaddr *addr, int *addrlen)
{
	return ::accept(s,addr,(socklen_t*)addrlen);
}

int OS::connect(AN_HANDLE s,const sockaddr *serv_addr,int addrlen)
{
	int rc =  ::connect(s,serv_addr,(socklen_t)addrlen);
	if( rc == -1 ){
		if( (errno==EAGAIN) || (errno==EINPROGRESS) ){
			rc = 1;
		}
	}
	return rc;
}

#endif

AN_HANDLE OS::socket(int domain, int type, int protocol)
{
	return (AN_HANDLE)::socket(domain,type,protocol);
}

int OS::recv(AN_HANDLE s, void *buf, size_t len, const CTimeValue *timeout)
{
	return OS::recv(s,buf,len,0,timeout);
}

int OS::send(AN_HANDLE s, const void *buf, size_t len, const CTimeValue *timeout)
{
	return OS::send(s,buf,len,0,timeout);
}

/*
u_short OS::htons(u_short hostlong)
{
	return ::htons(hostlong);
}

u_short OS::ntohs(u_short netshort)
{
	return ::ntohs(netshort);
}

u_long OS::htonl(u_long hostlong)
{
	return ::htonl(hostlong);
}

u_long OS::ntohl(u_long netlong)
{
	return ::ntohl(netlong);
}
*/

uint64 OS::htonll(uint64 hostll)
{
	u_long  low , high;

	memcpy(&low,&hostll,sizeof(u_long));
	memcpy(&high,(&hostll) + sizeof(u_long),sizeof(u_long));

	low = htonl(low);
	high = htonl(low);

	memcpy(&hostll,&high,sizeof(u_long));
	memcpy((&hostll) + sizeof(u_long),&low,sizeof(u_long));

	return hostll;
}

uint64 OS::ntohll(uint64 netll)
{
	u_long  low , high;

	memcpy(&low,&netll,sizeof(u_long));
	memcpy(&high,(&netll) + sizeof(u_long),sizeof(u_long));

	low = htonl(low);
	high = htonl(low);

	memcpy(&netll,&high,sizeof(u_long));
	memcpy((&netll) + sizeof(u_long),&low,sizeof(u_long));

	return netll;
}





