#include "Sphere.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>

GeomShape::Sphere::Sphere(glm::vec3 center, float radius, glm::vec3 color, glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, int sectorCount, int stackCount) :
	Actor(center, scale, rotationAxis, rotationAngle),
	Shape(color),
	m_radius(radius),
	m_sectorCount(sectorCount),
	m_stackCount(stackCount)
{
	createVertices();
	createIndices();

	if (m_numObjects == 0) {
		std::cout << "Generating VAO for Sphere." << std::endl;
		glGenVertexArrays(1, &m_vaoID);
	}
	m_numObjects += 1;

	init();
}

GeomShape::Sphere::~Sphere() {
	// virtual base destructors are always called after destructing derived
	if (m_numObjects == 1) {
		std::cout << "Deleting VAO for Sphere." << std::endl;
		glDeleteVertexArrays(1, &m_vaoID);
	}
	m_numObjects -= 1;
	assert(m_numObjects >= 0);
}

void GeomShape::Sphere::update()
{
	Actor::update();
	Shape::update(m_model);
}

void GeomShape::Sphere::render()
{
	// bind VAO
	glBindVertexArray(m_vaoID); // bind
	Shape::bindVBO();
	Shape::draw();
	glBindVertexArray(0);
	Shape::unbindVBO();

	const GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cout << "Error encountered after rendering: " << error << std::endl;
	}
}

void GeomShape::Sphere::createIndices() {
	unsigned int v1, v2;

	for (int j = 0; j < m_sectorCount; j++) {
		if (j != m_sectorCount - 1) {
			addIndex(0);
			addIndex(1 + j);
			addIndex(1 + j + 1);
		}
		else {
			addIndex(0);
			addIndex(1 + j);
			addIndex(1);
		}
		//std::cout << "(" << 1 << "," << j << "): (" << 0 << "," << (1 + j) % 16 << "," << (1 + j + 1) % 16 << ")\n";
	}
	for (int i = 1; i < m_stackCount-1; i++) { // the first vertices are one stackAngle down. (so increase first)
		v1 = 1 + m_sectorCount * (i - 1);
		v2 = v1 + m_sectorCount;
		for (int j = 0; j < m_sectorCount; j++) {

			// indices
			//  v1--v1+1
			//  | \  |
			//  |  \ |
			//  v2--v2+1
			unsigned int ul = v1 + j;
			unsigned int ll = v2 + j;
			unsigned int lr = v2 + j + 1;
			unsigned int ur = v1 + j + 1;
			if (j != m_sectorCount - 1) {
				addIndex(ul);
				addIndex(ll);
				addIndex(lr);

				addIndex(lr);
				addIndex(ur);
				addIndex(ul);
			}
			else {
				lr -= m_sectorCount;
				ur -= m_sectorCount;
				addIndex(ul);
				addIndex(ll);
				addIndex(lr);

				addIndex(lr);
				addIndex(ur);
				addIndex(ul);
			}
			
			
			//std::cout << "(" << i+1 << "," << j << "): (" << ul % 16 << "," << ll % 16 << "," << lr % 16 << "), " << "(" << ul << ", " << ll << ", " << lr << ")\n";
			//std::cout << "(" << i+1 << "," << j << "): (" << lr % 16 << "," << ur % 16 << "," << ul % 16 << "), " << "(" << lr << ", " << ur << ", " << ul << ")\n";
			
		}
	}
	for (int j = 0; j < m_sectorCount; j++) {
		unsigned int last = m_sectorCount * (m_stackCount - 1) + 1;

		if (j != m_sectorCount - 1) {
			addIndex(last - m_sectorCount + 1 + j);
			addIndex(last - m_sectorCount + j);
			addIndex(last);
		}
		else {
			addIndex(last - m_sectorCount);
			addIndex(last - m_sectorCount + j);
			addIndex(last);
		}
		//std::cout << "(" << m_stackCount << "," << j << "): (" << (m_sectorCount * (m_stackCount - 2) + j + 1 ) << "," << (m_sectorCount * (m_stackCount - 2) + j) << "," << (m_sectorCount * (m_stackCount - 1) + 1) << ")\n";
	}

	std::cout << "Spere triangles: " << 2 * m_sectorCount * (m_stackCount - 1) << " expected, vs " << getIndicesSize()/3 << "\n";
}

void GeomShape::Sphere::createVertices() {
	float sectorStep = 2 * M_PI / m_sectorCount;
	float stackStep = M_PI / m_stackCount;

	// from top to bottom
	addVertex(0, m_radius, 0);
	for (int stack = 1; stack < m_stackCount; stack++) {
		// get the angle from the top (0 deg)
		float stackAngle = stack * stackStep;

		float y = m_radius * cos(stackAngle);
		float xz_from_y_axis = m_radius * sin(stackAngle);

		for (int sector = 0; sector < m_sectorCount; sector++) { // changed: <=
			// get the angle corresponding to the sector #
			float sectorAngle = sector * -sectorStep;

			float x = xz_from_y_axis * cos(sectorAngle);
			float z = xz_from_y_axis * sin(sectorAngle);

			addVertex(x, y, z);
		}
	}
	addVertex(0, -m_radius, 0);
	std::cout << "Sphere Vertices: " << 2 + m_sectorCount * (m_stackCount - 1) << " expected, vs " << getVerticesSize()/3 << "\n";
}

