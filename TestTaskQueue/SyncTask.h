// SyncThread.h: interface for the CServiceThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNCTHREAD_H__A5982A45_8B2E_4CA1_9CC2_C1CF58555311__INCLUDED_)
#define AFX_SYNCTHREAD_H__A5982A45_8B2E_4CA1_9CC2_C1CF58555311__INCLUDED_

#include <WinBase.h>
#include <list>
#include <assert.h>
#include "AutoLock.h"
#include <memory>
using namespace std;
using namespace std::tr1;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBaseTask
{
public:
	CBaseTask() {}
	virtual ~CBaseTask(){}
	virtual void operator()(){};
};

typedef shared_ptr<CBaseTask> CBaseTaskPtr;

template <class _R, class _Ty, class _Fn>
class CTask0 : public CBaseTask
{
public:
	CTask0(_Fn f, _Ty *pTypeObj)
		: pFunction(f), pObject(pTypeObj) {}

	virtual void operator()()
	{
		if (pObject)
			Result = (pObject->*pFunction)()
		else
			assert(false);
	}

private:
	_Ty *pObject;
	_Fn pFunction;
	_R Result;
};

template <class _R, class _Ty, class _Fn>
_R make_function0(_Fn pFucntion, _Ty *pObject)
{
	return (pObject->*pFucntion)();
};


template <class _Ty, class _Fn>
class CSyncTask : public CBaseTask
{
public:
	CSyncTask(_Ty *pTypeObj,_Fn f)
		: pFunction(f), pObject(pTypeObj) {}

	virtual void operator()()
	{ 
		if (pObject)
			(pObject->*pFunction)(); 
		else
			assert(false);
	}
	~CSyncTask()
	{
		int a = 0;
	}
private:
	_Ty *pObject;
	_Fn pFunction;
};

template <class _Ty, class _Fn, class _A1>
class CSyncTask1 : public CBaseTask
{
public:
	CSyncTask1(_Ty *pTypeObj,_Fn f,  const _A1& _a1)
		: pFunction(f), pObject(pTypeObj), P1(_a1) {}

	virtual void operator()()
	{ 
		if (pObject)
			(pObject->*pFunction)(P1);
		else
			assert(false);
	}

private:
	_Ty *pObject;
	_Fn pFunction;
	_A1 P1;
};

template <class _Ty, class _Fn, class _A1, class _A2>
class CSyncTask2 : public CBaseTask
{
public:
	CSyncTask2( _Ty *pTypeObj,_Fn f, const _A1& _a1, const _A2& _a2)
		: pFunction(f), pObject(pTypeObj), P1(_a1), P2(_a2) {}

	virtual void operator()()
	{ 
		if (pObject)
			(pObject->*pFunction)(P1, P2);
		else
			assert(false);
	}

private:
	_Ty *pObject;
	_Fn pFunction;
	_A1 P1;
	_A2 P2;
};

template <class _Ty, class _Fn, class _A1, class _A2, class _A3>
class CSyncTask3 : public CBaseTask
{
public:
	CSyncTask3( _Ty *pTypeObj, _Fn f,const _A1& _a1, const _A2& _a2, const _A3& _a3)
		: pFunction(f), pObject(pTypeObj), P1(_a1), P2(_a2), P3(_a3) {}

	virtual void operator()()
	{ 
		if (pObject)
			(pObject->*pFunction)(P1, P2, P3); 
		else
			assert(false);
	}

private:
	_Ty *pObject;
	_Fn pFunction;
	_A1 P1;
	_A2 P2;
	_A3 P3;
};

template <class _Ty, class _Fn, class _A1, class _A2, class _A3, class _A4>
class CSyncTask4 : public CBaseTask
{
public:
	CSyncTask4( _Ty *pTypeObj,_Fn f, const _A1& _a1, const _A2& _a2, const _A3& _a3, const _A4& _a4)
		: pFunction(f), pObject(pTypeObj), P1(_a1), P2(_a2), P3(_a3), P4(_a4) {}

	virtual void operator()()
	{
		if (pObject)
			(pObject->*pFunction)(P1, P2, P3, P4);
		else
			assert(false);
	}

private:
	_Ty *pObject;
	_Fn pFunction;
	_A1 P1;
	_A2 P2;
	_A3 P3;
	_A4 P4;
};

template <class _Ty, class _Fn, class _A1, class _A2, class _A3, class _A4, class _A5>
class CSyncTask5 : public CBaseTask
{
public:
	CSyncTask5( _Ty *pTypeObj, _Fn f,const _A1& _a1, const _A2& _a2, const _A3& _a3, const _A4& _a4, const _A5& _a5)
		: pFunction(f), pObject(pTypeObj), P1(_a1), P2(_a2), P3(_a3), P4(_a4), P5(_a5) {}

	virtual void operator()()
	{
		if (pObject)
			(pObject->*pFunction)(P1, P2, P3, P4, P5);
		else
			assert(false);
	}

private:
	_Ty *pObject;
	_Fn pFunction;
	_A1 P1;
	_A2 P2;
	_A3 P3;
	_A4 P4;
	_A5 P5;
};


