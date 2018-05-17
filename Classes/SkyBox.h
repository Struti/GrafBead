#pragma once

#include "GLIncludes.h"

#include "Shader.h"
#include "TextureLoader.h"

#include <string>

namespace ti
{
	class Camera;
	class DirectionalLight;

	class SkyBox
	{
	public:
		SkyBox(const Shader& skyshader, const Texture& skybox);
		~SkyBox();

		void renderSkybox(const Camera& camera);
		Texture skybox;

	private:

		void loadSkybox();
		GLuint cube_vao_;
		Shader skybox_shader_;

	};
} 