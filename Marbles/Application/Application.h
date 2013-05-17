#pragma once

#include <memory>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
// --------------------------------------------------------------------------------------------------------------------
class Task;
typedef std::shared_ptr<Task> shared_task;
typedef std::weak_ptr<Task> weak_task;

class Service;
typedef std::shared_ptr<Service> shared_service;
typedef std::weak_ptr<Service> weak_service;

// --------------------------------------------------------------------------------------------------------------------
class Application
{
public:
	struct UserSettings
	{
		float FramesPerSecond;

		UserSettings()
		: FramesPerSecond(60.0f)
		{
		}
	};
	static UserSettings& Settings();
	//static int CommandLineCount();
	//static char*[] CommandLine();
	static unsigned NumHardwareThreads();

	static void Sleep(int milliseconds);
	static Application* Get();

	Application();
	~Application();
	void Stop(int runResult);
	int Run(unsigned numThreads = 0); // The value given to Application::Stop() is returned by this function

	template<typename Fn>
	shared_service Start();
	template<typename Fn, typename A0>
	shared_service Start(A0& a0);
	template<typename Fn, typename A0, typename A1>
	shared_service Start(A0& a0, A1& a1);
	template<typename Fn, typename A0, typename A1, typename A2>
	shared_service Start(A0& a0, A1& a1, A2& a2);
	template<typename Fn, typename A0, typename A1, typename A2, typename A3>
	shared_service Start(A0& a0, A1& a1, A2& a2, A3& a3);
	template<typename Fn, typename A0, typename A1, typename A2, typename A3, typename A4>
	shared_service Start(A0& a0, A1& a1, A2& a2, A3& a3, A4& a4);
	template<typename Fn, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
	shared_service Start(A0& a0, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5);

private:
	struct Kernel;
	struct Implementation;
	friend class Service;

	shared_service ActiveService() const;
	void Register(shared_service service);
	void Unregister(shared_service service);

	Implementation* mImplementation;
};

// --------------------------------------------------------------------------------------------------------------------
template<typename Fn>
shared_service Application::Start()
{
	shared_service service = Service::Create();
	Register(service);
	if (service)
	{
		service->Post(std::bind<void>(	&Service::MakeProvider<Fn>, 
										service.get()));
	}
	return service;
}

// --------------------------------------------------------------------------------------------------------------------
template<typename Fn, typename A0>
shared_service Application::Start(A0& a0)
{
	shared_service service = Service::Create();
	Register(service);
	if (service)
	{
		service->Post(std::bind<void>(	&Service::MakeProvider<Fn, A0>, 
										service.get(), a0));
	}
	
	return service;
}

// --------------------------------------------------------------------------------------------------------------------
template<typename Fn, typename A0, typename A1>
shared_service Application::Start(A0& a0, A1& a1)
{
	shared_service service = Service::Create();
	Register(service);
	if (service)
	{
		service->Post(std::bind<void>(	&Service::MakeProvider<Fn, A0, A1>, 
										service.get(), a0, a1));
	}
	return service;
}

// --------------------------------------------------------------------------------------------------------------------
template<typename Fn, typename A0, typename A1, typename A2>
shared_service Application::Start(A0& a0, A1& a1, A2& a2)
{
	shared_service service = Service::Create();
	Register(service);
	if (service)
	{
		service->Post(std::bind<void>(	&Service::MakeProvider<Fn, A0, A1, A2>, 
										service.get(), a0, a1, a2));
	}
	return service;
}

// --------------------------------------------------------------------------------------------------------------------
template<typename Fn, typename A0, typename A1, typename A2, typename A3>
shared_service Application::Start(A0& a0, A1& a1, A2& a2, A3& a3)
{
	shared_service service = Service::Create();
	Register(service);
	if (service)
	{
		service->Post(std::bind<void>(	&Service::MakeProvider<Fn, A0, A1, A2, A3>, 
										service.get(), a0, a1, a2, a3));
	}
	return service;
}

// --------------------------------------------------------------------------------------------------------------------
template<typename Fn, typename A0, typename A1, typename A2, typename A3, typename A4>
shared_service Application::Start(A0& a0, A1& a1, A2& a2, A3& a3, A4& a4)
{
	shared_service service = Service::Create();
	Register(service);
	if (service)
	{
		service->Post(std::bind<void>(	&Service::MakeProvider<Fn, A0, A1, A2, A3, A4>, 
										service.get(), a0, a1, a2, a3, a4));
	}
	return service;
}

// --------------------------------------------------------------------------------------------------------------------
template<typename Fn, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
shared_service Application::Start(A0& a0, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5)
{
	shared_service service = Service::Create();
	Register(service);
	if (service)
	{
		service->Post(std::bind<void>(	&Service::MakeProvider<Fn, A0, A1, A2, A3, A4, A5>, 
										service.get(), a0, a1, a2, a3, a4, a5));
	}
	return service;
}

// --------------------------------------------------------------------------------------------------------------------
} // namespace Marbles

// End of File --------------------------------------------------------------------------------------------------------
