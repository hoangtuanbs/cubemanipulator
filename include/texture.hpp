#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <unordered_map>

class CubeTexture;

typedef std::unordered_map<std::string, CubeTexture*> TextureMap;

class CubeTexture
{
	GLuint _TextureID;
	std::string _TexturePath;

public:
    static TextureMap s_Textures;

	CubeTexture(const char* texturePath);

	~CubeTexture();

    static CubeTexture* get(const std::string& name);
    static int loadTextures();
    static int cleanUp();

	bool load();

	void apply(GLuint bindTarget);
};

#endif