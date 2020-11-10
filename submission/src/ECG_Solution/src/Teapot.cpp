#include "Teapot.h"

Teapot::Teapot(glm::vec3 position, glm::vec3 color, glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle) : //, glm::vec3 rotationAxis, glm::vec3 rotationAngle
	Actor(position, scale, rotationAxis, rotationAngle),
	m_color(color),
	pos_lq(0),
	model_lq(1),
	color_lq(4)
{
}

void Teapot::update()
{
	//glUniform3fv(pos_lq, 1, glm::value_ptr(m_position));
	//glUniform3fv(pos_lq, 1, glm::value_ptr(m_position));
	glm::mat4 model = getModelMatrix();
	glUniformMatrix4fv(model_lq, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(color_lq, 1, glm::value_ptr(m_color));
}

void Teapot::render()
{
	drawTeapot();
}



glm::mat4 Teapot::getModelMatrix() const
{
	glm::mat4 M(1.0f);
	M = glm::translate(M, m_position);
	M = glm::scale(M, m_scale);
	M = glm::rotate(M, glm::radians(m_rotationAngle), m_rotationAxis);
	return M; //
}
