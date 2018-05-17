#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "GLIncludes.h"

namespace ti 
{
	enum class ShaderType : unsigned int
	{
		VERTEX,
		FRAGMENT,
		TESSCONT,
		TESSEV,
		GEOMETRY
	};

	int operator+ (ShaderType val);

	class Shader
	{
	public:

		Shader();

		~Shader();

		/*
			Loads the shaders using the file paths given and builds a program object.
			Tessellation shaders are not necessary.
		*/
		void load(
			const std::string& vertexShaderFile,
			const std::string& fragmentShaderFile,
			const char* tessControlShader,
			const char* tessEvShader);

		void use();
		static void useNone();

		GLuint getUniformLocation(const std::string& uniform);

		GLuint getProgram();
		bool isLoaded();

	private:

		GLuint program_;
		GLuint shaders_[5];

		static std::string loadFile(const std::string& path);
		static int getCompilationStatus(GLuint shader);
	};


}