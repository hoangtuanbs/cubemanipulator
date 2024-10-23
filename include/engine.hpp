#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <vector>
#include <iostream>


// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "model.hpp"
#include "shader.hpp"
#include "texture.hpp"

class Engine
{
public:
	int _ScreenWidth;
    int _ScreenHeight;
    std::string _ScreenTitle;

    glm::mat4 _Projection;
	glm::mat4 _View;

	float screenAspectRatio = 1;
	//synchronization
	long long int lastFrameMillis = 0;
	int targetFPS = 30;
	int millisPerFrame = 1000 / targetFPS;

    GLFWwindow* _Window;
    static std::vector<Model*> s_Models;
    GLuint _VertexArrayID;

public:
    Engine(const std::string& title,
        int width = 1024,
        int height = 768);

    ~Engine();

    bool initialize();

	int run();

    // Load resources
	int load();
	int drawFrame();
	int cleanUp();
};

#endif