template <class _Ty, class _Fn, class _A1, class _A2, class _A3, class _A4, class _A5, class _A6>
class CSyncTask6 : public CBaseTask
{
public:
	CSyncTask6( _Ty *pTypeObj,_Fn f, const _A1& _a1, const _A2& _a2, const _A3& _a3, const _A4& _a4, const _A5& _a5, const _A6& _a6)
		: pFunction(f), pObject(pTypeObj), P1(_a1), P2(_a2), P3(_a3), P4(_a4), P5(_a5), P6(_a6){}

	virtual void operator()()
	{
		if (pObject)
			(pObject->*pFunction)(P1, P2, P3, P4, P5, P6);
		else
			assert(false);
	}

private:
	_Ty *pObject;
	_Fn pFunction;
	_A1 P1;
	_A2 P2;
	_A3 P3;
	_A4 P4;
	_A5 P5;
	_A6 P6;
};

template <class _Ty, class _Fn, class _A1, class _A2, class _A3, class _A4, class _A5, class _A6, class _A7>
class CSyncTask7 : public CBaseTask
{
public:
	CSyncTask7( _Ty *pTypeObj,_Fn f, const _A1& _a1, const _A2& _a2, const _A3& _a3, const _A4& _a4, const _A5& _a5, const _A6& _a6)
		: pFunction(f), pObject(pTypeObj), P1(_a1), P2(_a2), P3(_a3), P4(_a4), P5(_a5), P6(_a6), P7(_a7){}

	virtual void operator()()
	{
		if (pObject)
			(pObject->*pFunction)(P1, P2, P3, P4, P5, P6, P7);
		else
			assert(false);
	}
private:
	_Ty *pObject;
	_Fn pFunction;
	_A1 P1;
	_A2 P2;
	_A3 P3;
	_A4 P4;
	_A5 P5;
	_A6 P6;
	_A7 P7;
};

template <class _Ty, class _Fn, class _A1, class _A2, class _A3, class _A4, class _A5, class _A6, class _A7, class _A8>
class CSyncTask8 : public CBaseTask
{
public:
	CSyncTask8( _Ty *pTypeObj,_Fn f, const _A1& _a1, const _A2& _a2, const _A3& _a3, const _A4& _a4, const _A5& _a5, const _A6& _a6, const _A7& _a7, const _A8& _a8)
		: pFunction(f), pObject(pTypeObj), P1(_a1), P2(_a2), P3(_a3), P4(_a4), P5(_a5), P6(_a6), P7(_a7), P8(_a8){}

	virtual void operator()()
	{
		if (pObject)
			(pObject->*pFunction)(P1, P2, P3, P4, P5, P6, P7, P8);
		else
			assert(false);
	}
private:
	_Ty *pObject;
	_Fn pFunction;
	_A1 P1;
	_A2 P2;
	_A3 P3;
	_A4 P4;
	_A5 P5;
	_A6 P6;
	_A7 P7;
	_A8 P8;
};

template <class _Ty, class _Fn, class _A1, class _A2, class _A3, class _A4, class _A5, class _A6, class _A7, class _A8, class _A9>
class CSyncTask9 : public CBaseTask
{
public:
	CSyncTask9( _Ty *pTypeObj,_Fn f, const _A1& _a1, const _A2& _a2, const _A3& _a3, const _A4& _a4, const _A5& _a5, const _A6& _a6, const _A7& _a7, const _A8& _a8, const _A9& _a9)
		: pFunction(f), pObject(pTypeObj), P1(_a1), P2(_a2), P3(_a3), P4(_a4), P5(_a5), P6(_a6), P7(_a7), P8(_a8), P9(_a9){}

	virtual void operator()()
	{
		if (pObject)
			(pObject->*pFunction)(P1, P2, P3, P4, P5, P6, P7, P8, P9);
		else
			assert(false);
	}
private:
	_Ty *pObject;
	_Fn pFunction;
	_A1 P1;
	_A2 P2;
	_A3 P3;
	_A4 P4;
	_A5 P5;
	_A6 P6;
	_A7 P7;
	_A8 P8;
	_A9 P9;
};


class CSyncTaskQueue
{

public:
	CSyncTaskQueue()
	{
		InitializeCriticalSection(&m_csTask);
	}
	virtual ~CSyncTaskQueue()
	{
		DeleteCriticalSection(&m_csTask);
	}
	
	size_t TaskCount()
	{
		CAutoLock Lock(&m_csTask);
		return m_listTask.size(); 
	}

	template<class _Ty, class _R>
	void PutTask(_Ty* pTypeObj,_R (_Ty::*MemFunction)())
	{
		typedef _R(_Ty::*_Fn)();

		CSyncTask<_Ty, _Fn> *pTask = new CSyncTask<_Ty, _Fn>(pTypeObj,MemFunction);
		EnterCriticalSection(&m_csTask);
		m_listTask.push_back(shared_ptr<CBaseTask>((CBaseTask *)pTask));
		LeaveCriticalSection(&m_csTask);
	}

