#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Actor
{
public:
	Actor(glm::vec3 position, glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle) :
		m_position(position),
		m_scale(scale),
		m_rotationAxis(rotationAxis),
		m_rotationAngle(rotationAngle) {

	}

	virtual void update() = 0;
	virtual void render() = 0;

	virtual void setScale(glm::vec3& scale)
	{
		m_scale = std::move(scale);
	}

	virtual void setPosition(glm::vec3& pos)
	{
		m_position = std::move(pos);
	}

	virtual void rotate(glm::vec3& rotationAxis, float rotationAngle)
	{
		m_rotationAxis = std::move(rotationAxis);
		m_rotationAngle = std::move(rotationAngle);
	}

protected:
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_rotationAxis;
	float m_rotationAngle;
};

