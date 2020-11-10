#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 upVector);

	void setCameraView(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 upVector);

	glm::vec3 getEye() {
		return m_eye;
	}
	glm::vec3 getUpVector() const {
		return m_upVector;
	}
	glm::vec3 getLookAt() const {
		return m_lookAt; 
	}
	glm::mat4 getViewMatrix() const { 
		return m_viewMatrix; 
	}
	/*glm::vec3 getViewDir() const {
		return -glm::transpose(m_viewMatrix)[2]; // direction vector
	}*/
	glm::vec3 getRightVector() const {
		return glm::transpose(m_viewMatrix)[0]; // phi vector
	}

	glm::vec3 getViewDir() const {
		return -glm::transpose(m_viewMatrix)[2]; 
	}

private:
	glm::vec3 m_eye;
	glm::vec3 m_lookAt;
	glm::vec3 m_upVector;

	glm::mat4 m_viewMatrix;

	void updateViewMatrix();

};

