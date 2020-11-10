#pragma once

#include "Actor.h"
#include "Utils.h"


// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>



class Teapot : public Actor
{
public:
	Teapot(glm::vec3 position, glm::vec3 color, glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle);

	virtual void update() override;
	virtual void render() override;

private:
	glm::mat4 getModelMatrix() const;

	glm::vec3 m_color;

	const GLuint pos_lq;
	const GLuint model_lq;
	const GLuint color_lq;


};

