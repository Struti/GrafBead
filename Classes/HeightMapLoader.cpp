#include "HeightMapLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ti::HeightMap::HeightMap(Shader& shader, std::string file, float maxHeight, ti::Texture grass, ti::Texture slope, ti::Texture rock, ti::Texture sand) :
	Object(&shader),
	terr_texture1(grass),
	terr_texture2(slope),
	terr_texture3(rock),
	terr_shore(sand)
{
	initHeightmap(file.c_str(), maxHeight);
}

void ti::HeightMap::loadHeightmap(const char* filename,float heightscale)
{
	int compontent = 0;

	data = stbi_load(filename, &mapwidth, &mapheight, nullptr, 1);

	int k = 0;
	int ind = 0;

	coords.reserve(mapheight*mapwidth*3);
	mapindex.reserve((mapheight-1)*(mapwidth-1)*6);
	for(int row=0;row<mapheight;row++)
	{
		for(int col=0;col<mapwidth;col++)
		{
			float height = data[k] / 255.0f;
	    	coords.push_back((float)col - mapwidth / 2.0f);
			coords.push_back((float)(height*heightscale));
			coords.push_back((float)row - mapheight / 2.0f);

			k++;
		}
	}

	for(int row=0;row<mapheight-1;row++)
	{
		for(int col=0;col<mapwidth-1;col++)
		{
			mapindex.push_back(row*mapwidth+col);
			mapindex.push_back((row+1)*mapwidth+col);
			mapindex.push_back(row*mapwidth+col+1);

			mapindex.push_back(row*mapwidth+col+1);
            mapindex.push_back((row+1)*mapwidth+col);
			mapindex.push_back((row+1)*mapwidth+col+1);
		}
	}
}

void ti::HeightMap::initHeightmap(const char*filename,float heightscale){

	loadHeightmap(filename,heightscale);
	getVertexNormal();

	glGenBuffers(1,&mapCoordinateBufferID);
	glGenBuffers(1,&mapIndexBufferID);
	glGenBuffers(1,&mapNormalBufferID);

	glGenVertexArrays(1,&terrainVertexArrayBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mapIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLuint)*mapindex.size(),&mapindex[0],GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,mapCoordinateBufferID);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*coords.size(),&coords[0],GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,mapNormalBufferID);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*mapnormal.size(),&mapnormal[0],GL_STATIC_DRAW);

	glBindVertexArray(terrainVertexArrayBuffer);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER,mapCoordinateBufferID);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

	glBindBuffer(GL_ARRAY_BUFFER,mapNormalBufferID);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mapIndexBufferID);

	glBindVertexArray(0);
}

void ti::HeightMap::renderHeightmap()
{
	glBindVertexArray(terrainVertexArrayBuffer);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terr_texture1.texture_id);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terr_texture2.texture_id);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, terr_texture3.texture_id);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, terr_shore.texture_id);

	GLuint tex0 = this->shader_->getUniformLocation("terr_texture1");
	GLuint tex1 = this->shader_->getUniformLocation("terr_texture2");
	GLuint tex2 = this->shader_->getUniformLocation("terr_texture3");
	GLuint tex3 = this->shader_->getUniformLocation("terr_tex_shore");

	glUniform1i(tex0, 0);
	glUniform1i(tex1, 1);
	glUniform1i(tex2, 2);
	glUniform1i(tex3, 3);

	glBindVertexArray(terrainVertexArrayBuffer);

	glDrawElements(GL_TRIANGLES, mapindex.size(), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(GL_NONE);
}

glm::vec3 ti::HeightMap::getCoordData(int c_x, int c_z)
{
	glm::vec3 coord;
	coord.x=c_x;
	coord.z=c_z;
	coord.y=(float)data[c_x+(int)mapwidth*c_z];
	return coord;
}
void ti::HeightMap::initialize()
{
}

void ti::HeightMap::update()
{

}

void ti::HeightMap::renderInternal()
{
	renderHeightmap();
}


void ti::HeightMap::getVertexNormal()
{
	glm::vec3 vec[9];
	glm::vec3 vNormalCoords[8];
	glm::vec3 FinalNormal(0.0f);

	for(int z=0; z<mapwidth;z++)
	{
		mapnormal.push_back(0);
	    mapnormal.push_back(1);
		mapnormal.push_back(0);
	}

	for(int z=1; z<mapheight-1;z++)
	{
		mapnormal.push_back(0);
	    mapnormal.push_back(1);
	    mapnormal.push_back(0);

		for(int x=1; x<mapwidth-1;x++)
		{
			vec[0]=getCoordData(x,z);

			vec[1]=getCoordData(x+1,z);
			vec[2]=getCoordData(x-1,z);
			vec[3]=getCoordData(x,z-1);
			vec[4]=getCoordData(x,z+1);
			vec[5]=getCoordData(x+1,z-1);
			vec[6]=getCoordData(x-1,z+1);
			vec[7]=getCoordData(x+1,z+1);
			vec[8]=getCoordData(x-1,z-1);

			vNormalCoords[0]=glm::cross(glm::normalize(vec[1]-vec[0]),glm::normalize(vec[5]-vec[0]));
			vNormalCoords[1]=glm::cross(glm::normalize(vec[4]-vec[0]),glm::normalize(vec[5]-vec[0]));
			vNormalCoords[2]=glm::cross(glm::normalize(vec[8]-vec[0]),glm::normalize(vec[4]-vec[0]));
			vNormalCoords[3]=glm::cross(glm::normalize(vec[8]-vec[0]),glm::normalize(vec[2]-vec[0]));
			vNormalCoords[4]=glm::cross(glm::normalize(vec[2]-vec[0]),glm::normalize(vec[6]-vec[0]));
			vNormalCoords[5]=glm::cross(glm::normalize(vec[3]-vec[0]),glm::normalize(vec[6]-vec[0]));
			vNormalCoords[6]=glm::cross(glm::normalize(vec[7]-vec[0]),glm::normalize(vec[3]-vec[0]));
			vNormalCoords[7]=glm::cross(glm::normalize(vec[7]-vec[0]),glm::normalize(vec[1]-vec[0]));

			FinalNormal =vNormalCoords[0];
			FinalNormal+=vNormalCoords[1];
			FinalNormal+=vNormalCoords[2];
			FinalNormal+=vNormalCoords[3];
			FinalNormal+=vNormalCoords[4];
			FinalNormal+=vNormalCoords[5];
			FinalNormal+=vNormalCoords[6];
			FinalNormal+=vNormalCoords[7];

			FinalNormal=glm::normalize(FinalNormal);

			mapnormal.push_back(FinalNormal.x);
			mapnormal.push_back(FinalNormal.y);
			mapnormal.push_back(FinalNormal.z);
		}
		
		mapnormal.push_back(0);
	    mapnormal.push_back(1);
	    mapnormal.push_back(0);
	}

	for(int z=0; z<mapwidth;z++)
	{
		mapnormal.push_back(0);
	    mapnormal.push_back(1);
		mapnormal.push_back(0);
	}
}