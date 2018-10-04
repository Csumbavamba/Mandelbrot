#// Library Includes
#include <chrono>

// Local Includes

// This Includes
#include "clock.h"

// Static Variables

// Static Function Prototypes

Clock * Clock::instance = NULL;

// Implementation
//****************************************************
// CClock: CClock Class Constructor
// @author: 
// @parameter: No parameters
//
// @return: none
//*****************************************************
Clock::Clock()
	: timeElapsed(0.0f)
	, deltaTime(0.0f)
{

}

//****************************************************
// ~CClock: CClock Class Destructor
// @author: 
// @parameter: No parameters
//
// @return: none
//*****************************************************
Clock::~Clock()
{

}

Clock * Clock::GetInstance()
{
	if (instance == NULL)
	{
		instance = new Clock();
	}
	return instance;
}

//****************************************************
// Initialise: CClock Class Initialiser - sets the first time values
// @author: 
// @parameter: No parameters
//
// @return: true if initialisation is successful, false if not
//*****************************************************
bool Clock::Initialise()
{
	currentTime = std::chrono::high_resolution_clock::now();
	return (true);
}

//****************************************************
// Process: processes the change in time since it was last called
// @author:
// @parameter: No parameters
//
// @return: void
//*****************************************************
void Clock::Update()
{
	lastTime = currentTime;

	currentTime = std::chrono::high_resolution_clock::now();


	deltaTime = (double)(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count());

	timeElapsed += deltaTime;
}

//****************************************************
// GetDeltaTick: gets the current delta tick value
// @author: 
// @parameter: No parameters
//
// @return: the current delta tick value
//*****************************************************
float Clock::GetDeltaTick()
{
	return (float)((deltaTime)/100.0f);
}