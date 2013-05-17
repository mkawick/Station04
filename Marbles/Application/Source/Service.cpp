#include <Application\Service.h>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{

// --------------------------------------------------------------------------------------------------------------------
Service::Service()
: mState(Service::Uninitialized)
{
}

// --------------------------------------------------------------------------------------------------------------------
Service::ExecutionState Service::State() const
{
	return mState.get();
}

// --------------------------------------------------------------------------------------------------------------------
void Service::Stop(bool /*block*/)
{
	Post(std::bind<void>(&Application::Unregister, Application::Get(), mSelf.lock()));
	//if (block)
	//{
	//	Wait(Stopped);
	//}
}

// --------------------------------------------------------------------------------------------------------------------
//bool Service::Wait(ExecutionState state)
//{
//	const bool isSelf = Active().get() == this;
//	ASSERT(!isSelf); // We cannot wait for ourself!
//	if (!isSelf)
//	{
//		MutexLock lock(mStateMutex);
//		while (state > mState.get())
//		{
//			mStateChanged.wait(lock);
//		}
//	}
//	return !isSelf;
//}

// --------------------------------------------------------------------------------------------------------------------
bool Service::Post(Task::Fn& fn)
{	
	shared_service service = mSelf.lock();
	shared_task task(new Task(fn, service));
	return Post(task);
}

// --------------------------------------------------------------------------------------------------------------------
bool Service::Post(shared_task task)
{
	return mTaskQueue.try_push(task);
}

// --------------------------------------------------------------------------------------------------------------------
shared_service Service::Active()
{
	return Application::Get()->ActiveService();
}

// --------------------------------------------------------------------------------------------------------------------
shared_service Service::Create()
{
	shared_service service(new Service());
	service->mSelf = service;
	return service;
}

// --------------------------------------------------------------------------------------------------------------------
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
