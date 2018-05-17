#pragma once

#include "GLIncludes.h"

#include "Object.h"
#include "Shader.h"
#include "TextureLoader.h"

#include <string>

namespace ti
{
	class InfiniteShore : public Object
	{
	public:
		InfiniteShore(Shader& shader, const Texture& shore, float scale);

		~InfiniteShore();

		void initialize();
		void update();
		void renderInternal();

		GLuint infiniteShoreVao;
		Texture shoreTexture;

		float scale;

	};
}