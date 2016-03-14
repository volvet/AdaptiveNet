#
#    Adaptive Network Module Test 
#    Author :   ZhangQi
#


TOPDIR:=$(shell pwd)
TESTSRCDIR:=../
INCLUDEDIR:=../../inc/
SRCDIR:=../../src/
CFLAGS:=-O3 -Wall -pipe -D_LINUX_ -I$(INCLUDEDIR)
CC:=gcc
CXX:=g++
AR:=ar

OBJS:= \
	testAN.o TimeValue.o EventHandler.o TimerQueue.o \
	Reactor.o OS.o IpcSap.o Addr.o InetAddr.o Sock.o \
	SockIO.o TimerList.o ReactorImpl.o SockDgrm.o \
	SockStream.o SockAcceptor.o SockConnector.o DataBlock.o \
	Timer.o ByteStream.o ClientDgrm.o TransPdu.o \
	TransAcceptor.o TransAcceptorDgrm.o TransConnectionDgrm.o

testAN : $(OBJS)
	$(CXX) $(CFLAGS) -o testAN $(OBJS)

clean:
	@rm -f $(OBJS)	

testAN.o : $(TESTSRCDIR)testAN.cpp
	$(CXX) $(CFLAGS) -c $(TESTSRCDIR)testAN.cpp

TimeValue.o : $(SRCDIR)TimeValue.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)TimeValue.cpp

EventHandler.o : $(SRCDIR)EventHandler.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)EventHandler.cpp

TimerQueue.o : $(SRCDIR)TimerQueue.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)TimerQueue.cpp

Reactor.o : $(SRCDIR)Reactor.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)Reactor.cpp

OS.o : $(SRCDIR)OS.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)OS.cpp

IpcSap.o : $(SRCDIR)IpcSap.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)IpcSap.cpp
	
Addr.o : $(SRCDIR)Addr.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)Addr.cpp

InetAddr.o : $(SRCDIR)InetAddr.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)InetAddr.cpp
	
Sock.o : $(SRCDIR)Sock.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)Sock.cpp

SockIO.o : $(SRCDIR)SockIO.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)SockIO.cpp

TimerList.o : $(SRCDIR)TimerList.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)TimerList.cpp

ReactorImpl.o : $(SRCDIR)ReactorImpl.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)ReactorImpl.cpp

SockDgrm.o : $(SRCDIR)SockDgrm.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)SockDgrm.cpp

SockStream.o : $(SRCDIR)SockStream.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)SockStream.cpp

SockAcceptor.o : $(SRCDIR)SockAcceptor.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)SockAcceptor.cpp

SockConnector.o : $(SRCDIR)SockConnector.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)SockConnector.cpp

DataBlock.o : $(SRCDIR)DataBlock.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)DataBlock.cpp

Timer.o : $(SRCDIR)Timer.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)Timer.cpp

ByteStream.o : $(SRCDIR)ByteStream.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)ByteStream.cpp

ClientDgrm.o : $(SRCDIR)ClientDgrm.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)ClientDgrm.cpp

TransPdu.o : $(SRCDIR)TransPdu.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)TransPdu.cpp

TransAcceptor.o : $(SRCDIR)TransAcceptor.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)TransAcceptor.cpp

TransAcceptorDgrm.o : $(SRCDIR)TransAcceptorDgrm.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)TransAcceptorDgrm.cpp

TransConnectionDgrm.o : $(SRCDIR)TransConnectionDgrm.cpp
	$(CXX) $(CFLAGS) -c $(SRCDIR)TransConnectionDgrm.cpp
	
