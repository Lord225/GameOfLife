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

class UnlinkedShader
{
public:
	enum ShaderType {
		VERTEX_SHADER,
		FRAGMENT_SHADER
	};
	UnlinkedShader();
	void LoadFromFile(ShaderType type, std::string&& source);
	[[nodiscard]]Shader Link();
	
private:
	std::vector<GLuint> shaders;
	std::map<std::string, GLuint> uniforms;

	GLuint compiled_program;

	void compile(ShaderType type, std::string&& shader_source) ;
	void compile(GLuint &handle, std::string&& shader_source);

	friend ShaderAssign;
	friend Shader;
};


class Shader : public UnlinkedShader
{
public:
	Shader& operator=(Shader&&) = default;
	Shader(Shader&) = default;
	Shader()
	{
		this->compiled_program = 0;
	}
	Shader(UnlinkedShader&& other)
	{
		if(&other != this)
		{
			this->compiled_program = other.compiled_program;
			this->uniforms = std::move(other.uniforms);
			other.compiled_program = 0;
		}
	}
	void Use() const;
	void UnUse() const;
	void AddUniform(std::string&& uniform);

	void camera(Camera& cam, glm::mat4& model);

	[[nodiscard]] ShaderAssign operator[](std::string&& atrib);
	[[nodiscard]] GLuint get_program() const;
};


class ShaderAssign {
public:
	ShaderAssign() = default;
	ShaderAssign(ShaderAssign&& rsh) = default;
	ShaderAssign& operator=(ShaderAssign&&) = default;

	ShaderAssign(Shader* ptr, GLuint uniform) :ptr(ptr), uniform(uniform) {}

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

class Camera {
public:
	glm::mat4 view;
	glm::mat4 transform(glm::mat4& model)
	{
		
		return projection * view * model;
	}

	glm::vec4 inversed_transform(glm::vec3 pos) {

		return glm::vec4(inversed * glm::make_vec4(pos));;
	}
	glm::vec4 inversed_global_transform(glm::vec3 pos) {
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

