// --------------------------------------------------------------------------------------------------------------------
#include <Application\Application.h>
#include <Application\Service.h>
#include <boost\any.hpp>
#include <boost\thread.hpp>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{

// --------------------------------------------------------------------------------------------------------------------
struct Application::Implementation
{
	typedef boost::shared_mutex					SharedMutex;
	typedef boost::unique_lock<SharedMutex>		UniqueLock;
	typedef boost::shared_lock<SharedMutex>		SharedLock;
	typedef boost::thread_specific_ptr<shared_service> ActiveService;
	typedef boost::thread_specific_ptr<Application> ActiveApplication;
	typedef std::vector<weak_service>			ServiceList;
	typedef std::vector<shared_service>			KernelList;

	Implementation()
	: activeService(&Empty<shared_service>)
	, nextService(0)
	, runResult(0)
	{}

	SharedMutex				kernelMutex;
	KernelList				kernels;

	SharedMutex				serviceMutex;
	ServiceList				services;

	ActiveService			activeService;
	atomic<unsigned int>	nextService;
	int						runResult;

	static ActiveApplication sApplication;
};

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
	Application::Implementation::sApplication.reset(application);
	application->mImplementation->activeService.reset(&mActive);
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
	Application::Implementation* const application = Application::Get()->mImplementation;
	unsigned int start = application->nextService.get();
	bool isSelectionPossible = false;
	bool selected = false;

	Implementation::SharedLock lock(application->serviceMutex);
	const size_t size = application->services.size();
	if (0 != size)
	{
		do
		{
			unsigned int next = 0;
			unsigned int index = 0;
			do 
			{	
				index = application->nextService.get();
				next = (index + 1) % size;
				ASSERT(index != next); // Ensures that 2 <= mServiceList.size()
				// try again if another thread has changed the value before me 
			} while(index != application->nextService.compare_exchange(next, index));

			candidate = application->services[index].lock();
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
Application::Implementation::ActiveApplication Application::Implementation::sApplication(&Empty<Application>);

// --------------------------------------------------------------------------------------------------------------------
Application::Application()
{
	mImplementation = new Implementation();
}

// --------------------------------------------------------------------------------------------------------------------
Application::~Application()
{
	delete mImplementation;
}

// --------------------------------------------------------------------------------------------------------------------
void Application::Stop(int runResult)
{
	mImplementation->runResult = runResult;

	{
		Implementation::SharedLock lock(mImplementation->serviceMutex);
		for(Implementation::ServiceList::iterator i = mImplementation->services.begin(); 
			i != mImplementation->services.end(); 
			++i)
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
	bool isRunning = NULL != Implementation::sApplication.get();
	if (isRunning)
	{
		return -1;
	}

	Implementation::sApplication.reset(this);

	if (0 >= numThreads)
	{
		numThreads = boost::thread::hardware_concurrency();
	}

	// The services to initialize first are the kernels.
	mImplementation->nextService = mImplementation->services.size();

	{	// Create the requested kernel
		Application::Implementation::UniqueLock lock(mImplementation->kernelMutex);
		mImplementation->kernels.reserve(numThreads);
		for(int i = numThreads; i--; )
		{
			shared_service service = Service::Create();
			Register(service);
			service->MakeProvider<Kernel>();
			Application::Kernel* provider = service->Provider<Kernel>();
			if (!mImplementation->kernels.empty())
			{	// Launch a thread for all kernels except the current thread.
				service->Post(std::bind<void>(&Application::Kernel::StartThread, provider));
			}
			provider->mId = mImplementation->kernels.size();
			mImplementation->kernels.push_back(service);
		}
	}

	shared_service primary = mImplementation->services[mImplementation->nextService.get()].lock();
	primary->Provider<Kernel>()->Main(this);

	for(Implementation::KernelList::iterator service = mImplementation->kernels.begin(); 
		service != mImplementation->kernels.end(); 
		++service)
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
	mImplementation->kernels.clear();
	mImplementation->services.clear();
	
	Implementation::sApplication.reset();
	return mImplementation->runResult;
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
shared_service Application::ActiveService() const
{
	return *mImplementation->activeService;
}

// --------------------------------------------------------------------------------------------------------------------
void Application::Register(shared_service service)
{
	weak_service serviceItem;
	{	// If something has expired then reuse the slot
		Implementation::SharedLock lock(mImplementation->serviceMutex);
		Implementation::ServiceList::iterator item = mImplementation->services.begin();
		while(item != mImplementation->services.end() && !item->expired())
		{
			++item;
		}
		if (item != mImplementation->services.end())
		{
			*item = serviceItem = service;
		}
	}

	if (serviceItem.expired())
	{
		Implementation::UniqueLock lock(mImplementation->serviceMutex);
		mImplementation->services.erase(
			std::remove_if(	mImplementation->services.begin(), 
							mImplementation->services.end(), 
							Expired<Service>()), 
			mImplementation->services.end());
		mImplementation->services.push_back(service);
	}

	service->mState = Service::Queued;
}

// --------------------------------------------------------------------------------------------------------------------
void Application::Unregister(shared_service service)
{
	Implementation::SharedLock lock(mImplementation->serviceMutex);
	Implementation::ServiceList::iterator item = mImplementation->services.begin();
	while(item->lock() != service && item != mImplementation->services.end())
	{
		++item;
	}
	if (item != mImplementation->services.end())
	{	
		item->reset(); // Item is no longer a candidate
	}

	service->mState = Service::Stopped;
}

// --------------------------------------------------------------------------------------------------------------------
Application* Application::Get() 
{ 
	Application* app = Implementation::sApplication.get();
	ASSERT(app || !"You must call Application::Run before calling this function."); 
	return app; 
}

// --------------------------------------------------------------------------------------------------------------------
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
