#include <Application/Service.h>
#include <Application/Application.h>

BOOST_AUTO_TEST_SUITE( service )

struct ExecutedService
{
	bool executed;
	~ExecutedService() {}
	ExecutedService() 
	{
		executed = false;
		
		Marbles::shared_service active = Marbles::Service::Active();
		active->Post(std::bind(&ExecutedService::Execute, this));
	}

	void Execute() 
	{
		executed = true; 
		Marbles::Service::Active()->Stop();
	}
};

struct ApplicationStop
{
	int count;
	int end;
	Marbles::Task::Fn updateTask; 

	ApplicationStop(const int count) 
		: count(0)
		, end(count)
	{
		Marbles::shared_service active = Marbles::Service::Active();
		updateTask = std::bind(&ApplicationStop::Update, this);
		active->Post(updateTask);
	}

	~ApplicationStop() 
	{}

	void Update() 
	{
		++count;
		if (count == end)
		{
			Marbles::Application::Get()->Stop(0);
		}
		else
		{
			Marbles::shared_service active = Marbles::Service::Active();
			active->Post(updateTask);
		}
	}
};

BOOST_AUTO_TEST_CASE( single_thread_test )
{
	BOOST_MESSAGE( "service.single_thread_test" );

	const int numCyclesToStop = 10;
	Marbles::Application app;
	Marbles::shared_service executeTest = app.Start<ExecutedService>();
	Marbles::shared_service stopTest = app.Start<ApplicationStop>(numCyclesToStop);

	app.Run(1);

	BOOST_CHECK(executeTest->Provider<ExecutedService>()->executed);
	BOOST_CHECK_EQUAL(numCyclesToStop, stopTest->Provider<ApplicationStop>()->count);
}

BOOST_AUTO_TEST_CASE( multiple_thread_test )
{
	BOOST_MESSAGE( "service.multiple_thread_test" );
	const int numCyclesToStop = 1000;
	Marbles::Application app;
	Marbles::shared_service executeTest = app.Start<ExecutedService>();
	Marbles::shared_service stopTest = app.Start<ApplicationStop>(numCyclesToStop);

	app.Run();

	BOOST_CHECK(executeTest->Provider<ExecutedService>()->executed);
	BOOST_CHECK_EQUAL(numCyclesToStop, stopTest->Provider<ApplicationStop>()->count);
}

BOOST_AUTO_TEST_SUITE_END()
