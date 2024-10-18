#include "engine.hpp"

#include <GL/glew.h>

std::vector<Model*> Engine::s_Models;

Engine::Engine(const std::string& title, int width, int height):
    _ScreenTitle(title),
    _ScreenWidth(width),
    _ScreenHeight(height)
{

};

Engine::~Engine()
{
    cleanUp();

    glfwTerminate();
}

bool Engine::initialize()
{
    // Initialize GLFW
	if( !glfwInit() )
	{
		std::cout << "Failed to initialize GLFW." << std::endl;
		return false;
	}

    srand(time(0));

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef __linux__ 
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
#endif
	
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
	_Window = glfwCreateWindow( _ScreenWidth, _ScreenHeight, _ScreenTitle.c_str(), NULL, NULL);
	if (_Window == NULL )
    {
		std::cerr << "Failed to open GLFW window." << std::endl;

		return false;
	}
	glfwMakeContextCurrent(_Window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW." << std::endl;
		return false;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(_Window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        /*for (auto& model: s_Models)
        {
            model->handleInput(key);
        }*/
    });

    glfwSetScrollCallback(_Window, [](GLFWwindow* window, double xoffset, double yoffset)
    {
        for (auto& model: s_Models)
        {
            model->handleScroll(xoffset, yoffset);
        }
    });

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS);

    _Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	_View       = glm::lookAt(
								glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0));  // Head is up (set to 0,-1,0 to look upside-down)

    
    return true;
}

void Engine::handleKeyInputs()
{
    int key{-1};
    if (glfwGetKey(_Window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        key = GLFW_KEY_UP;
    }
    else if (glfwGetKey(_Window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        key = GLFW_KEY_DOWN;
    } else if (glfwGetKey(_Window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        key = GLFW_KEY_LEFT;
    }
    else if (glfwGetKey(_Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        key = GLFW_KEY_RIGHT;
    } else if (glfwGetKey(_Window, GLFW_KEY_1) == GLFW_PRESS)
    {
        key = GLFW_KEY_1;
    } else if (glfwGetKey(_Window, GLFW_KEY_2) == GLFW_PRESS)
    {
        key = GLFW_KEY_2;
    } else if (glfwGetKey(_Window, GLFW_KEY_3) == GLFW_PRESS)
    {
        key = GLFW_KEY_3;
    } else if (glfwGetKey(_Window, GLFW_KEY_4) == GLFW_PRESS)
    {
        key = GLFW_KEY_4;
    } else if (glfwGetKey(_Window, GLFW_KEY_5) == GLFW_PRESS)
    {
        key = GLFW_KEY_5;
    } else if (glfwGetKey(_Window, GLFW_KEY_6) == GLFW_PRESS)
    {
        key = GLFW_KEY_6;
    }  
    else if (glfwGetKey(_Window, GLFW_KEY_T) == GLFW_PRESS)
    {
        // Compute time difference between current and last frame
        static double lastTime = 0;
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);

        if (deltaTime > 0.2f)
        {
            // For the next frame, the "last time" will be "now"
            lastTime = currentTime;
        
            key = GLFW_KEY_T;
        }
    }  else if (glfwGetKey(_Window, GLFW_KEY_A) == GLFW_PRESS)
    {
        // Compute time difference between current and last frame
        static double lastTime = 0;
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);

        if (deltaTime > 0.2f)
        {
            // For the next frame, the "last time" will be "now"
            lastTime = currentTime;
            key = GLFW_KEY_A;
        }
    }

    for (auto& model: s_Models)
    {
        model->handleInput(key);
    }
}

int Engine::run()
{
    while (glfwGetKey(_Window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(_Window))
    {
        // Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        handleKeyInputs();

        drawFrame();
		
		// Swap buffers
		glfwSwapBuffers(_Window);
		glfwPollEvents();
    }

    return 0;
}
	
int Engine::load()
{
	glGenVertexArrays(1, &_VertexArrayID);
	glBindVertexArray(_VertexArrayID);

    CubeShader::loadShaders();
    CubeTexture::loadTextures();

    //_Models;
    Model* model = new CubeModel();

    s_Models.push_back(model);

    if (CubeShader::s_Shaders.size() > 0 && CubeTexture::s_Textures.size() > 0)
    {
        return true;
    }

    return false;
}

int Engine::drawFrame()
{
    for (auto& model: s_Models)
    {
        model->draw(_Projection, _View);
    }

    return 0;
}

int Engine::cleanUp()
{
    for (auto& model: s_Models)
    {
        delete model;
    }
    s_Models.clear();

    glDeleteVertexArrays(1, &_VertexArrayID);


    return s_Models.size();
}