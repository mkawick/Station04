#pragma once

#include <Application/Task.h>
#include <Common/CircularBuffer.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/any.hpp>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{

// --------------------------------------------------------------------------------------------------------------------
class Service
{
public:
	enum ExecutionState
	{
		Uninitialized = -1,
		Startup,
		Queued,
		Running,
		// ShutdownPending,
		Shutdown,
		Stopped,
	};
	ExecutionState			State() const;
	bool					HasStopped() const;
	void					Stop(bool block = false);
	//bool					Wait(float timeout = infinity);
	//bool					Wait(ExecutionState state/*, float timeout = infinity*/);

	template<typename T>
	T*						Provider();
	template<typename Fn>
	bool					Post(Fn fn);
	bool					Post(Task::Fn& fn);
	bool					Post(shared_task task);

	bool					operator==(const Service& rhs);

	~Service() {}

	// Service API
	static size_t			Count();
	static shared_service	At(unsigned int i);
	static shared_service	Active();
private:
	friend class Application;
	typedef std::shared_ptr<boost::any> shared_provider;
	typedef std::weak_ptr<boost::any> weak_provider;

							Service();

	static shared_service	Create();
	template<typename Fn>
	void					MakeProvider();
	template<typename Fn, typename A0>
	void					MakeProvider(A0& a0);
	template<typename Fn, typename A0, typename A1>
	void					MakeProvider(A0& a0, A1& a1);
	template<typename Fn, typename A0, typename A1, typename A2>
	void					MakeProvider(A0& a0, A1& a1, A2& a2);
	template<typename Fn, typename A0, typename A1, typename A2, typename A3>
	void					MakeProvider(A0& a0, A1& a1, A2& a2, A3& a3);
	template<typename Fn, typename A0, typename A1, typename A2, typename A3, typename A4>
	void					MakeProvider(A0& a0, A1& a1, A2& a2, A3& a3, A4& a4);
	template<typename Fn, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
	void					MakeProvider(A0& a0, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5);

	typedef CircularBuffer<shared_task, 128> TaskQueue;
	typedef boost::mutex					Mutex;
	typedef boost::unique_lock<Mutex>		MutexLock;
	typedef boost::condition_variable		Condition;

	TaskQueue				mTaskQueue;

	atomic<ExecutionState>	mState;
	mutable Mutex			mStateMutex;
	mutable Condition		mStateChanged;

	boost::any				mProvider;
	weak_service			mSelf;
};

// --------------------------------------------------------------------------------------------------------------------
inline bool Service::operator==(const Service& rhs)
{
	return this == &rhs;
}

// --------------------------------------------------------------------------------------------------------------------
inline bool	Service::HasStopped() const
{
	return Stopped == mState.get();
}

//// --------------------------------------------------------------------------------------------------------------------
//inline void	Service::SetState(ExecutionState state)
//{
//	mState = state;
//	mStateChanged.notify_all();
//}

// --------------------------------------------------------------------------------------------------------------------
template<typename T>
T* Service::Provider()
{
	return boost::any_cast< std::shared_ptr<T> >(mProvider).get();
}

// --------------------------------------------------------------------------------------------------------------------
template<typename Fn> 
bool Service::Post(Fn fn)
{
	shared_service service = mSelf.lock();
	shared_task task(new Task(fn, service));
	return Post(task);
}

// --------------------------------------------------------------------------------------------------------------------
template<typename Fn> 
void Service::MakeProvider()
{
	mProvider = std::shared_ptr<Fn>(new Fn());
}

// --------------------------------------------------------------------------------------------------------------------
template<typename Fn, typename A0> 
void Service::MakeProvider(A0& a0)
{
	mProvider = std::shared_ptr<Fn>(new Fn(a0));
}

// --------------------------------------------------------------------------------------------------------------------
template<typename Fn, typename A0, typename A1> 
void Service::MakeProvider(A0& a0, A1& a1)
{
	mProvider = std::shared_ptr<Fn>(new Fn(a0, a1));
}

// --------------------------------------------------------------------------------------------------------------------
template<typename Fn, typename A0, typename A1, typename A2> 
void Service::MakeProvider(A0& a0, A1& a1, A2& a2)
{
	mProvider = std::shared_ptr<Fn>(new Fn(a0, a1, a2));
}

// --------------------------------------------------------------------------------------------------------------------
template<typename Fn, typename A0, typename A1, typename A2, typename A3> 
void Service::MakeProvider(A0& a0, A1& a1, A2& a2, A3& a3)
{
	mProvider = std::shared_ptr<Fn>(new Fn(a0, a1, a2, a3));
}

// --------------------------------------------------------------------------------------------------------------------
template<typename Fn, typename A0, typename A1, typename A2, typename A3, typename A4> 
void Service::MakeProvider(A0& a0, A1& a1, A2& a2, A3& a3, A4& a4)
{
	mProvider = std::shared_ptr<Fn>(new Fn(a0, a1, a2, a3, a4));
}

// --------------------------------------------------------------------------------------------------------------------
template<typename Fn, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5> 
void Service::MakeProvider(A0& a0, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5)
{
	mProvider = std::shared_ptr<Fn>(new Fn(a0, a1, a2, a3, a4, a5));
}

// --------------------------------------------------------------------------------------------------------------------
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
