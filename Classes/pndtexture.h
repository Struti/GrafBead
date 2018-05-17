#ifndef PNDTEXTURE_H
#define PNDTEXTURE_H

#include "GLIncludes.h"

class PNDTEXTURE
{
public:
	unsigned char* data;
	int width;
	int height;

	GLuint textureid;

	PNDTEXTURE(): 
		data(NULL)
	{
	}

	bool loadteximage(const char*);

	void releaseteximage();
};
#endif