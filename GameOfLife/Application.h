#pragma once
#include <glm/glm.hpp>

class Application
{
public:
	virtual void OnStart(ArgParser::map_type&& args) = 0; //Called once on start
	virtual void OnResize(GLFWwindow* window) = 0; //Called only on resize

	virtual void OnUpdate(double delta) = 0; //Called every frame
	virtual void OnClose(GLFWwindow* window) = 0; //Called on close

	virtual void OnCursor(GLFWwindow* window, int button, int action, int mods) = 0; //Called when mouse button is pressed/relased
	virtual void OnMouseMove(GLFWwindow* window, double xpos, double ypos) = 0; //Called when mouse moves.
	virtual void OnScroll(GLFWwindow* window, double xoffset, double yoffset) = 0; //Called when scroll moves
	virtual void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods) = 0; //Called when key is pressed/relased
	GLFWwindow* window;     //link to window.
	glm::ivec2 window_size; //window size.
};

