#pragma once

#include <Application\Application.h>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{

class Task;
typedef std::shared_ptr<Task> shared_task;
typedef std::weak_ptr<Task> weak_task;

// --------------------------------------------------------------------------------------------------------------------
class Task
{
public:
	typedef std::function<void ()> Fn;

					Task();
					Task(Fn task, shared_service& service);

	Fn				task;
	weak_service	service;
	atomic<Task*>	next;
};

// --------------------------------------------------------------------------------------------------------------------
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
