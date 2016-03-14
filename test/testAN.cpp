//=============================================================================
/**
 *  @file    testAN.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#include <stdio.h>
#include "InetAddr.h"
#include "EventHandler.h"
#include "TimerQueue.h"
#include "Reactor.h"
#include "ClientDgrm.h"
#include "Timer.h"
#include "ByteStream.h"

int test_InetAddr()
{
	char strAddr[] = "192.168.0.21:34";
	char strAddr_1[] = "192.168.1.21:34";
	CInetAddr  aAddr(strAddr);
	CInetAddr  aAddr_1(strAddr_1);

	printf("Test InetAddr Class\n");
	printf("Set address is %s\n",strAddr);
	printf("Actual ip is %s,port is %d\n",aAddr.get_host_addr(),aAddr.get_port());
	printf("Set address is %s\n",strAddr_1);
	printf("Actual ip is %s,port is %d\n",aAddr_1.get_host_addr(),aAddr_1.get_port());
	printf(aAddr == aAddr_1 ? "Equal\n" : "Not equal\n");
	
	return 1;
}

int test_ByteStream()
{
    short s = 3;
	int l = 2;
	int64 ll = 5;

	short s_r;
	int   l_r;
	int64 ll_r;

	unsigned char buf[64];

	printf("Test CByteStream Class\n");

	CByteStream  stream(buf,64);

	stream << s;
	stream << l;
	stream << ll;

	CByteStream stream_r(buf,64);

	stream_r >> s_r;
	stream_r >> l_r;
	stream_r >> ll_r;

	if( (s==s_r) && (l==l_r) && (ll==ll_r) ){
		printf("Test OK\n");
	} else {
		printf("Test failed\n");
	}
	return 0;
}

int test_TimerQueue()
{
	class MyTimerSink : public ITimerSink
	{
	public:
		MyTimerSink() { count = 0; } 
		virtual int on_timer(void *arg,ITimer *timer)
		{
			printf("cur count is %d\n",count ++ );

			if( count > 50 ){
				CReactor::instance()->end_event_loop();
			}
			return 0;
		}
	private:
		int count;
	};

	MyTimerSink  sink;
	CNetTimer *timer = new CNetTimer(&sink,CReactor::instance());

	timer->schedule(500,NULL);
	
	CReactor::instance()->run_event_loop();	
	
	return 1;
}

int test_Reactor()
{
	printf("Test Reactor Class\n");   
	return 0;
}

int test_ClientDgrm()
{
	printf("Test ClientDgrm Class");

	class CClientSink : public IClientDgrmSink
	{
	public:
		virtual int handle_input(CDataBlock &,CInetAddr &)
		{
			return 0;
		}
		virtual int handle_close(int errcode)
		{
			return 0;
		}
	};

	CClientSink  sink;
	CClientDgrm  client(&sink,CReactor::instance());
	CInetAddr   remote("192.168.0.1:2000");

	if( client.connect(remote) != 0 ){
		printf(" connect failed !!! \n");
	}


	return 0;
}




int main()
{
	printf("\n-----------Adaptive Network Module Test----------\n");
	printf("----------------Version 0.01----------------------\n\n");


	CReactor::instance()->open();

	test_InetAddr();
	printf("\n\n");
    test_Reactor();
	printf("\n\n");
	test_TimerQueue();
	printf("\n\n");
	test_ClientDgrm();
	printf("\n\n");
	test_ByteStream();
	printf("\n\n");


	CReactor::instance()->close();
	CReactor::close_singleton();


	printf("\n-----------Test Complete-------------------------\n");	
	return 0;
}




