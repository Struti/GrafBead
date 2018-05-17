#include "Control.h"
#include "Camera.h"

ti::CameraState ti::Controller::cameraState;
ti::Mouse ti::Controller::mouseData;
ti::DirectionalLight* ti::Controller::light = nullptr;

void ti::Controller::keyMapping(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (key == GLFW_KEY_W || key == GLFW_KEY_UP)
	{
		if (action == GLFW_REPEAT || action == GLFW_PRESS)
		{
			cameraState.movementState = cameraState.movementState | CameraMovement::FORWARD;
		}
		else if (action == GLFW_RELEASE)
		{
			cameraState.movementState = cameraState.movementState & ~CameraMovement::FORWARD;
		}
	}

	if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN)
	{
		if (action == GLFW_REPEAT || action == GLFW_PRESS)
		{
			cameraState.movementState = cameraState.movementState | CameraMovement::BACKWARD;
		}
		else if (action == GLFW_RELEASE)
		{
			cameraState.movementState = cameraState.movementState & ~CameraMovement::BACKWARD;
		}
	}

	if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
	{
		if (action == GLFW_REPEAT || action == GLFW_PRESS)
		{
			cameraState.movementState = cameraState.movementState | CameraMovement::LEFT;
		}
		else if (action == GLFW_RELEASE)
		{
			cameraState.movementState = cameraState.movementState & ~CameraMovement::LEFT;
		}
	}

	if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
	{
		if (action == GLFW_REPEAT || action == GLFW_PRESS)
		{
			cameraState.movementState = cameraState.movementState | CameraMovement::RIGHT;
		}
		else if (action == GLFW_RELEASE)
		{
			cameraState.movementState = cameraState.movementState & ~CameraMovement::RIGHT;
		}
	}

	if (key == GLFW_KEY_F1)
	{
		if (action == GLFW_REPEAT || action == GLFW_PRESS)
		{
			
		}
		else if (action == GLFW_RELEASE)
		{
			
		}
	}

	if (key == GLFW_KEY_E)
	{
		if (action == GLFW_REPEAT || action == GLFW_PRESS)
		{
		}
		else if (action == GLFW_RELEASE)
		{

		}
	}

	if (key == GLFW_KEY_R)
	{
		if (action == GLFW_REPEAT || action == GLFW_PRESS)
		{
		}
		else if (action == GLFW_RELEASE)
		{

		}
	}
	if (key == GLFW_KEY_KP_ADD)
	{
		if (action == GLFW_REPEAT || action == GLFW_PRESS)
		{
			light->setDiffuseColor(light->getDiffuseColor()  + glm::vec3(0.1, 0.1, 0.1));
		}
		else if (action == GLFW_RELEASE)
		{

		}
	}
	if (key ==GLFW_KEY_KP_SUBTRACT)
	{
		if (action == GLFW_REPEAT || action == GLFW_PRESS)
		{
			light->setDiffuseColor(light->getDiffuseColor() - glm::vec3(0.1, 0.1, 0.1));
		}
		else if (action == GLFW_RELEASE)
		{

		}
	}

	glm::vec3 diffNorm = light->getDiffuseColor();
	diffNorm.r = diffNorm.r > 1 ? 1 : diffNorm.r < 0 ? 0 : diffNorm.r;
	diffNorm.g = diffNorm.g > 1 ? 1 : diffNorm.g < 0 ? 0 : diffNorm.g;
	diffNorm.b = diffNorm.b > 1 ? 1 : diffNorm.b < 0 ? 0 : diffNorm.b;
	light->setDiffuseColor(diffNorm);
}
void ti::Controller::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		mouseData.isRightPressed = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		mouseData.isRightPressed = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mouseData.isLeftPressed = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		mouseData.isLeftPressed = false;
	}
}
void ti::Controller::cursorPosition(GLFWwindow* window, double xposition, double yposition)
{
	mouseData.currentX = xposition;
	mouseData.currentY = yposition;
}
void ti::Controller::cameraMovement(ti::Camera* cam, int state, float deltaSecs, float cameraSpeed)
{
	if (state == ti::CameraMovement::IDLE)
		return;

	if (cam != nullptr)
	{
		if (state & ti::CameraMovement::FORWARD)
		{
			cam->setPosition(cam->getPosition() + (cam->getForward()*deltaSecs*cameraSpeed));
		}

		if (state & ti::CameraMovement::BACKWARD)
		{
			cam->setPosition(cam->getPosition() - (cam->getForward()*deltaSecs*cameraSpeed));
		}

		if (state & ti::CameraMovement::RIGHT)
		{
			cam->setPosition(cam->getPosition() + (cam->getRight()*deltaSecs*cameraSpeed));
		}

		if (state & ti::CameraMovement::LEFT)
		{
			cam->setPosition(cam->getPosition() - (cam->getRight()*deltaSecs*cameraSpeed));
		}
	}
}
void ti::Controller::cameraRotation(ti::Camera* cam)
{
	if (cam != nullptr && mouseData.isRightPressed)
	{
		float dh, dv;
		int w = 1, h = 1;
		cam->getViewPort(w, h);
		dh = (float)mouseData.deltaX / w * PI;
		dv = (float)mouseData.deltaY / h * PI;
		cam->rotateByDelta(dh, dv);
	}
}
void ti::Controller::cursorPositionUpdate()
{
	mouseData.deltaX = mouseData.currentX - mouseData.lastX;
	mouseData.deltaY = mouseData.currentY - mouseData.lastY;
	mouseData.lastX = mouseData.currentX;
	mouseData.lastY = mouseData.currentY;
}