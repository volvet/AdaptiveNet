//=============================================================================
/**
 *  @file    OS.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#ifndef _OS_H_
#define _OS_H_


#ifdef _WIN32_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

#else // _LINUX_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <stdarg.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <linux/unistd.h>
#include <linux/sysctl.h>
#include <sys/resource.h>
#include <sys/poll.h>

#endif

#ifdef _WIN32_
typedef HANDLE              AN_HANDLE;
typedef SOCKET              AN_SOCKET;
typedef CRITICAL_SECTION    AN_MUTEX;
typedef   __int64           int64;
typedef unsigned __int64    uint64;

#define AN_INVALID_HANDLE   INVALID_HANDLE_VALUE
#define AN_SD_RECEIVE       SD_RECEIVE
#define AN_SD_SEND          SD_SEND
#define AN_SD_BOTH          SD_BOTH

typedef struct _iovec
{
	size_t iov_len;
	char *iov_base;    
} iovec;



#else // _LINUX_
typedef int                 AN_HANDLE;
typedef int                 AN_SOCKET;
typedef pthread_mutex_t     AN_MUTEX;
typedef const char*         LPCSTR;
typedef  long long          int64;
typedef  unsigned long long  uint64;

#define AN_INVALID_HANDLE   (-1)
#define AN_SD_RECEIVE       SHUT_RD
#define AN_SD_SEND          SHUT_WR
#define AN_SD_BOTH          SHUT_RDWR



#endif

class CTimeValue;

class OS
{
public:
	static CTimeValue gettimeofday(void);
	static int        mutex_init(AN_MUTEX *m);
	static int        mutex_lock(AN_MUTEX *m);
	static int        mutex_trylock(AN_MUTEX *m);
	static int        mutex_unlock(AN_MUTEX *m);
	static int        mutex_destroy(AN_MUTEX *m);
	static int        ioctl(AN_HANDLE h,int cmd,void *arg);
	static AN_HANDLE  socket(int domain,int type,int protocol);
	static int        closesocket(AN_HANDLE s);
	static int        setsockopt(AN_HANDLE s,int level,int optname,const char *optval,int optlen);
	static int        getsockopt(AN_HANDLE s,int level,int optname,char *optval,int *optlen);
	static int        getsockname(AN_HANDLE s,sockaddr *name,int *namelen);
	static int        getpeername(AN_HANDLE s,sockaddr *name,int *namelen);
	static int        recv(AN_HANDLE s,void *buf,size_t len,int flags,const CTimeValue *timeout = 0);
	static int        recv(AN_HANDLE s,void *buf,size_t len,const CTimeValue *timeout = 0);
	static int        recv(AN_HANDLE s,iovec iov[],size_t n,const CTimeValue *timeout = 0);
	static int        send(AN_HANDLE s,const void *buf,size_t len,int flags,const CTimeValue *timeout = 0);
	static int        send(AN_HANDLE s,const void *buf,size_t len,const CTimeValue *timeout = 0);
	static int        send(AN_HANDLE s,iovec iov[],size_t n,const CTimeValue *timeout = 0);
	static int        bind(AN_HANDLE s,const sockaddr *addr,int addrlen);
	static int        sendto(AN_HANDLE s,const void *msg,int len,int flags,const sockaddr* to ,int tolen);
	static int        recvfrom(AN_HANDLE s,void *buf,int len,int flags,sockaddr *from,int *fromlen);
	static int        shutdown(AN_HANDLE s,int how);
	static int        listen(AN_HANDLE s,int backlog);
	static AN_HANDLE  accept(AN_HANDLE s,sockaddr *addr,int *addrlen);
	static int        connect(AN_HANDLE s,const sockaddr *serv_addr,int addrlen);
	static uint64     htonll(uint64 hostll);
	static uint64     ntohll(uint64 netll);
	//static u_long     htonl(u_long hostlong);
	//static u_long     ntohl(u_long netlong);
	//static u_short    htons(u_short hostshort);
	//static u_short    ntohs(u_short netshort);
};



#endif




