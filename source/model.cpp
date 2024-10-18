#include "model.hpp"

#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

namespace 
{
    constexpr GLfloat g_VertexBufferData[] =
    { 
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,

		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,

		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,

		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,

		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,

		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
	};

	// Two UV coordinatesfor each vertex. They were created with Blender.
	constexpr GLfloat g_UVBufferData[] =
    { 
		0.000059f, 1.0f-0.000004f, 
		0.000103f, 1.0f-0.336048f, 
		0.335973f, 1.0f-0.335903f, 
		1.000023f, 1.0f-0.000013f, 
		0.667979f, 1.0f-0.335851f, 
		0.999958f, 1.0f-0.336064f, 

		0.667979f, 1.0f-0.335851f, 
		0.336024f, 1.0f-0.671877f, 
		0.667969f, 1.0f-0.671889f, 
		1.000023f, 1.0f-0.000013f, 
		0.668104f, 1.0f-0.000013f, 
		0.667979f, 1.0f-0.335851f, 

		0.000059f, 1.0f-0.000004f, 
		0.335973f, 1.0f-0.335903f, 
		0.336098f, 1.0f-0.000071f, 
		0.667979f, 1.0f-0.335851f, 
		0.335973f, 1.0f-0.335903f, 
		0.336024f, 1.0f-0.671877f, 

		1.000004f, 1.0f-0.671847f, 
		0.999958f, 1.0f-0.336064f, 
		0.667979f, 1.0f-0.335851f, 
		0.668104f, 1.0f-0.000013f, 
		0.335973f, 1.0f-0.335903f, 
		0.667979f, 1.0f-0.335851f, 

		0.335973f, 1.0f-0.335903f, 
		0.668104f, 1.0f-0.000013f, 
		0.336098f, 1.0f-0.000071f, 
		0.000103f, 1.0f-0.336048f, 
		0.000004f, 1.0f-0.671870f, 
		0.336024f, 1.0f-0.671877f, 

		0.000103f, 1.0f-0.336048f, 
		0.336024f, 1.0f-0.671877f, 
		0.335973f, 1.0f-0.335903f, 
		0.667969f, 1.0f-0.671889f, 
		1.000004f, 1.0f-0.671847f, 
		0.667979f, 1.0f-0.335851f
	};

    std::pair<GLfloat, GLfloat> RandomUV()
    {
        int index = rand() % (sizeof(g_UVBufferData) / (2*sizeof(GLfloat)));
        return std::pair<GLfloat, GLfloat>(g_UVBufferData[index*2], g_UVBufferData[index*2+1]);
    }

    GLintptr offset{0};
};

Model::~Model()
{
	glDeleteBuffers(1, &_VertexBuffer);
	glDeleteBuffers(1, &_ColorBuffer);
}	

CubeModel::CubeModel():
    Model()
{
    loadShaders();
};

void CubeModel::changeColor(int index)
{
    if (_TextureMode)
    {
        return;
    }
    static double lastTime = 0;
    double currentTime = glfwGetTime();

    if (currentTime - lastTime < 2.0f)
    {
        return;
    }

    // Index to cube vertices map
    static std::pair<GLuint,GLuint> s_Map[] = {{0, 24}, {6, 18}, {12, 30}, {36, 66}, {42,48}, {60, 54}};
    GLuint buffer = _ColorBuffer;
    index = index % 6;

    const std::pair<GLuint,GLuint> &map = s_Map[index];

    constexpr int s_DataSize{6};
    GLfloat buffer_data[s_DataSize];

    std::pair<GLfloat, GLfloat> color = RandomUV();
    for (int i = 0; i < s_DataSize/2; i++)
    {
        buffer_data[2*i] = color.first;
        buffer_data[2*i+1] = color.second;
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    offset = map.first;
    glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(GLfloat), sizeof(buffer_data), &buffer_data);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    offset = map.second;
    glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(GLfloat), sizeof(buffer_data), &buffer_data);
}

void CubeModel::loadShaders()
{
    _TextureShader = CubeShader::get("texture");
    _ColorShader = CubeShader::get("normal");

	glGenBuffers(1, &_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_VertexBufferData), g_VertexBufferData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

	glGenBuffers(1, &_ColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _ColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_UVBufferData), g_UVBufferData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
}

void CubeModel::handleInput(int keyCode)
{
    switch (keyCode)
    {
        case GLFW_KEY_UP:
        {
            _RotationX++;
            break;
        }

        case GLFW_KEY_DOWN:
        {
            _RotationX--;
            break;   
        }

        case GLFW_KEY_LEFT:
        {
            _RotationY--;
            break;   
        }
        case GLFW_KEY_RIGHT:
        {
            _RotationY++;
            break;   
        }
        case GLFW_KEY_1:
        {
            changeColor(0);
            break;   
        }

        case GLFW_KEY_2:
        {
            changeColor(1);
            break;   
        }

        case GLFW_KEY_3:
        {
            changeColor(2);
            break;   
        }

        case GLFW_KEY_4:
        {
            changeColor(3);
            break;   
        }

        case GLFW_KEY_5:
        {
            changeColor(4);
            break;   
        }

        case GLFW_KEY_6:
        {
            changeColor(5);
            break;   
        }

        case GLFW_KEY_T:
        {
            _TextureMode = !_TextureMode;
            break;   
        }

        case GLFW_KEY_A:
        {
            _AutoRotationMode = !_AutoRotationMode;

            if (_AutoRotationMode)
            {
                _AutoRotationCws = !_AutoRotationCws;
            }
            break;   
        }

        case GLFW_KEY_D:
        {
            offset += 1;
            changeColor(1);
            break;
        }
    }
}

void CubeModel::handleScroll(double xoffset, double yoffset)
{
    _RotationDelta += yoffset / 10;
    if (_RotationDelta < 0)
    {
        _RotationDelta = 0.1f;
    }

    if (_RotationDelta > 20.0f)
    {
        _RotationDelta = 20.0f;
    }
}

void CubeModel::draw(const glm::mat4& projection, const glm::mat4& view)
{
    if (_AutoRotationMode)
    {
        if (_AutoRotationCws)
        {
            _RotationX += _RotationDelta;
        } else
        {
            _RotationX -= _RotationDelta;
        }
    }

    // Rotate the cube
    // using complex mathematic equations
    _Model = glm::mat4(1.0f);
    _Model = glm::rotate(_Model, glm::radians(_RotationX), glm::vec3(1.0f, 0.0f, 0.0f)); // rotate the cube
    _Model = glm::rotate(_Model, glm::radians(_RotationY), glm::vec3(0.0f, 1.0f, 0.0f)); // rotate the cube 

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    glm::mat4 mvp = projection * view * _Model;

    if (!_TextureShader || !_ColorShader)
    {
        loadShaders();
    }    
    // Use our shader
    if ( _TextureMode)
    {
        _TextureShader->use();
        _TextureShader->set("MVP", mvp);
        _TextureShader->applyTexture(*(CubeTexture::get("brick")));
    } else
    {
        _ColorShader->use();
        _ColorShader->set("MVP", mvp);
    }

    glBindBuffer(GL_ARRAY_BUFFER, _VertexBuffer);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    glBindBuffer(GL_ARRAY_BUFFER, _ColorBuffer);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        2,                                // size : U+V or texture coords
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles
}
