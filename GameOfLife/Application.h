#pragma once
#include <glm/glm.hpp>

class Application
{
public:
	virtual void OnStart(ArgParser::map_type&& args) = 0;
	virtual void OnResize(GLFWwindow* window) = 0;

	virtual void OnUpdate(double delta) = 0;
	virtual void OnClose(GLFWwindow* window) = 0;

	virtual void OnCursor(GLFWwindow* window, int button, int action, int mods) = 0;
	virtual void OnMouseMove(GLFWwindow* window, double xpos, double ypos) = 0;
	virtual void OnScroll(GLFWwindow* window, double xoffset, double yoffset) = 0;
	virtual void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
	GLFWwindow* window;
	glm::ivec2 window_size;
};

