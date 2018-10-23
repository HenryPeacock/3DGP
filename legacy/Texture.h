#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

class Texture
{
	GLuint m_id;
	glm::vec2 m_size;

public:
	Texture(std::string path);
	glm::vec2 getSize();
	GLuint getId();

};
