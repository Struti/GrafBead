#pragma once

#include "Shader.h"

#include <glm/glm.hpp>

namespace ti
{
	/*
		Simple directional light with diffuse and ambient color.
	*/
	class DirectionalLight
	{
	public:

		DirectionalLight();
		~DirectionalLight();

		void setDirection(const glm::vec3& direction);
		void setDiffuseColor(const glm::vec3& color);
		void setAmbientColor(const glm::vec3& color);

		glm::vec3 getDirection() const;
		glm::vec3 getDiffuseColor() const;
		glm::vec3 getAmbientColor() const;

	private:

		glm::vec3 direction_;
		glm::vec3 diffuse_color_;
		glm::vec3 ambient_color_;
	};
}