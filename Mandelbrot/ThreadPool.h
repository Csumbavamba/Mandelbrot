#include <thread>
#include <vector>
#include <cstdint>
#include <cstdio>
#include <queue>
#include <mutex>
#include <string>
#include <condition_variable>
#include <atomic>
#include <memory>

#include <functional>
#include <future>

#include "WorkQueue.h"
#include "Task.h"


class ThreadPool {
	// typedef std::function<void(void)> Task;

public:
	static ThreadPool * GetInstance();

	void ProcessTask();
	void Stop();
	void Initialise();

	bool isQueueEmpty();
	void Start();
	void Submit(Task task);

	size_t GetNumberOfThreads() const;


private:
	ThreadPool();
	~ThreadPool();

	static ThreadPool * instance;

	std::vector<std::thread> vectorOfThreads;
	WorkQueue<Task> * queue;
	std::condition_variable conditionVariable;
	std::atomic_bool isFinished;

	size_t numberOfThreads;

};
