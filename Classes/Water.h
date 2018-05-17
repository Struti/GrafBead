#pragma once

#define MAX_FRAMES 32

#include <Windows.h>
#include <iostream>
#include <vector>
#include <math.h>

#include "GLIncludes.h"
#include "fbomanager.h"

#include "TextureLoader.h"
#include "Shader.h"

#include "Object.h"

namespace ti {
	class Water : public Object
	{
	public:

		Water(Shader& shader, int w, int h, ti::Texture normal_map, ti::Texture foam);

		int width;
		int height;

		std::vector<float> watercoords;
		std::vector<unsigned int> waterindex;
		std::vector<float> waterdepth;

		void loadWater();
		void init_water();
		void render_water();

		ti::Texture normal_map;
		ti::Texture foam;

		GLuint waterCoordBufferID;
		GLuint waterIndexBufferID;
		GLuint waterDepthBufferID;
		GLuint waterVertArrayBuffer;
		GLuint skytexture;

		FboManager waterReflection;
		FboManager waterGlobalReflection;
		FboManager waterRefraction;

		float waterlevel;
		void decrWaterHeight();
		void incWaterHeight();

		void initialize();
		void update();
		void renderInternal();
	};

}