#pragma once

#include <functional>
#include <future>

//template <typename T>
class Task
{
public:
	Task() {};
	Task(std::function<void(int y)> function, int y)
		:task(function)
		, y(y)
	{ }

	void Execute()
	{
		task(y);
	}

private:
	std::function<void(int y)> task;
	float y;
};

