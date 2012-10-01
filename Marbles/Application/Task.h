#pragma once

#include <vector>
#include <Application\Message.h>

class Task;
typedef shared_ptr<Task> shared_task;
typedef weak_ptr<Task> weak_task;

class Task
{
public:
	enum Status {
		Init,
		Delayed,

		Running,
		Interrupted,
		Done,
	};

	static const std::vector<weak_task>& ActiveTasks();
	static const weak_task& ActiveTask();

	template<typename T> bool Post(const T&);
	void ClearMessages();
	// Start()
	// StartAfter(int timeout);
	// StartAfter(const Task&);
	// Stop()
	// 
	// OnStart(fnObj, const Task&)
	// OnDone(fnObj)
	// OnDone(fnObj, const Task&)
	// OnInterrupt(fnObj)
	// OnInterrupt(fnObj, const Task&)
protected:
	friend class PostService
	PostService mPostOffice;
};

template<typename T> 
bool Task::Post(const T& message)
{
	mPostOffice.Post(message);
}

// End of file --------------------------------------------------------------------------------------------------------
