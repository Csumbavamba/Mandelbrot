#pragma once

#include <queue>
#include <mutex>

template<typename T>
class WorkQueue
{
public:
	WorkQueue() {};

	// Insert object at the back of the queue, notify the threads
	void PushTask(const T& task);

	// Retrieves the front item from the queue and pops it off as well
	// If the queue is empty just returns false
	bool NonBlockingPop(T& workItem);
	void BlockingPop(T& workItem);

	// Check if the queue is empty or not
	bool Empty() const;

private:
	std::queue<T> workQueue;
	mutable std::mutex mutex;
	std::condition_variable workCondition;

};

template<typename T>
inline void WorkQueue<T>::PushTask(const T & task)
{
	std::lock_guard<std::mutex> lock(mutex);
	workQueue.push(task);
	workCondition.notify_all();
}

template<typename T>
inline bool WorkQueue<T>::NonBlockingPop(T & workItem)
{
	// If the queue is empty, return false
	std::lock_guard<std::mutex> lock(mutex);
	if (workQueue.empty())
	{
		return false;
	}

	workItem = workQueue.front();
	workQueue.pop();

	return true;
}

template<typename T>
inline void WorkQueue<T>::BlockingPop(T & workItem)
{
	std::unique_lock<std::mutex> lock(mutex);
	// if the queue is empty block the thread until it's not empty
	workCondition.wait(lock, [this] { return !workQueue.empty(); });
	workItem = std::move(workQueue.front());
	workQueue.pop();
}

template<typename T>
inline bool WorkQueue<T>::Empty() const
{
	std::lock_guard<std::mutex> lock(mutex);
	return workQueue.empty();
}
