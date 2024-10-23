#include "texture.hpp"

// Include GLEW
#include <GL/glew.h>

// STB Image 
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TextureMap CubeTexture::s_Textures;

/*** Texture */
namespace
{
GLuint loadTexture(char const* filePath)
{
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, bpp;
    stbi_uc* data = stbi_load(filePath, &width, &height, &bpp, 0);
    if (data)
    {
        GLenum format;
        switch (bpp)
        {
            case 1: format = GL_RED; break;
            case 3: format = GL_RGB; break;
            case 4: format = GL_RGBA; break;
            default: format = 0;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

    }
    else
    {
        std::cout << "Failed to load texture:" << std::endl << filePath << std::endl;
    }

    stbi_image_free(data);

    return textureID;
}

};
/*** Static methods */
CubeTexture* CubeTexture::get(const std::string& name)
{
    TextureMap::iterator iter = s_Textures.find(name);
    if (iter != s_Textures.end())
    {
        return iter->second;
    }

    return nullptr;
}

int CubeTexture::loadTextures()
{
    CubeTexture* brick = new CubeTexture("resources/textures/brickwall.jpg");

    if (brick->load())
    {
        s_Textures["brick"] = brick;
    }

    return s_Textures.size();
}
int CubeTexture::cleanUp()
{
    for (TextureMap::iterator iter = s_Textures.begin();
        iter != s_Textures.end();
        iter++)
    {
        auto shp = iter->second;
        delete shp;    
    }

    s_Textures.clear();

    return s_Textures.size();
}

/*** Class methods */

CubeTexture::CubeTexture(const char* texturePath):
    _TexturePath(texturePath)
{
    load();
}

CubeTexture::~CubeTexture()
{
    glDeleteTextures(1, &_TextureID);
}

bool CubeTexture::load()
{
    _TextureID = loadTexture(_TexturePath.c_str());

    if (_TextureID < 0)
    {
        std::cout << "Unable to load texture: " << _TexturePath << std::endl;

        return false;
    }

    return true;
}

void CubeTexture::apply(GLuint bindTarget)
{
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _TextureID);

    glUniform1i(glGetUniformLocation(bindTarget, "Sampler"), 0);
}

/** */