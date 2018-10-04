#include "ThreadPool.h"
#include "Task.h"

#include <queue>
#include <algorithm>

ThreadPool * ThreadPool::instance = nullptr;

bool ThreadPool::isQueueEmpty()
{
	return queue->Empty();
}

void ThreadPool::Start()
{
	
	// Get the hardware concurrency
	for (size_t i = 0; i < numberOfThreads; ++i)
	{
		vectorOfThreads.push_back(std::thread(&ThreadPool::ProcessTask, this));
	}
}

void ThreadPool::Submit(Task task)
{
	queue->PushTask(task);
}

size_t ThreadPool::GetNumberOfThreads() const
{
	return numberOfThreads;
}


//Constructing the queue, creating threads until we reach the maxumum of cores in the cpu
ThreadPool::ThreadPool()
{
	isFinished = false;
	numberOfThreads = std::thread::hardware_concurrency();
}

//Destructor
ThreadPool::~ThreadPool()
{
	//Finished
	isFinished = true;
	conditionVariable.notify_all();

	// Wait for threads to finish before we exit
	for (size_t i = 0; i < vectorOfThreads.size(); i++)
	{
		//Check too see if is joinable
		if (vectorOfThreads[i].joinable())
		{
			//Join thread
			vectorOfThreads[i].join();
		}
	}

	

	delete queue;
	queue = 0;
}

ThreadPool * ThreadPool::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new ThreadPool();
	}
	return instance;
}



void ThreadPool::ProcessTask()
{
	while (!isFinished)
	{
		Task workItem;

		queue->BlockingPop(workItem);

		workItem.Execute();

	}
}



void ThreadPool::Stop()
{
	isFinished = true;
}

void ThreadPool::Initialise()
{
	
	queue = new WorkQueue<Task>();
}


