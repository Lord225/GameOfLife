#pragma once
#include <string>
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW\glfw3.h>
#include <fstream>
#include <vector>
#include <array>
#include <map>


//#define CHECK_RATIO_IN_EACH_FRAME
#define TRANSFORMATION_UNIFORM_NAME "proj"


class ShaderAssign;
class Camera;

class Shader;

// Unlinced shader class. You can load and compile your shader. This shader is not usable as long as is not builded.
// You can compile with 'Link'. Function will return linked shader ('Shader class') where you can du all wild stuff.
class UnlinkedShader
{
public:
	enum ShaderType {
		VERTEX_SHADER,
		FRAGMENT_SHADER
	};
	UnlinkedShader();
	void LoadFromFile(ShaderType type, std::string&& source);
	[[nodiscard]]Shader Link(); //Links Shader and returns linked version.
	
private:
	std::vector<GLuint> shaders;
	std::map<std::string, GLuint> uniforms;

	GLuint compiled_program;

	void compile(ShaderType type, std::string&& shader_source) ;
	void compile(GLuint &handle, std::string&& shader_source);

	friend ShaderAssign;
	friend Shader;
};

// Linked Shader class aka standard shader. You can't construct it by yourself. But you can change 'Unlinked Shader' into thisone.
class Shader : public UnlinkedShader
{
public:
	Shader& operator=(Shader&&) = default;
	Shader(Shader&) = default;
	Shader()
	{
		this->compiled_program = 0;
	}
	
	void Use() const;
	void UnUse() const;
	void AddUniform(std::string&& uniform);     //Adds new uniform to list.

	void camera(Camera& cam, glm::mat4& model); //Applys all camera transformations.

	[[nodiscard]] ShaderAssign operator[](std::string&& atrib); //Sets uniform value. You can use synax shader["name_of_uniform"] = value (any type) it will decuct good assign operator.
	[[nodiscard]] GLuint get_program() const;

private:
	Shader(UnlinkedShader&& other)
	{
		if (&other != this)
		{
			this->compiled_program = other.compiled_program;
			this->uniforms = std::move(other.uniforms);
			other.compiled_program = 0;
		}
	}
	friend UnlinkedShader;
};

//Helper class returned by shader. 
class ShaderAssign {
public:
	ShaderAssign() = default;
	ShaderAssign(ShaderAssign&& rsh) = default;
	ShaderAssign& operator=(ShaderAssign&&) = default;

	ShaderAssign(Shader* ptr, GLuint uniform) :ptr(ptr), uniform(uniform) {}
	~ShaderAssign() = default;

	//All possible uniform assigns
	void operator=(const float var) const {
		glUniform1f(uniform, var);
	}
	void operator=(const int var) const {
		glUniform1i(uniform, var);
	}
	void operator=(const glm::mat4& mat) const {
		glUniformMatrix4fv(uniform, 1, false, glm::value_ptr(mat));
	}
	void operator=(const glm::vec3& vec) const {
		glUniform3f(uniform, vec.x, vec.y, vec.z);
	}
	void operator=(const glm::vec2& vec) const {
		glUniform2f(uniform, vec.x, vec.y);
	}
	void operator=(const glm::mat3& mat) const {
		glUniformMatrix3fv(uniform, 1, false, glm::value_ptr(mat));
	}
private:
	const Shader* ptr;
	const GLuint uniform;
};

//Abstract camera class. Contains common functions for cameras.
class Camera {
public:
	glm::mat4 view;

	// model to screen
	glm::mat4 transform(glm::mat4& model) const
	{
		
		return projection * view * model;
	}

	// possible screen to camera local
	[[nodiscard]] glm::vec4 inversed_transform(const glm::vec3 pos) const {

		return glm::vec4(inversed * glm::make_vec4(pos));;
	}

	// possible screen to world
	[[nodiscard]] glm::vec4 inversed_global_transform(const glm::vec3 pos) const {
		return viewInversed * glm::make_vec4(pos);
	}

protected:
	glm::mat4 projection;
	glm::mat4 inversed;
	glm::mat4 viewInversed;
};

class PerspectiveCamera : public Camera {
public:
	PerspectiveCamera() {
	
	}
	PerspectiveCamera(int weight, int height, float fov = 90.0f, float near_plane = 0.1f, float far_plane = 100.0f)
	{
		view = glm::mat4(1.0f);
		update_aspect_ratio(weight, height, fov, near_plane, far_plane);
	}
	void update_aspect_ratio(int weight, int height, float fov = 90.0f, float near_plane = 0.1f, float far_plane = 100.0f)
	{
		projection = glm::perspective(glm::radians(fov), weight / (float)height, near_plane, far_plane);
		inversed = glm::inverse(projection);
		viewInversed = glm::inverse(projection * view);
	}
};

class OrographicCamera : public Camera {
public:
	OrographicCamera() {
	}
	OrographicCamera(int weight, int height, float scale = 1.0f, float near_plane = -1000.0f, float far_plane = 1000.0f)
	{
		view = glm::mat4(1.0f);
		update_aspect_ratio(weight, height, scale, near_plane, far_plane);
	}
	void update_aspect_ratio(int weight, int height, float scale = 1.0f, float near_plane = -1000.0f, float far_plane = 1000.0f)
	{
		float h = height * scale*0.005f;
		float w = weight * scale*0.005f;
		projection = glm::ortho(-w / 2, w / 2, -h / 2, h / 2, near_plane, far_plane);
		inversed = glm::inverse(projection);
		viewInversed = glm::inverse(projection * view);
	}

};

