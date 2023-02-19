#include "ProgramShader.h"
#include<fstream>
#include<iostream>
#include<sstream>

ProgramShader::ProgramShader()
	:mHandle(0)
{
}

ProgramShader::~ProgramShader()
{
	glDeleteProgram(mHandle);
}

bool ProgramShader::loadShaders(const char* vsFilename, const char* fsFilename)
{
	string vsString = fileToString(vsFilename);
	string fsString = fileToString(fsFilename);
	const GLchar* vsSourcePtr = vsString.c_str();
	const GLchar* fsSourcePtr = fsString.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vsSourcePtr, NULL);
	glShaderSource(fs, 1, &fsSourcePtr, NULL);

	glCompileShader(vs);
	checkCompileError(vs, VERTEX);

	glCompileShader(fs);
	checkCompileError(fs, FRAGMENT);


	mHandle = glCreateProgram();
	glAttachShader(mHandle, vs);
	glAttachShader(mHandle, fs);
	glLinkProgram(mHandle);

	checkCompileError(mHandle, PROGRAM);

	glDeleteShader(vs);
	glDeleteShader(fs);
	mUniformLocations.clear();
	return true;
}

void ProgramShader::use()
{
	if (mHandle > 0)
		glUseProgram(mHandle);
}

string ProgramShader::fileToString(const string& filename)
{
	std::stringstream ss;
	std::ifstream file;
	
	try
	{
		file.open(filename, std::ios::in);
		if (!file.fail())
		{
			ss << file.rdbuf();
		}
		file.close();
	}
	catch(std::exception ex)
	{
		std::cerr << "Error reading shader filname!" << std::endl;
	}
	return ss.str();
}

void ProgramShader::checkCompileError(GLuint shader, ShaderType type)
{
	int status = 0;
	if (type == PROGRAM)
	{
		glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);
			string errorLog(length, ' ');
			glGetProgramInfoLog(mHandle, length, &length, &errorLog[0]);
			std::cerr << "Error! program failed to link. " << errorLog << std::endl;
		}
	}
	else
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			string errorLog(length, ' ');
			glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
			std::cerr << "Error! shader failed to compile. " << errorLog << std::endl;
		}
	}
}

GLint ProgramShader::getUniformLocation(const GLchar* name)
{
	std::map<string, GLint>::iterator it = mUniformLocations.find(name);
	if (it == mUniformLocations.end())
	{
		mUniformLocations[name] = glGetUniformLocation(mHandle, name);
	}
	return mUniformLocations[name];
}

void ProgramShader::setUniform(const GLchar* name, const glm::vec2& v)
{
	GLint loc = getUniformLocation(name);
	glUniform2f(loc, v.x,v.y);
}

void ProgramShader::setUniform(const GLchar* name, const glm::vec3& v)
{
	GLint loc = getUniformLocation(name);
	glUniform3f(loc, v.x, v.y,v.z);
}

void ProgramShader::setUniform(const GLchar* name, const glm::vec4& v)
{
	GLint loc = getUniformLocation(name);
	glUniform4f(loc, v.x, v.y,v.z,v.w);
}
