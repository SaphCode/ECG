#pragma once

/*
// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
*/
#include <glm/glm.hpp>
//#include "glm/ext.hpp"
class Light
{
public:
	glm::vec3 getAmbient() const {
		return m_ambient;
	}
	glm::vec3 getDiffuse() const {
		return m_diffuse;
	}
	glm::vec3 getSpecular() const {
		return m_specular;
	}
protected:
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : 
		m_ambient(ambient),
		m_diffuse(diffuse),
		m_specular(specular)
	{
	
	}

	virtual ~Light() {}
	
	void setColor(glm::vec3 color) {
		m_ambient = color;
		m_diffuse = color;
		m_specular = color;
	}

	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
};

