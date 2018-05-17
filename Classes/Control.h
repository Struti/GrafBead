#include "Camera.h"

#include "DirectionalLight.h"

#ifndef CONTROL_H
#define CONTROL_H

		
namespace ti {

		static const float PI = 3.141592f;

		struct Mouse
		{
			double lastX;
			double lastY;
			double deltaX;
			double deltaY;
			double currentX;
			double currentY;
			bool isLeftPressed;
			bool isRightPressed;
		};
		
		struct CameraMovement
		{
			static const int IDLE = 0;
			static const int FORWARD = 1;
			static const int BACKWARD = 2;
			static const int RIGHT = 4;
			static const int LEFT = 8;
		};

		struct CameraState
		{
			int movementState = CameraMovement::IDLE;
			float cameraSpeed = 5.0f;
		};

	class Controller {

	public:
		static DirectionalLight* light;
		static CameraState cameraState;
		static Mouse mouseData;

		static void keyMapping(GLFWwindow * window, int key, int scancode, int action, int mods);
		static void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods);
		static void cursorPosition(GLFWwindow * window, double xposition, double yposition);
		static void cameraMovement(ti::Camera* cam, int state, float deltaSecs, float cameraSpeed);
		static void cameraRotation(ti::Camera* cam);
		static void cursorPositionUpdate();

	};
#endif
	extern Controller control;
	extern const float PI;
}
