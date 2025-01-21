#ifndef  SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "GL/glew.h"
#include<string>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include<map>
using std::string;


class ProgramShader
{
public:

	ProgramShader();
	~ProgramShader();
	enum ShaderType
	{
		VERTEX,
		FRAGMENT,
		PROGRAM
	};
	bool loadShaders(const char* vsFilename, const char* fsFilename);
	void use();

	void setUniform(const GLchar* name, const glm::vec2& v);
	void setUniform(const GLchar* name, const glm::vec3& v);
	void setUniform(const GLchar* name, const glm::vec4& v);
	void setUniform(const GLchar* name, const glm::mat4& m);

	GLuint getProgram() const;

private:

	string fileToString(const string& filename);
	void checkCompileError(GLuint shader, ShaderType type);
	GLint getUniformLocation(const GLchar* name);
	
	GLuint mHandle;
	std::map<string, GLint> mUniformLocations;
};

#endif // ! SHADER_PROGRAM_H

