#include "InfiniteShore.h"

ti::InfiniteShore::InfiniteShore(Shader& shader, const Texture& shore, float scale) :
	Object(&shader),
	shoreTexture(shore),
	scale(scale)
{

}

ti::InfiniteShore::~InfiniteShore()
{

}

void ti::InfiniteShore::initialize()
{
	float vertices[] = {
		-scale,  -scale,
		scale,  -scale,
		scale,  scale,
		-scale,  scale,
	};

	unsigned int indices[] = {
		3, 1, 0,
		2, 1, 3
	};

	GLuint vbo;
	GLuint ebo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &infiniteShoreVao);

	glBindVertexArray(infiniteShoreVao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBindVertexArray(0);
}

void ti::InfiniteShore::update()
{

}


void ti::InfiniteShore::renderInternal()
{
	glBindVertexArray(infiniteShoreVao);

	GLuint texid = this->shader_->getUniformLocation("shore_texture");
	glUniform1i(texid, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shoreTexture.texture_id);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}