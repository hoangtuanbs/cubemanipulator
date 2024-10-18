#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.hpp"

class CubeShader;
typedef std::unordered_map<std::string, CubeShader*> ShaderMap;

class CubeShader
{
	GLuint _ShaderID;

public:
	static ShaderMap s_Shaders;

	CubeShader(const char* vertex_shader, const char* fragment_shader);

	~CubeShader();

	static int loadShaders();
    static int cleanUp();

	static CubeShader* get(const std::string& name);

	void use();

	void set(const std::string& name, int value) const;

	void set(const std::string& name, const glm::mat4& matrix) const;

	void applyTexture(CubeTexture& cube);
};

#endif