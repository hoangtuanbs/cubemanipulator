#include "shader.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

/** Shaders */
ShaderMap CubeShader::s_Shaders;

GLuint LoadShaders(const char * vertextPath,const char * fragmentPath)
{
	// Create the shaders
	GLuint vsId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsId = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string vsCode;
	std::ifstream vsStream(vertextPath, std::ios::in);
	if (vsStream.is_open())
    {
		std::stringstream sstr;
		sstr << vsStream.rdbuf();
		vsCode = sstr.str();
		vsStream.close();
	}
    else
    {
		std::cout << "Unable to load " << vertextPath << ". Are you in the right directory ? " << std::endl;
		return 0;
	}

	// Read the Fragment Shader code from the file fragmentPath
	std::string fsCode;
	std::ifstream fsStream(fragmentPath, std::ios::in);
	if (fsStream.is_open())
    {
		std::stringstream sstr;
		sstr << fsStream.rdbuf();
		fsCode = sstr.str();
		fsStream.close();
	}

	GLint compileStatus = GL_FALSE;
	int infoLogLength;

	// Compile Vertex Shader
	std::cout << "Compiling vertex shader: " << vertextPath << std::endl;
	char const * vsPointer = vsCode.c_str();
	glShaderSource(vsId, 1, &vsPointer , NULL);
	glCompileShader(vsId);

	// Check Vertex Shader
	glGetShaderiv(vsId, GL_COMPILE_STATUS, &compileStatus);
	glGetShaderiv(vsId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
    {
		std::vector<char> vsError(infoLogLength+1);
		glGetShaderInfoLog(vsId, infoLogLength, NULL, &vsError[0]);

        std::cout << "Error: " << &vsError[0] << std::endl;
	}

	// Compile Fragment Shader
	std::cout << "Compiling shader: " << fragmentPath << std::endl;
	char const * fsPointer = fsCode.c_str();
	glShaderSource(fsId, 1, &fsPointer , NULL);
	glCompileShader(fsId);

	// Check Fragment Shader
	glGetShaderiv(fsId, GL_COMPILE_STATUS, &compileStatus);
	glGetShaderiv(fsId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
    {
		std::vector<char> fsError(infoLogLength+1);
		glGetShaderInfoLog(fsId, infoLogLength, NULL, &fsError[0]);

		std::cout << "Error: " << &fsError[0] << std::endl;
	}

	// Link the program
	GLuint shaderId = glCreateProgram();
	glAttachShader(shaderId, vsId);
	glAttachShader(shaderId, fsId);
	glLinkProgram(shaderId);

	// Check the program
	glGetProgramiv(shaderId, GL_LINK_STATUS, &compileStatus);
	glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
    {
		std::vector<char> linkingError(infoLogLength+1);
		glGetProgramInfoLog(shaderId, infoLogLength, NULL, &linkingError[0]);

        std::cout << "Linking error: " << &linkingError[0] << std::endl;
	}

	glDetachShader(shaderId, vsId);
	glDetachShader(shaderId, fsId);

	glDeleteShader(vsId);
	glDeleteShader(fsId);

	return shaderId;
}

CubeShader* CubeShader::get(const std::string& name)
{
    ShaderMap::iterator iter = s_Shaders.find(name);
    if (iter != s_Shaders.end())
    {
        return iter->second;
    }

    return nullptr;
}

int CubeShader::loadShaders()
{
    CubeShader* ns = new CubeShader("resources/shaders/normalmapping.vs", "resources/shaders/normalmapping.fs");
    s_Shaders["normal"] = ns;

    CubeShader* ts = new CubeShader("resources/shaders/texturemapping.vs", "resources/shaders/texturemapping.fs");
    s_Shaders["texture"] = ts;

    return s_Shaders.size();
}

int CubeShader::cleanUp()
{
    glUseProgram(0);

    for (ShaderMap::iterator iter = s_Shaders.begin();
        iter != s_Shaders.end();
        iter++)
    {
        auto shp = iter->second;
        delete shp;    
    }

    s_Shaders.clear();

    return s_Shaders.size();
}

CubeShader::CubeShader(const char* vertex_shader, const char* fragment_shader)
{
    _ShaderID = LoadShaders(vertex_shader, fragment_shader);
}

CubeShader::~CubeShader()
{
    glDeleteProgram(_ShaderID);
}

void CubeShader::use()
{
    glUseProgram(_ShaderID);
}

void CubeShader::set(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(_ShaderID, name.c_str()), value);
}

void CubeShader::set(const std::string& name, const glm::mat4& matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(_ShaderID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void CubeShader::applyTexture(CubeTexture& cube)
{
    cube.apply(_ShaderID);
}

/***----------------------------- */