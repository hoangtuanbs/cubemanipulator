
#ifndef __MODEL_H__
#define __MODEL_H__

// Include GLEW
#include <GL/glew.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader.hpp"

// Abstract model class
class Model
{

public:
    ~Model();

    virtual void handleInput(int keyCode) = 0;
    virtual void handleScroll(double xoffset, double yoffset) = 0;

    virtual void draw(const glm::mat4& projection, const glm::mat4& view) = 0;

protected:
    GLuint _VertexBuffer;
    GLuint _ColorBuffer;
};

// Cube models
class CubeModel: public Model
{
public:
    CubeModel();

    void loadShaders();

    void handleInput(int keyCode) override;
    void handleScroll(double xoffset, double yoffset) override;

    void draw( const glm::mat4& projection, const glm::mat4& view) override;

    void changeColor(int index);

private:

    float _RotationX{0.0f};
    float _RotationY{0.0f};
    float _RotationDelta{1.0f};

    bool _AutoRotationMode{false};
    bool _AutoRotationCws{false};
    bool _DebugMode{false};

    bool _TextureMode{false};
    
    CubeShader *_TextureShader;
    CubeShader *_ColorShader;

    glm::mat4 _Model;
};

#endif