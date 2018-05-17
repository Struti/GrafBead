#include "TimeCalculator.h"

#include <GLFW\glfw3.h>

static double elapsedTime = 0;
static double deltaTime = 0;

static double frameStart = 0;

double ti::CalculateTime::getFrameDelta()
{
	return deltaTime;
}

double ti::CalculateTime::getElapsedTime()
{
	return glfwGetTime();
}

void ti::CalculateTime::init()
{
	frameStart = glfwGetTime();
}

void ti::CalculateTime::calculateFrameTime()
{
	double cur = glfwGetTime();
	deltaTime = cur - frameStart;
	frameStart = cur;
}
