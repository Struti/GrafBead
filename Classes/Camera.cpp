#include "Camera.h"

static float EPSILON = 0.0001f;

ti::Camera::Camera() :
	Camera("")
{
}

ti::Camera::Camera(std::string name) :
	name(name),
	position(0, 0, 0),
	forward(0, 0, 1),
	up(0, 1, 0)
{
};


ti::Camera::~Camera()
{

};

void ti::Camera::update(int width, int height)
{
	glViewport(0, 0, width, height);
	this->width = width;
	this->height = height;
};

glm::mat4 ti::Camera::getViewMatrix() const
{
	return glm::lookAt(position, position + forward, up);
};

glm::mat4 ti::Camera::getProjectionMatrix() const
{
	return glm::perspective(glm::radians(fov), (float)width / height, near_, far_);
};

void ti::Camera::getViewPort(int& width, int& height) const
{
	height = this->height;
	width = this->width;
};

void ti::Camera::getParameters(float& fov, float& near_, float& far_) const
{
	fov = this->fov;
	near_ = this->near_;
	far_ = this->far_;
};

void ti::Camera::setParameters(float fov, float near_, float far_)
{
	this->fov = fov;
	this->near_ = near_;
	this->far_ = far_;
}

glm::vec3 ti::Camera::getPosition() const
{
	return this->position;
}

glm::vec3 ti::Camera::getForward() const
{
	return this->forward;
}

glm::vec3 ti::Camera::getRight() const
{
	return 	glm::normalize(glm::cross(forward, up));
}

void ti::Camera::setPosition(const glm::vec3& position)
{
	this->position = position;
}

void ti::Camera::lookAt(const glm::vec3& target)
{
	glm::vec3 forw = target - this->position;
	if (glm::length(forw) > EPSILON)
	{
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->forward = glm::normalize(forw);
	}
}

void ti::Camera::rotateByDelta(float horizontal, float vertical)
{
	glm::vec3 right = getRight();

	glm::mat4 hrm = glm::rotate(glm::mat4(), horizontal, glm::vec3(0, 1, 0));
	glm::mat4 vrm = glm::rotate(glm::mat4(), vertical, right);

	forward = vrm*hrm*glm::vec4(forward, 1);
	right = hrm*glm::vec4(right, 1);
	up = glm::normalize(glm::cross(right, forward));
}