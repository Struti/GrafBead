#include "Shader.h"

#include <vector>

int ti::operator+ (ti::ShaderType val)
{
	return static_cast<int>(val);
}

ti::Shader::Shader() :
	program_(GL_NONE)
{
	std::fill_n(shaders_, sizeof(shaders_) / sizeof(*shaders_), 0);
}

ti::Shader::~Shader()
{
	if (program_ != 0)
	{
		glDeleteProgram(program_);
	}

	for (int i = 0; i < sizeof(shaders_) / sizeof(*shaders_); i++)
	{
		if (shaders_[i] != 0) glDeleteShader(shaders_[i]);
	}
};

void ti::Shader::load(
	const std::string & vertexShaderFile,
	const std::string & fragmentShaderFile,
	const char * tessControlShaderFile,
	const char * tessEvShaderFile)
{
	if (program_ != GL_NONE) return;

	std::string vss = loadFile(vertexShaderFile);
	std::string fss = loadFile(fragmentShaderFile);
	std::string tess;
	std::string tcss;

	const char* vertexShaderSource = vss.c_str();
	const char* fragmentShaderSource = fss.c_str();
	const char* tessControlShaderSource = nullptr;
	const char* tessEvShaderSource = nullptr;

	if (tessControlShaderFile != nullptr && tessEvShaderFile != nullptr)
	{
		tess = loadFile(std::string(tessControlShaderFile));
		tcss = loadFile(std::string(tessEvShaderFile));
		tessControlShaderSource = tess.c_str();
		tessEvShaderSource = tcss.c_str();
	};

	program_ = glCreateProgram();

	shaders_[+ShaderType::VERTEX] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shaders_[+ShaderType::VERTEX], 1, &vertexShaderSource, NULL);
	glCompileShader(shaders_[+ShaderType::VERTEX]);
	if (getCompilationStatus(shaders_[+ShaderType::VERTEX]) != 0)
	{
		glDeleteShader(shaders_[+ShaderType::VERTEX]);
		shaders_[+ShaderType::VERTEX] = 0;
		return;
	}

	shaders_[+ShaderType::FRAGMENT] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shaders_[+ShaderType::FRAGMENT], 1, &fragmentShaderSource, NULL);
	glCompileShader(shaders_[+ShaderType::FRAGMENT]);
	if (getCompilationStatus(shaders_[+ShaderType::FRAGMENT]) != 0)
	{
		glDeleteShader(shaders_[+ShaderType::FRAGMENT]);
		shaders_[+ShaderType::FRAGMENT] = 0;
		return;
	}

	if (tessControlShaderSource != nullptr && tessEvShaderSource != nullptr)
	{
		shaders_[+ShaderType::TESSCONT] = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(shaders_[+ShaderType::TESSCONT], 1, &tessControlShaderSource, NULL);
		glCompileShader(shaders_[+ShaderType::FRAGMENT]);
		if (getCompilationStatus(shaders_[+ShaderType::TESSCONT]) != 0)
		{
			glDeleteShader(shaders_[+ShaderType::TESSCONT]);
			shaders_[+ShaderType::TESSCONT] = 0;
			return;
		}

		shaders_[+ShaderType::TESSEV] = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(shaders_[+ShaderType::TESSEV], 1, &tessEvShaderSource, NULL);
		glCompileShader(shaders_[+ShaderType::TESSEV]);
		if (getCompilationStatus(shaders_[+ShaderType::TESSEV]) != 0)
		{
			glDeleteShader(shaders_[+ShaderType::TESSEV]);
			shaders_[+ShaderType::TESSEV] = 0;
			return;
		}
	}

	for (int i = 0; i < sizeof(shaders_) / sizeof(*shaders_); i++)
	{
		if (shaders_[i] != 0) glAttachShader(program_, shaders_[i]);
	}

	glLinkProgram(program_); 
	
	int isLinked;
	glGetProgramiv(program_, GL_LINK_STATUS, &isLinked); 
	
	GLint maxLength = 0;
	glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &maxLength);

	if (maxLength > 0) 
	{
		std::vector<GLchar> log(maxLength);
		glGetProgramInfoLog(program_, maxLength, &maxLength, &log[0]);

		std::cout << "Program linking log:\n" << std::string(log.data(), maxLength) << "\n";
	}

	if (isLinked == GL_FALSE)
	{
		glDeleteProgram(program_); 
		program_ = GL_NONE;
	}
};

void ti::Shader::use()
{
	glUseProgram(program_);
};

GLuint ti::Shader::getProgram() { return program_; }

bool ti::Shader::isLoaded() { return program_ != GL_NONE; }

std::string ti::Shader::loadFile(const std::string & path)
{
	std::ifstream t(path);

	if (t)
	{
		std::cout << "Reading file " << path << "\n";
	}
	else
	{
		std::cout << "Error reading file " << path << "\n";
	}

	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	return str;
}

int ti::Shader::getCompilationStatus(GLuint shader)
{
	GLint isCompiled = 0;

	GLint maxLength = 0;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
	if (maxLength > 0)
	{
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
		std::cout << "Shader info:\n" << std::string(infoLog.data(), maxLength) << "\n";
	}

	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		return -1;
	}

	return 0;
}

void ti::Shader::useNone()
{
	glUseProgram(GL_NONE);
}

GLuint ti::Shader::getUniformLocation(const std::string & uniform)
{
	return glGetUniformLocation(program_, uniform.c_str());
}
