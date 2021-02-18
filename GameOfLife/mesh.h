#pragma once
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <GLFW\glfw3.h>
#include <fstream>
#include <vector>
#include <array>
#include <map>
#include "Shader.h"
#include <vector>

struct Vertex {
	glm::vec3 pos;
	glm::vec2 texturePos;
};

struct Texture {
	GLuint texture_id;
	size_t w;
	size_t h;
};


class Mesh
{
public:
	glm::mat4 model;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Texture texture;
	Mesh() = default;
	Mesh& operator=(const Mesh&) = default;
	Mesh& operator=(Mesh&&) = default;
	Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices, Texture texture) : vertices(std::move(vertices)), indices(std::move(indices)), texture(texture)
	{
		Init(texture.w, texture.h);
	}

	void draw(Shader& shader, Camera& camera) 
	{
		shader.Use();
		shader.camera(camera, model);

		glBindTexture(GL_TEXTURE_2D, texture.texture_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	size_t depth = 8;
	void update_texture(const char* data, size_t w, size_t h, size_t offset) const {
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture.texture_id);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, offset, w, h, 1, GL_RED, GL_UNSIGNED_BYTE, data);
	}

	void set_position(glm::vec3 pos) {
		model = translate(glm::mat4(1), pos);
	}
	void Init(size_t w, size_t h) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); //cords
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float))); //text cords
		glEnableVertexAttribArray(1);


		glGenTextures(1, &(texture.texture_id));
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture.texture_id);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //Wtf?

		char* arr = new char[w * h*8];

		for (size_t i = 0; i < w*h*8; i++)
		{
			arr[i] = 0;
		}


		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RED, w, h, 8, 0, GL_RED, GL_UNSIGNED_BYTE, arr);
	}
	glm::vec2 global_to_plane_cords(glm::vec2& world)
	{
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(model, scale, rotation, translation, skew, perspective);

		return world / glm::vec2(scale);
	}
private:
	GLuint VAO, VBO, EBO;
	
};

