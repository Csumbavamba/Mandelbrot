#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

#include "ThreadPool.h"
#include "Task.h"
#include <mutex>

#include <fstream>
#include <iostream>
#include <chrono>

std::mutex mutex;

struct Pixel
{
	float red;
	float green;
	float blue;
};


double timeElapsed;
std::chrono::high_resolution_clock::time_point finalTime;
std::chrono::high_resolution_clock::time_point startTime;

// Get the required input values from the file
std::ifstream fin("input.txt");
int maxIterations;
double minReal, maxReal, minImaginary, maxImaginary;

const int imageWidth = 800;
const int imageHeight = 600;


Pixel pixels[imageWidth * imageHeight];
Pixel pixelPattern[1000 * 1000];


bool processingDone;

int location = 0; // TODO ask ASma


void Render();
void Update();
void Initialise();
void CreatePixelData(int x, int y);

void ProcessPackage(int y);

void CreatePattern();
int CalculateMandelbrot(double cReal, double cImaginary, int MAX_ITERATIONS);

double MapToReal(int x, int imageWidth, double minReal, double maxReal)
{
	double range = maxReal - minReal;
	// [0, width]
	// [0, minReal - maxReal] - value * range / width
	// [minReal, maxReal] - last step + MinReal
	return x * (range / imageWidth) + minReal;
}
double MapToImaginary(int y, int imageHeight, double minImaginary, double maxImaginary)
{
	double range = maxImaginary - minImaginary;
	return y * (range / imageHeight) + minImaginary;
}


int main (int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(imageWidth, imageHeight);
	glutCreateWindow("Mandelbrot Multithreading");

	glClearColor(1, 0, 0, 0); // Clear Window
	glClear(GL_COLOR_BUFFER_BIT);

	glewInit();

	Initialise();


	// ThreadPool::GetInstance()->Start();

	// Register callbacks
	glutDisplayFunc(Render);

	glutIdleFunc(Update);

	glutMainLoop();

	// TODO destroy queue 
	return 0;


}

void Update()
{
	processingDone = ThreadPool::GetInstance()->isQueueEmpty();

	if (processingDone)
	{
		finalTime = std::chrono::high_resolution_clock::now();
		timeElapsed = (double)(std::chrono::duration_cast<std::chrono::milliseconds>(finalTime - startTime).count());

		std::cout << timeElapsed << std::endl;
	}

	glutPostRedisplay();
}

void Render()
{
	glClearColor(1.0, 1.0, 0.0, 1.0); // clear red
	glClear(GL_COLOR_BUFFER_BIT);

	
	glDrawPixels(imageWidth, imageHeight, GL_RGB, GL_FLOAT, pixels);
	glutSwapBuffers();
}

void Initialise()
{
	glViewport(0, 0, imageWidth, imageHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, imageWidth, 0, imageHeight);

	//// Get the required input values from the file
	//if (!fin)
	//{
	//	std::cout << "Could not open file!" << std::endl;
	//	std::cin.ignore();
	//	return;
	//}
	//fin >> imageWidth >> imageHeight >> maxIterations;
	//fin >> minReal >> maxReal >> minImaginary >> maxImaginary;
	//fin.close(); // Not neccessary, good practice

	minReal = -1.9;
	minImaginary = -1.9;

	maxReal = 1.9;
	maxImaginary = 1.9;
	maxIterations = 1024;

	CreatePattern();


	startTime = std::chrono::high_resolution_clock::now();
	ThreadPool::GetInstance()->Initialise();
	ThreadPool::GetInstance()->Start();
	processingDone = false;

	// size_t numberOfThreads = ThreadPool::GetInstance()->GetNumberOfThreads();
	
	// Create every pixel based on image Size
	for (int y = 0; y < imageHeight; ++y)
	{
		ProcessPackage(y);
		// ThreadPool::GetInstance()->Submit(Task(ProcessPackage, y));
	
	}
	
	
	
	
	
	// ThreadPool::GetInstance()->Start();
	// 


	
}

void ProcessPackage(int y)
{

	for (int x = 0; x < imageWidth; ++x)
	{
		CreatePixelData(x, y);
	}
}

void CreatePixelData(int x, int y)
{
	
	// Find the imaginary	and real values for c, corresponding to that x,y pixen in the image
	double cReal = MapToReal(x, imageWidth, minReal, maxReal);
	double cImaginary = MapToImaginary(y, imageHeight, minImaginary, maxImaginary);
	// Find the number of iterations in the Mandelbrot formula using c
	int numberToSkip = CalculateMandelbrot(cReal, cImaginary, maxIterations);

	if (numberToSkip >= maxIterations)
	{
		pixels[location].red = 0;
		pixels[location].blue = 0;
		pixels[location].green = 0;
	}
	else
	{
		pixels[location].red = pixelPattern[numberToSkip].red;
		pixels[location].blue = pixelPattern[numberToSkip].blue;
		pixels[location].green = pixelPattern[numberToSkip].green;

	}
	
	
	location++; // TODO THIS IS THE FUCKING PROBLEM
	
}



void CreatePattern()
{
	int patternIteration = 0;

	// Generate color pattern for drawing
	for (float r = 0.01; r <= 1.0; r += 0.01)
	{
		for (float g = 0.01; g <= 1.0; g += 0.01)
		{
			for (float b = 0.01; b <= 1.0; b += 0.01)
			{
				pixelPattern[patternIteration].red = b  * 5;
				pixelPattern[patternIteration].green = r * 5;
				pixelPattern[patternIteration].blue = g * 20;

				patternIteration++;
			}
		}
	}

	for (; patternIteration <= 999; patternIteration++)
	{
		pixelPattern[patternIteration].red = 1;
		pixelPattern[patternIteration].green = 1;
		pixelPattern[patternIteration].blue = 1;
	}
}


int CalculateMandelbrot(double cReal, double cImaginary, int MAX_ITERATIONS)
{
	
	int iteration = 0;
	double zReal = 0.0;
	double zImaginary = 0.0;

	// Repeat until the number goes into infinity or the max number of iterations has been reached
	while (iteration < MAX_ITERATIONS && (zReal * zReal + zImaginary * zImaginary) < 4.0)
	{
		double temp = zReal * zReal - zImaginary * zImaginary + cReal;
		zImaginary = 2 * zReal * zImaginary + cImaginary;
		zReal = temp;

		iteration++;
	}

	return iteration;
}


