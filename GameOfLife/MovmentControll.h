#pragma once
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "Shader.h"

// Class that handels camera movment based on mouse input.
class MovmentControll
{
	static glm::vec2 acc;
	static glm::vec2 pos_last;
	static glm::vec2 actual;
	static bool isMiddlePressed;
	static float low_barier;
	static float high_barier;
	static float low_hard_barier;
	static float high_hard_barier;
	static float drag;
	static float zoom_acc;
public:
	//clamp function
	//auto clamper = get_clamper(low, high); //clamper is callable. clamper(float)->float
	//float clamped = clamper(unclamped);
	static auto get_clamper(float low, float high) {
		return [=](float val) -> float {
			if (val < low) {
				return low;
			}
			if (val > high) {
				return high;
			}
			return val;
		};
	}

	static glm::vec2 move;        // acceleration for camera movment.
	static glm::vec2 screen_size; // actual screensize (should be updated)
	static float zoom;            // zoom value

private:
	static void update_acc() {
		acc = pos_last - actual;
		pos_last = actual;
	}
	static void update_zooom() {
		const auto clamp = get_clamper(low_hard_barier, high_hard_barier);
		zoom = clamp(zoom + zoom_acc);
	}
	static void update_move(Camera& camera) {
		const auto world = glm::vec2(camera.inversed_transform(glm::vec3(acc, 0.0f)));
		move += world * (float)isMiddlePressed * 0.5f;
	}
public:

	// Should be called everyframe. Updates all internal mechanics, sets move and zoom to proper values.
	static void Update(Camera& camera)
	{
		update_acc();

		update_zooom();

		update_move(camera);

		move *= drag;
		zoom_acc *= drag;
	}

	//All callbacks.
	static void OnCursor(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == 2) {
			isMiddlePressed = action;
		}
	}

	static void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
	{
		 actual = { xpos, ypos };
		 actual = 2.0f * actual / glm::vec2(screen_size) - 1.0f;
	}

	static void OnScroll(GLFWwindow* window, double xoffset, double yoffset)
	{
		zoom_acc += yoffset * std::sqrt(zoom / 20) / 3.5f;
		auto centered = (screen_size*0.5f - pos_last) / (0.5f*screen_size);
	}
};



