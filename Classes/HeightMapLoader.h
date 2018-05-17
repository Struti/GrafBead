#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <iostream>
#include <vector>

#include "GLIncludes.h"
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>

#include "TextureLoader.h"
#include "Object.h"

using namespace std;

namespace ti 
{
	class HeightMap : public Object
	{
	public:

		HeightMap(Shader& shader, std::string file, float maxHeight, ti::Texture grass, ti::Texture slope, ti::Texture rock, ti::Texture sand);

		GLuint
			terrainVertexArrayBuffer,
			mapCoordinateBufferID,
			mapIndexBufferID,
			mapNormalBufferID;

		void loadHeightmap(const char*, float);

		void initHeightmap(const char*, float);
		void renderHeightmap();

		void getVertexNormal();

		std::vector<float> coords;
		std::vector<unsigned int> mapindex;
		std::vector<float> mapnormal;

		ti::Texture terr_texture1;
		ti::Texture terr_texture2;
		ti::Texture terr_texture3;
		ti::Texture terr_shore;

		int mapwidth;
		int mapheight;
		long image_size;
		unsigned char* data;

		glm::vec3 getCoordData(int, int);

		void initialize();
		void update();		
		void renderInternal();
	};
#endif
}