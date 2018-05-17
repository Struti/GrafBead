#include "Water.h"
#include <iostream>
#include <fstream>

#include "TimeCalculator.h"

ti::Water::Water(Shader& shader, int w, int h, ti::Texture normal_map, ti::Texture foam) :
	Object(&shader),
	width(w),
	height(h),
	normal_map(normal_map),
	foam(foam)
{
}

void ti::Water::loadWater()
{
	watercoords.clear();
	waterindex.clear();

	watercoords.reserve((height + 1)*(width + 1) * 2);
	waterindex.reserve(height*width * 6);

	for (int x = 0; x < height + 1; x++)
	{
		for (int y = 0; y < width + 1; y++)
		{
			watercoords.push_back(x - height / 2.0f);
			watercoords.push_back(y - width / 2.0f);
		}
	}

	for(int i=0;i < height;i++)
	{
		for(int j=0;j < width;j++)
		{
			waterindex.push_back(i*(width + 1)+j);
			waterindex.push_back(i*(width + 1) +j+1);
			waterindex.push_back((i + 1)*(width + 1) + j);

			waterindex.push_back(i*(width + 1) +j+1);
			waterindex.push_back((i+1)*(width + 1) +j+1);
			waterindex.push_back((i + 1)*(width + 1) + j);
		}
	}

};

void ti::Water::init_water()
{
	waterlevel=10.0f;
	loadWater();

	glGenBuffers(1, &waterIndexBufferID);
	glGenBuffers(1,&waterCoordBufferID);

	glGenVertexArrays(1, &waterVertArrayBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waterIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*waterindex.size(),&waterindex[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, waterCoordBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*watercoords.size(),&watercoords[0], GL_STATIC_DRAW);

	glBindVertexArray(waterVertArrayBuffer);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, waterCoordBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waterIndexBufferID);
	
	glBindVertexArray(0);
}

void ti::Water::render_water()
{
	glUniform1f(shader_->getUniformLocation("water_level"), waterlevel);
	glUniform1f(shader_->getUniformLocation("d_time"), CalculateTime::getElapsedTime());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterReflection.fbo_texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, waterRefraction.fbo_texture);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, normal_map.texture_id);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skytexture);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, foam.texture_id);

	glUniform1i(shader_->getUniformLocation("reflection_texture"), 0);
	glUniform1i(shader_->getUniformLocation("refraction_texture"), 1);
	glUniform1i(shader_->getUniformLocation("bump_map"), 2);
	glUniform1i(shader_->getUniformLocation("skybox_texture"), 3);
	glUniform1i(shader_->getUniformLocation("foam"), 4);

    glBindVertexArray(waterVertArrayBuffer);
	
	glDrawElements(GL_TRIANGLES, waterindex.size(), GL_UNSIGNED_INT, nullptr);
};

void ti::Water::incWaterHeight()
{
	waterlevel+=0.05;
}

void ti::Water::initialize()
{
	waterReflection.createFBO(1024, 1024);
	waterGlobalReflection.createFBO(1024, 1024);
	waterRefraction.createFBO(1024, 1024);

	init_water();
}
void ti::Water::update()
{
}
void ti::Water::renderInternal()
{
	render_water();
}

void ti::Water::decrWaterHeight()
{
	waterlevel-=0.05;
};