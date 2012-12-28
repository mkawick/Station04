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
	Post(std::bind<void>(&Service::AllStop, mSelf.lock()));
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
void Service::AllStop()
{
	Application* application = Application::Get();
	Application::SharedLock lock(application->mServiceMutex);

	shared_service self = mSelf.lock();
	Application::ServiceList::iterator item = application->mServices.begin();
	while(item->lock() != self && item != application->mServices.end())
	{
		++item;
	}
	if (item != application->mServices.end())
	{	
		item->reset(); // Item is no longer a candidate
	}
	mState = Stopped;
}

// --------------------------------------------------------------------------------------------------------------------
shared_service Service::Active()
{
	return *Application::Get()->mActiveService;
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
