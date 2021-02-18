#include "Shader.h"


UnlinkedShader::UnlinkedShader() {
	shaders.resize(2);
}
void UnlinkedShader::LoadFromFile(ShaderType type, std::string&& source)
{
	std::fstream stream(source);
	if (stream.good()) {
		std::string shader_source;
		shader_source.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
		compile(type, std::move(shader_source));
	}
	else {
		throw std::exception("Failed on loading shader");
	}
}

[[nodiscard]] Shader UnlinkedShader::Link()
{
	int success;
	compiled_program = glCreateProgram();

	for(auto shader : shaders)
		if (shader != 0)
			glAttachShader(compiled_program, shader);
	
	glLinkProgram(compiled_program);

	glGetProgramiv(compiled_program, GL_LINK_STATUS, &success);
	if (!success) {
		std::array<char, 512> log;
		glGetProgramInfoLog(compiled_program, 512, nullptr, log.data());
		throw std::exception((std::string("Failed to compile shader: \n") + std::string(log.begin(), log.end())).c_str());
	}

	for (auto shader : shaders)
		if (shader != 0)
			glAttachShader(compiled_program, shader);
	shaders.clear();
	Shader compiled = Shader(std::move(*this));

	compiled.AddUniform(TRANSFORMATION_UNIFORM_NAME);
	return compiled;
}

void Shader::Use() const
{
	glUseProgram(get_program());
}

void Shader::UnUse() const
{
	glUseProgram(0);
}

void Shader::AddUniform(std::string&& uniform)
{
	uniforms[uniform] = glGetUniformLocation(compiled_program, uniform.c_str());
}

void Shader::camera(Camera& cam, glm::mat4& model)
{
	this->operator[](TRANSFORMATION_UNIFORM_NAME) = cam.transform(model);
}

ShaderAssign Shader::operator[](std::string&& atrib)
{
	return ShaderAssign(this, uniforms[atrib]);
}

GLuint Shader::get_program() const
{
	return compiled_program;
}

void UnlinkedShader::compile(ShaderType type, std::string&& shader_source)
{
	switch (type)
	{
	case Shader::VERTEX_SHADER:
		shaders[0] = glCreateShader(GL_VERTEX_SHADER);
		compile(shaders[0], std::move(shader_source));
		break;
	case Shader::FRAGMENT_SHADER:
		shaders[1] = glCreateShader(GL_FRAGMENT_SHADER);
		compile(shaders[1], std::move(shader_source));
		break;
	default:
		throw std::exception("Bad Shader type");
	}
}

void UnlinkedShader::compile(GLuint& handle, std::string&& shader_source)
{
	const auto c_source = shader_source.c_str();
	int success;

	if (handle == 0) {
		throw std::exception("Failed to create shader");
	}

	glShaderSource(handle, 1, &c_source, NULL);
	glCompileShader(handle);
	glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

	if (!success) {
		std::array<char, 512> log;
		glGetShaderInfoLog(handle, 512, NULL, log.data());
		throw std::exception((std::string("Failed to compile shader: \n") + std::string(log.begin(),log.end())).c_str());
	}
}

