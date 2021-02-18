#include <iostream>
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <glm/common.hpp>
#include "ArgParser.h"
#include "main_app.h"

Application* app;

const auto HEIGHT = 600;
const auto WIDTH = 600;

void resize(GLFWwindow* window, int width, int height)
{
    app->window_size.x = width;
    app->window_size.y = height;
    app->OnResize(window);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    app->OnCursor(window, button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    app->OnScroll(window, xoffset, yoffset);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    app->OnMouseMove(window, xpos, ypos);
}
void key_pressed_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    app->OnKey(window, key, scancode, action, mods);
}

auto get_window(std::string&& name) {
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, name.c_str(), nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        throw std::exception("Failed to create GLFW window");
    }
    return window;
}

void make_glad() 
{
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        throw std::exception("Failed to initialize GLAD");
    }
}

int main(const int num, const char** args)
{
    app = new main_app();
	
    auto argparser = ArgParser(
        ArgParser::map_type(
            {
            	{"size", 1024},
            	{"pixelsize", 10.0f},
                {"shaderpath", std::string("../GameOfLife")},
            	{"threads", 8},
            	{"randominit", false},
            	{"showsteptimes", false},
            }
        )
    );
    try {
        argparser.parse_arguments(num, args);
    }
	catch(std::exception& err)
    {
        std::cout << err.what() << std::endl;
        argparser.give_help();
        return -1;
    }
	
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    auto* const window = get_window("Game Of Life");

    glfwMakeContextCurrent(window);

    make_glad();

    glfwSetFramebufferSizeCallback(window, resize);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_pressed_callback);

    double last_time = glfwGetTime(), current_time = 0, delta = 0;
    app->window = window;
    resize(window, HEIGHT, WIDTH);
    app->OnStart(std::move(argparser.parsed_args));
    while (!glfwWindowShouldClose(window))
    {
        current_time = glfwGetTime();
        delta = current_time- last_time;
        last_time = current_time;
        

        app->OnUpdate(delta*1000);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    app->OnClose(window);
    glfwTerminate();
    return 0;
}