
#pragma once

#ifndef _CLOCK_H_
#define _CLOCK_H_

// Library Includes
#include <chrono>
#include <vector>

// Local Includes

// Types

// Constants

// Prototypes
class Clock
{
	// Member Functions
public:
	static Clock * GetInstance();

	bool Initialise();
	void Update();
	float GetDeltaTick();

protected:

private:
	Clock();
	~Clock();
	Clock(const Clock& _kr);
	Clock& operator= (const Clock& _kr);

	static Clock * instance;

	// Member Variables
public:

protected:
	double timeElapsed;
	double deltaTime;
	std::chrono::high_resolution_clock::time_point lastTime;
	std::chrono::high_resolution_clock::time_point currentTime;

	std::vector<double> timeHistory;

	long long numberOfCounts;

private:

};
#endif // 