	template<class _Ty, class _R, class _A1>
	void PutTask(_Ty* pTypeObj,_R (_Ty::*MemFunction)(_A1),  const _A1& _a1)
	{
		typedef _R(_Ty::*_Fn)(_A1);

		CSyncTask1<_Ty, _Fn, _A1> *pTask = new CSyncTask1<_Ty, _Fn, _A1>(pTypeObj, MemFunction, _a1);
		EnterCriticalSection(&m_csTask);
		m_listTask.push_back(shared_ptr<CBaseTask>((CBaseTask *)pTask));;
		LeaveCriticalSection(&m_csTask);
	}

	template<class _Ty, class _R, class _A1, class _A2>
	void PutTask(_Ty* pTypeObj,_R (_Ty::*MemFunction)(_A1, _A2),  const _A1& _a1, const _A2& _a2)
	{
		typedef _R(_Ty::*_Fn)(_A1, _A2);

		CSyncTask2<_Ty, _Fn, _A1, _A2> *pTask = new CSyncTask2<_Ty, _Fn, _A1, _A2>(pTypeObj,MemFunction, _a1, _a2);
		EnterCriticalSection(&m_csTask);
		m_listTask.push_back(shared_ptr<CBaseTask>((CBaseTask *)pTask));;
		LeaveCriticalSection(&m_csTask);
	}

	template<class _Ty, class _R, class _A1, class _A2, class _A3>
	void PutTask(_Ty* pTypeObj,_R (_Ty::*MemFunction)(_A1, _A2, _A3),  const _A1& _a1, const _A2& _a2, const _A3& _a3)
	{
		typedef _R(_Ty::*_Fn)(_A1, _A2, _A3);

		CSyncTask3<_Ty, _Fn, _A1, _A2, _A3> *pTask = new CSyncTask3<_Ty, _Fn, _A1, _A2, _A3>(pTypeObj,MemFunction, _a1, _a2, _a3);
		EnterCriticalSection(&m_csTask);
		m_listTask.push_back(shared_ptr<CBaseTask>((CBaseTask *)pTask));;
		LeaveCriticalSection(&m_csTask);
	}

	template<class _Ty, class _R, class _A1, class _A2, class _A3, class _A4>
	void push_backTask(_Ty* pTypeObj,_R (_Ty::*MemFunction)(_A1, _A2, _A3, _A4),  const _A1& _a1, const _A2& _a2, const _A3& _a3, const _A4& _a4)
	{
		typedef _R(_Ty::*_Fn)(_A1, _A2, _A3, _A4);

		CSyncTask4<_Ty, _Fn, _A1, _A2, _A3, _A4> *pTask = new CSyncTask4<_Ty, _Fn, _A1, _A2, _A3, _A4>(pTypeObj,MemFunction, _a1, _a2, _a3, _a4);
		EnterCriticalSection(&m_csTask);
		m_listTask.push_back(shared_ptr<CBaseTask>((CBaseTask *)pTask));;
		LeaveCriticalSection(&m_csTask);
	}

	template<class _Ty, class _R, class _A1, class _A2, class _A3, class _A4, class _A5>
	void PutTask(_Ty* pTypeObj,_R(_Ty::*MemFunction)(_A1, _A2, _A3, _A4,_A5),  const _A1& _a1, const _A2& _a2, const _A3& _a3, const _A4& _a4, const _A4& _a5)
	{
		typedef _R(_Ty::*_Fn)(_A1, _A2, _A3, _A4,_A5);

		CSyncTask5<_Ty, _Fn, _A1, _A2, _A3, _A4,_A5> *pTask = new CSyncTask5<_Ty, _Fn, _A1, _A2, _A3, _A4,_A5>(pTypeObj,MemFunction, _a1, _a2, _a3, _a4,_a5);
		EnterCriticalSection(&m_csTask);
		m_listTask.push_back(shared_ptr<CBaseTask>((CBaseTask *)pTask));;
		LeaveCriticalSection(&m_csTask);
	}
	void Clear()
	{
		EnterCriticalSection(&m_csTask);
		for (auto it = m_listTask.begin(); it != m_listTask.end();)
			it = m_listTask.erase(it);
		LeaveCriticalSection(&m_csTask);
	}

	CBaseTaskPtr  GetTask()
	{
		CBaseTaskPtr pTask = nullptr;
		EnterCriticalSection(&m_csTask);
		if (m_listTask.size() > 0)
		{
			pTask = m_listTask.front();
			m_listTask.pop_front();
		}
		LeaveCriticalSection(&m_csTask);
		return pTask;
	}
protected:
	list<CBaseTaskPtr> m_listTask;
	CRITICAL_SECTION  m_csTask;
	

};


#endif // !defined(AFX_SYNCTHREAD_H__A5982A45_8B2E_4CA1_9CC2_C1CF58555311__INCLUDED_)