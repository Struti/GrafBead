#include "DirectionalLight.h"

ti::DirectionalLight::DirectionalLight()
{

}


ti::DirectionalLight::~DirectionalLight()
{

}
void ti::DirectionalLight::setDirection(const glm::vec3& dir)
{
	direction_ = dir;
}

void ti::DirectionalLight::setDiffuseColor(const glm::vec3& color)
{
	diffuse_color_ = color;
}

void ti::DirectionalLight::setAmbientColor(const glm::vec3& color)
{
	ambient_color_ = color;
}

glm::vec3 ti::DirectionalLight::getDirection() const
{
	return direction_;
}

glm::vec3 ti::DirectionalLight::getDiffuseColor() const
{
	return diffuse_color_;
}

glm::vec3 ti::DirectionalLight::getAmbientColor() const
{
	return ambient_color_;
}