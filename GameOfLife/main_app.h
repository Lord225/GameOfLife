#pragma once
#include <iostream>
#include "Application.h"
#include "Shader.h"
#include "Camera2D.h"
#include "Mesh.h"
#include "MovmentControll.h"
#include "gameoflife_engie.h"

//						TODO 
// program structure							... ok
// event handling								... ok
// movment										... ok
// core engine									... ok
// flow control									... ok
// place pixels									..
// fade out effect								... ok 
// multithreded gof with 2 ptr's				... ok
// better movment								... ok
//		with screen to view cords projection	... ok
// loading from files							.			
// off thread rendering							... ok				
// FIX THIS SHADER CLASS						... ok
//	   (ADD SAFE CHECK FOR LINKING XD)
//	   (2x Fail)

class main_app : public Application {
public:
	main_app() = default;

	Shader shader;
	Mesh render_plane;
	Camera2D camera;
	gameoflife_engie engine;
	
	glm::vec2 mouse_pos = {0, 0};

	size_t GOL_SIZE = 1024;
	size_t frame_counter = 0;
	float pixel_scale = GOL_SIZE / 10.0f;
	bool show_times = false;
public:
	void OnStart(ArgParser::map_type&& args) override {
		GOL_SIZE = std::get<int>(args["size"]);
		pixel_scale = ((float)GOL_SIZE)/ std::get<float>(args["pixelsize"]);
		show_times = std::get<bool>(args["showsteptimes"]);
		
		std::vector<Vertex> vert = {
			Vertex{{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}},
			Vertex{{ 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}},
			Vertex{{ 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
			Vertex{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}}
		};
		std::vector<GLuint> tris = {
			0, 1, 2,
			0, 2, 3
		};
		const Texture texture {0, GOL_SIZE, GOL_SIZE };
		
		camera = std::move(Camera2D(window_size.x, window_size.y, 10.0f)); 
		render_plane = std::move(Mesh(std::move(vert), std::move(tris), texture));
		render_plane.set_position({ 0, 0, 0 });
		render_plane.model = glm::scale(render_plane.model, { pixel_scale, pixel_scale, 1.0f });

		try 
		{
			UnlinkedShader unlinked;
			const auto path = std::get<std::string>(args["shaderpath"]);
			
			unlinked.LoadFromFile(Shader::FRAGMENT_SHADER, path+"/fragment.frag");
			unlinked.LoadFromFile(Shader::VERTEX_SHADER, path+"/geometry.vert");
			
			shader = unlinked.Link();
			shader.AddUniform("StartingIndex");
		}
		catch (std::exception& what) {
			std::cout << what.what() << std::endl;
			return;
		}


		std::cout << "Using " << GOL_SIZE << "x" << GOL_SIZE << " board." << std::endl;
		shader["StartingIndex"] = 0;
		engine.init(GOL_SIZE, GOL_SIZE, std::get<int>(args["threads"]));
		engine.fill_random(0.5f);
	}

	void OnResize(GLFWwindow* window) override {
		glViewport(0, 0, window_size.x, window_size.y);
		MovmentControll::screen_size = {-window_size.x, window_size.y};
		camera.update_aspect_ratio(this->window_size.x, this->window_size.y, MovmentControll::zoom);
	}
	
	bool isPaused = false;

	float simulation_speed = 0.3f;
	float simulation_timer = 0.0f;

	const float minimal_speed = 1000.0f;
	const float maximal_speed = 0.0f;
	const float speed_delta = 10.0f;

	void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods) override {
		const auto clamper = MovmentControll::get_clamper(maximal_speed, minimal_speed);
		if (key == GLFW_KEY_SPACE && action==1) 
		{
			isPaused = !isPaused;
		}

		if (key == GLFW_KEY_LEFT)
		{
			simulation_speed = clamper(simulation_speed - speed_delta);
		}
		if (key == GLFW_KEY_RIGHT)
		{
			if (isPaused && action==1)
			{
				engine.queue_job();
			}
			else
			{
				simulation_speed = clamper(simulation_speed + speed_delta);
			}
		}

	}


	int texture_offset = 0;
	void UpdateTexture() {
		render_plane.update_texture(engine.get_ptr(), engine.get_w(), engine.get_h(), texture_offset);
		texture_offset = (texture_offset + 1) % render_plane.depth;
		shader["StartingIndex"] = texture_offset;
	}

	void engine_update(const double delta)
	{
		if (!isPaused) {
			if (simulation_timer > simulation_speed) {
				if (engine.isJobDone())
				{
					UpdateTexture();
					engine.queue_job();
				}

				simulation_timer = 0;
			}
			else {
				simulation_timer += delta;
			}
		}
		else
		{
			if (engine.isJobDone())
			{
				UpdateTexture();
			}
		}
	}

	void OnUpdate(const double delta) override {
		frame_counter++;
		engine_update(delta);
		
		camera.update_aspect_ratio(this->window_size.x, this->window_size.y, MovmentControll::zoom);
		camera.view = glm::translate(camera.view, (glm::make_vec3(MovmentControll::move)));
		

		MovmentControll::Update(camera);

		glClearColor(0.05f, 0.05f, 0.05f, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render_plane.draw(shader, camera);

		if (show_times && frame_counter % 120 == 0)
			std::cout << "sim: dt = " << engine.delta_time << "ms \t frame: dt = " << delta << "ms" << std::endl;
	}

	glm::vec3 map_screen_to_texture_pixels(glm::vec2& data) 
	{
		const auto normalized = 2.0f*data / glm::vec2(window_size) - 1.0f;

		return camera.inversed_global_transform({ normalized.x, -normalized.y, -1.0f});
	}

	void OnClose(GLFWwindow* window) override 
	{

	}


	void OnCursor(GLFWwindow* window, int button, int action, int mods) override
	{
		MovmentControll::OnCursor(window, button, action, mods);

		const auto worldCords = map_screen_to_texture_pixels(mouse_pos);

		if (button == 0 && action == 1) //place
		{
			
		}

		if (button == 1 && action == 1) //remove
		{

		}
	}
	void OnMouseMove(GLFWwindow* window, double xpos, double ypos) override
	{
		mouse_pos = { xpos, ypos };
		MovmentControll::OnMouseMove(window, xpos, ypos);
	}
	void OnScroll(GLFWwindow* window, double xoffset, double yoffset) override
	{
		MovmentControll::OnScroll(window, xoffset, yoffset);
	}
};