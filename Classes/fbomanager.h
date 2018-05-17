#include <stdio.h>

#include <GL/glew.h>

#ifndef PND_FBO_H
#define PND_FBO_H

class FboManager
{
public:

	bool createFBO(GLuint,GLuint);
	void reshapeFBO(unsigned int width,unsigned int height);
	void bindFBO();
	static void unbindFBO();
	GLuint fbo_texture;
	GLuint width;
	GLuint height;
private:
	GLuint FBO;
	GLuint RBO;
};

#endif