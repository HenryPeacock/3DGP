#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <fstream>
#include <string>

class VertexArray;

class ShaderProgram
{
private:
	GLuint id;

public:
	ShaderProgram();
	ShaderProgram(std::string vert, std::string frag);

	void draw(VertexArray *vertexArray);

	void setUniform(std::string uniform, glm::vec4 value);
	void setUniform(std::string uniform, float value);
	void setUniform(std::string uniform, glm::mat4 value);
	void setUniform(std::string uniform, int value);

	GLuint getId();
};
