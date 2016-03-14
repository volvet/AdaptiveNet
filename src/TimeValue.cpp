//=============================================================================
/**
 *  @file    TimeValue.cpp
 *    
 *  @author ZhangQi <volvet2002@gmail.com>
 *  
 */
//=============================================================================


#include "TimeValue.h"



const CTimeValue CTimeValue::max_time(LONG_MAX,ONE_SECOND_IN_USEC-1);
const CTimeValue CTimeValue::zero_time(0,0);


CTimeValue::CTimeValue()
{
	this->set(0,0);
}

CTimeValue::CTimeValue(long s,long us)
{
	this->set(s,us);
}

CTimeValue::CTimeValue(const timeval &t)
{
	this->set(t);
	normalize();
}

CTimeValue::CTimeValue(const CTimeValue &t) :
	_tv(t._tv)
{
}

CTimeValue::~CTimeValue()
{
}


void CTimeValue::set(long s,long us)
{
	this->_tv.tv_sec = s;
	this->_tv.tv_usec = us;
}

void CTimeValue::set(const timeval &t)
{
	this->_tv.tv_sec = t.tv_sec;
	this->_tv.tv_usec = t.tv_usec;
	normalize();
}

void CTimeValue::normalize()
{
	if( this->_tv.tv_usec > ONE_SECOND_IN_USEC ){
		do {
			this->_tv.tv_sec ++;
			this->_tv.tv_usec -= ONE_SECOND_IN_USEC;
		} while( _tv.tv_usec > ONE_SECOND_IN_USEC );
	}
	if( this->_tv.tv_usec <= ONE_SECOND_IN_USEC ){
		do {
			this->_tv.tv_sec --;
			this->_tv.tv_usec += ONE_SECOND_IN_USEC;
		} while( _tv.tv_usec <= ONE_SECOND_IN_USEC );
	}
	if( this->_tv.tv_sec >= 1 && this->_tv.tv_usec < 0 ){
		this->_tv.tv_sec --;
		this->_tv.tv_usec += ONE_SECOND_IN_USEC;
	}
	if( this->_tv.tv_sec < 0 && this->_tv.tv_usec > 0 ){
		this->_tv.tv_sec ++;
		this->_tv.tv_usec -= ONE_SECOND_IN_USEC;
	}
}

long CTimeValue::msec() const
{
	return this->_tv.tv_sec * 1000 + this->_tv.tv_usec / 1000;
}

void CTimeValue::msec(long ms)
{
	this->_tv.tv_sec = ms/1000;
	this->_tv.tv_usec = (ms - this->_tv.tv_sec * 1000) * 1000;
}

long CTimeValue::sec() const
{
	return this->_tv.tv_sec;
}

void CTimeValue::sec(long s)
{
	this->_tv.tv_sec = s;
}

long CTimeValue::usec() const
{
	return this->_tv.tv_usec;
}

void CTimeValue::usec(long us)
{
	this->_tv.tv_usec = us;
}

CTimeValue::operator timeval() const
{
	return this->_tv;
}

CTimeValue::operator const timeval * () const
{
	return &this->_tv;
}

void CTimeValue::operator += (const CTimeValue &t)
{		
	this->sec( this->sec() + t.sec() );
	this->usec( this->usec() + t.usec() );
	normalize();
}

void CTimeValue::operator -= (const CTimeValue &t)
{
	this->sec( this->sec() - t.sec() );
	this->usec( this->usec() - t.usec() );
	normalize();
}

CTimeValue operator + (const CTimeValue &left,const CTimeValue &right)
{
	CTimeValue t;

	t.sec(left.sec() + right.sec());
	t.usec(left.usec() + right.usec());
	t.normalize();
	return t;
}

CTimeValue operator - (const CTimeValue &left,const CTimeValue &right)
{
	CTimeValue t;

	t.sec(left.sec() - right.sec());
	t.usec(left.usec() - right.usec());
	t.normalize();
	return t;
}

int operator > (const CTimeValue &left,const CTimeValue &right)
{
	if( left.sec() > right.sec() ){
		return 1;
	} else if( left.sec() == right.sec() && left.usec() > right.usec() ){
		return 1;
	} else {
		return 0;
	}
}

int operator < (const CTimeValue &left,const CTimeValue &right)
{
	return right > left;
}

int operator >= (const CTimeValue &left,const CTimeValue &right)
{
	if( left.sec() > right.sec() ){
		return 1;
	} else if( left.sec() == right.sec() && left.usec() >= right.usec() ){
		return 1;
	} else {
		return 0;
	}
}

int operator <= (const CTimeValue &left,const CTimeValue &right)
{
	return right >= left;
}

int operator == (const CTimeValue &left,const CTimeValue &right)
{
	if( left.sec() == right.sec() && left.usec() == right.usec() ){
		return 1;
	} else {
		return 0;
	}
}

int operator != (const CTimeValue &left,const CTimeValue &right)
{
	return !(left == right);
}




