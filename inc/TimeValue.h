//=============================================================================
/**
 *  @file    TimeValue.h
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#ifndef _TIMEVALUE_H_
#define _TIMEVALUE_H_


#include "OS.h"

#define   ONE_SECOND_IN_USEC   1000000

class CTimeValue;

CTimeValue operator + (const CTimeValue &left,const CTimeValue &right);
CTimeValue operator - (const CTimeValue &left,const CTimeValue &right);
int operator > (const CTimeValue &left,const CTimeValue &right);
int operator < (const CTimeValue &left,const CTimeValue &right);
int operator >= (const CTimeValue &left,const CTimeValue &right);
int operator <= (const CTimeValue &left,const CTimeValue &right);
int operator == (const CTimeValue &left,const CTimeValue &right);
int operator != (const CTimeValue &left,const CTimeValue &right);

class CTimeValue
{
public:
	CTimeValue(void);
	CTimeValue(long s,long us = 0);
	CTimeValue(const struct timeval &t);
	CTimeValue(const CTimeValue &t);
	virtual ~CTimeValue();

	void   set(long s,long us = 0);
	void   set(const struct timeval &t);
	long   msec(void) const;
	void   msec(long ms);
	long   sec(void) const;
	void   sec(long s);
	long   usec(void) const;
	void   usec(long us);

	operator timeval() const;
	operator const timeval * () const;
	void operator += (const CTimeValue &t);
	void operator -= (const CTimeValue &t);

	friend CTimeValue operator + (const CTimeValue &left,const CTimeValue &right);
	friend CTimeValue operator - (const CTimeValue &left,const CTimeValue &right);
	friend int operator > (const CTimeValue &left,const CTimeValue &right);
    friend int operator < (const CTimeValue &left,const CTimeValue &right);
    friend int operator >= (const CTimeValue &left,const CTimeValue &right);
    friend int operator <= (const CTimeValue &left,const CTimeValue &right);
    friend int operator == (const CTimeValue &left,const CTimeValue &right);
    friend int operator != (const CTimeValue &left,const CTimeValue &right);

	static const CTimeValue  zero_time;
	static const CTimeValue  max_time;

protected:
	void   normalize();

private:
	struct timeval _tv;
};



#endif



