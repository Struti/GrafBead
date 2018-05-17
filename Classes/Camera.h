#pragma once

#include "GLIncludes.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>

#include <string>

namespace ti
{
	class Camera
	{
	public:
		Camera();
		Camera(std::string name);
		~Camera();

		void update(int width, int height);

		void getViewPort(int& width, int& height) const;
		void getParameters(float & fov, float & near, float & far) const;
		glm::vec3 getPosition() const;
		glm::vec3 getForward() const;
		glm::vec3 getRight() const;

		glm::mat4 getViewMatrix() const;
		glm::mat4 getProjectionMatrix() const;

		void setParameters(float fov, float near, float far);
		void setPosition(const glm::vec3& position);
		void lookAt(const glm::vec3& target);
		void rotateByDelta(float horizontal, float vertical);

	private:

		const std::string name;

		int width;
		int height;

		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 up;

		float fov;
		float near_;
		float far_;

	};

}
