#pragma once

#include "GLIncludes.h"

#include <glm\glm.hpp>

#include "Object.h"
#include "Shader.h"
#include "TextureLoader.h"

#include <map>

namespace ti
{

	struct RenderPart
	{
		int indexOffset;
		int indexCount;
		int material;
	};

	struct Vertex
	{
		glm::vec3 pos;
		glm::vec2 texCoord;
		glm::vec3 normal;
		glm::vec3 color;
	};

	class Model : public Object
	{
	public:
		Model(Shader* shader, TextureLoader textureLoader, std::string mesh);

		void initialize();
		void update();
		void renderInternal();

	private:
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> texcoords;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> colors;
		std::vector<unsigned int> indices;
		std::vector<ti::Texture> materials;
		std::vector<RenderPart> renderParts;
			
		GLuint vao;
	};
}