#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"


struct Pixel
{
	float red;
	float green;
	float blue;

	// float positionX, positionY;
};

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Pixel pixels[SCREEN_HEIGHT * SCREEN_WIDTH];




void Render();
void Initialise();
void CalculateMandelbrot();


int main (int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(SCREEN_HEIGHT, SCREEN_HEIGHT);
	glutCreateWindow("Mandelbrot Multithreading");

	glClearColor(1, 1, 1, 0); // Clear Window
	glClear(GL_COLOR_BUFFER_BIT);

	glewInit();

	Initialise();

	// Register callbacks
	glutDisplayFunc(Render);

	glutMainLoop();
	return 0;


}

void Render()
{


	glClearColor(1.0, 0.0, 0.0, 1.0); // clear red
	glClear(GL_COLOR_BUFFER_BIT);
	
	glDrawPixels(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_FLOAT, pixels);
	glutSwapBuffers();
}

void Initialise()
{
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);


	for (int iteration = 0; iteration < SCREEN_WIDTH * SCREEN_HEIGHT; ++iteration)
	{
		pixels[iteration].red = 1;
		pixels[iteration].green = 1;
		pixels[iteration].blue = 1;
	}

	CalculateMandelbrot();
		
		

}


void CalculateMandelbrot()
{
	// Calculate Mandelbrot every frame
		// Calculate pixel positions
		// Calculate pixel colors

	glutPostRedisplay();
}


