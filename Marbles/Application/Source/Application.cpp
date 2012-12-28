// --------------------------------------------------------------------------------------------------------------------
#include <Application\Application.h>
#include <Application\Service.h>
#include <boost\any.hpp>
#include <boost\thread.hpp>
#include <boost\interprocess\detail\atomic.hpp>

// --------------------------------------------------------------------------------------------------------------------
namespace boost
{
	using namespace interprocess::ipcdetail;
}

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{

// --------------------------------------------------------------------------------------------------------------------
struct Application::Kernel
{
	boost::thread	mThread;
	shared_service	mActive;
	shared_service	mNext;
	size_t			mId;

	Task::Fn		mChooseService;

	Kernel();
	~Kernel();

	inline bool Running() const { return mActive; }

	void Main(Application* application);
	void StartThread();
	void StopThread();
	void ChooseService();
	shared_service SelectService();
};

// --------------------------------------------------------------------------------------------------------------------
Application::Kernel::Kernel()
{
	mChooseService = std::bind<void>(&Application::Kernel::ChooseService, this);
	mActive.reset();
}

// --------------------------------------------------------------------------------------------------------------------
Application::Kernel::~Kernel()
{
	StopThread();
	if (mThread.joinable())
	{
		mThread.join();
	}
}

// --------------------------------------------------------------------------------------------------------------------
void Application::Kernel::Main(Application* application)
{
	Application::sApplication.reset(application);
	application->mActiveService.reset(&mActive);
	ASSERT(!mActive);
	ChooseService();
	while(mActive && !mActive->mTaskQueue.empty()) 
	{
		ASSERT(Service::Queued != mActive->State());
		shared_task task = mActive->mTaskQueue.pop();
		task->task();
	}
	mActive.reset();
}

// --------------------------------------------------------------------------------------------------------------------
void Application::Kernel::StartThread()
{	// What if we are already running?
	Application* application = Application::Get();
	boost::thread kernel(std::bind(&Application::Kernel::Main, this, application));
	mThread.swap(kernel);
}

// --------------------------------------------------------------------------------------------------------------------
void Application::Kernel::StopThread()
{	// Without an active service the thread will exit
	mActive.reset();
}

// --------------------------------------------------------------------------------------------------------------------
void Application::Kernel::ChooseService()
{
	shared_service selected;
	if (mActive)
	{	// Queue current service
		mActive->mState = Service::Queued;
		mActive.reset();
	}

	// Select the next service
	if (mNext)
	{
		mNext->mState = Service::Running;
		mActive = mNext;
		mNext.reset();
	}
	else
	{
		mActive = SelectService();
	}

	if (mActive)
	{
		mActive->Post(mChooseService); // Schedule the next attempt to switch services
	}
}

// --------------------------------------------------------------------------------------------------------------------
shared_service Application::Kernel::SelectService()
{	
	shared_service candidate;
	Application* const application = Application::Get();
	unsigned int start = application->mNextService.get();
	bool isSelectionPossible = false;
	bool selected = false;

	Application::SharedLock lock(application->mServiceMutex);
	const size_t size = application->mServices.size();
	if (0 != size)
	{
		do
		{
			unsigned int next = 0;
			unsigned int index = 0;
			do 
			{	
				index = application->mNextService.get();
				next = (index + 1) % size;
				ASSERT(index != next); // Ensures that 2 <= mServiceList.size()
				// try again if another thread has changed the value before me 
			} while(index != application->mNextService.compare_exchange(next, index));

			candidate = application->mServices[index].lock();
			if (!candidate || Service::Queued != candidate->mState.compare_exchange(Service::Running, Service::Queued))
			{
				candidate.reset();
			}

			// If we have looped over all services and their is no possible selection then exit loop.
			const bool accepted = candidate;
			const bool atEnd	= next == start;
			const bool sleep	= atEnd & isSelectionPossible;
			const bool exit		= atEnd & !isSelectionPossible;
			isSelectionPossible = (atEnd & accepted) | (!atEnd & (isSelectionPossible | accepted));
			selected = accepted | exit;
			if (sleep)
			{
				Application::Sleep(1);
			}
			// If the service is valid and is being processed then pick another.
		} while(!selected);
	}
	ASSERT(!candidate || Service::Queued != candidate->mState.get());
	return candidate;
}

// --------------------------------------------------------------------------------------------------------------------
Application::ActiveApplication Application::sApplication(&Empty<Application>);

// --------------------------------------------------------------------------------------------------------------------
Application::Application()
: mActiveService(&Empty<shared_service>)
, mNextService(0)
, mRunResult(0)
{
}

// --------------------------------------------------------------------------------------------------------------------
void Application::Stop(int runResult)
{
	mRunResult = runResult;

	{
		SharedLock lock(mServiceMutex);
		for(ServiceList::iterator i = mServices.begin(); i != mServices.end(); ++i)
		{
			shared_service service = (*i).lock();
			if (service)
			{
				service->Stop();
			}
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------
int Application::Run(unsigned numThreads)
{
	bool isRunning = NULL != sApplication.get();
	if (isRunning)
	{
		return -1;
	}

	sApplication.reset(this);

	if (0 >= numThreads)
	{
		numThreads = boost::thread::hardware_concurrency();
	}

	// The services to initialize first are the kernels.
	mNextService = mServices.size();

	{	// Create the requested kernel
		Application::UniqueLock lock(mKernelMutex);
		mKernels.reserve(numThreads);
		for(int i = numThreads; i--; )
		{
			shared_service service = Service::Create();
			Register(service);
			service->MakeProvider<Kernel>();
			Application::Kernel* provider = service->Provider<Kernel>();
			if (!mKernels.empty())
			{	// Launch a thread for all kernels except the current thread.
				service->Post(std::bind<void>(&Application::Kernel::StartThread, provider));
			}
			provider->mId = mKernels.size();
			mKernels.push_back(service);
		}
	}

	shared_service primary = mServices[mNextService.get()].lock();
	primary->Provider<Kernel>()->Main(this);

	for(KernelList::iterator service = mKernels.begin(); service != mKernels.end(); ++service)
	{
		if (primary != (*service))
		{
			Kernel* kernel = (*service)->Provider<Kernel>();
			if (kernel->mThread.joinable())
			{
				kernel->mThread.join();
			}
		}
	}
	mKernels.clear();
	mServices.clear();
	
	sApplication.reset();
	return mRunResult;
}

// --------------------------------------------------------------------------------------------------------------------
unsigned Application::NumHardwareThreads()
{
	return boost::thread::hardware_concurrency();
}

// --------------------------------------------------------------------------------------------------------------------
void Application::Sleep(int milliseconds)
{
	boost::chrono::milliseconds duration(milliseconds);
	boost::this_thread::sleep_for(duration);
}

// --------------------------------------------------------------------------------------------------------------------
void Application::Register(shared_service service)
{
	weak_service serviceItem;
	{	// If something has expired then reuse the slot
		SharedLock lock(mServiceMutex);
		Application::ServiceList::iterator item = mServices.begin();
		while(item != mServices.end() && !item->expired())
		{
			++item;
		}
		if (item != mServices.end())
		{
			*item = serviceItem = service;
		}
	}

	if (serviceItem.expired())
	{
		UniqueLock lock(mServiceMutex);
		mServices.erase(std::remove_if(mServices.begin(), mServices.end(), Expired<Service>()), 
						mServices.end());
		mServices.push_back(service);
	}

	service->mState = Service::Queued;
}

// --------------------------------------------------------------------------------------------------------------------
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
