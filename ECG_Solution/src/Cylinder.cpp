#include "Cylinder.h"

#define _USE_MATH_DEFINES
#include <math.h>

GeomShape::Cylinder::Cylinder(glm::vec3 center, float radius, float height, glm::vec3 color, glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, int sectorCount)
	:
	Actor(center, scale, rotationAxis, rotationAngle),
	Shape(color),
	m_radius(radius),
	m_height(height),
	m_sectorCount(sectorCount)
{
	createVertices();
	createIndices();


	if (m_numObjects == 0) {
		std::cout << "Generating VAO for Cylinder." << std::endl;
		glGenVertexArrays(1, &m_vaoID);
	}
	m_numObjects += 1;

	init();
}

GeomShape::Cylinder::~Cylinder()
{
	// virtual base destructors are always called after destructing derived
	if (m_numObjects == 1) {
		std::cout << "Deleting VAO for Cylinder." << std::endl;
		glDeleteVertexArrays(1, &m_vaoID);
	}
	m_numObjects -= 1;
	assert(m_numObjects >= 0);
}

void GeomShape::Cylinder::update()
{
	Actor::update();
	Shape::update(m_model);
}

void GeomShape::Cylinder::render()
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

void GeomShape::Cylinder::createVertices()
{
	float sectorStep = 2 * M_PI / m_sectorCount;
	addVertex(0, m_height / 2, 0);
	for (int i = 0; i <= 1; i++) {
		float y = (i == 0) ? m_height / 2 : -m_height / 2; // flipping because of wrong winding
		for (int sector = 0; sector < m_sectorCount; sector++) { // 
			float sectorAngle = sector * -sectorStep;
			float x = m_radius * cos(sectorAngle);
			float z = m_radius * sin(sectorAngle);

			addVertex(x, y, z);
		}
	}
	addVertex(0, -m_height / 2, 0);
	std::cout << "Cylinder vertices: " << 2 * m_sectorCount + 2 << " expected, vs " << getVerticesSize() / 3 << "\n";
}

void GeomShape::Cylinder::createIndices()
{
	unsigned int startOfCircle, nextCircle;

	for (int sector = 0; sector < m_sectorCount; sector++) {
		if (sector != m_sectorCount - 1) {
			addIndex(0);
			addIndex(sector + 1);
			addIndex(sector + 2);
		}
		else {
			addIndex(0);
			addIndex(sector + 1);
			addIndex(1);
		}
	}


	startOfCircle = 1;
	for (int sector = 0; sector < m_sectorCount; sector++) {

		nextCircle = startOfCircle + m_sectorCount;

		// indices
		//  v1--v1+1
		//  | \  |
		//  |  \ |
		//  v2--v2+1
		unsigned int ul = startOfCircle + sector;
		unsigned int ll = nextCircle + sector;
		unsigned int lr = nextCircle + sector + 1;
		unsigned int ur = startOfCircle + sector + 1;
		if (sector != m_sectorCount - 1) {
			addIndex(ul);
			addIndex(ll);
			addIndex(lr);

			addIndex(lr);
			addIndex(ur);
			addIndex(ul);
		}
		else {
			addIndex(ul);
			addIndex(ll);
			addIndex(lr - m_sectorCount);

			addIndex(lr - m_sectorCount);
			addIndex(ur - m_sectorCount);
			addIndex(ul);
		}


	}


	for (int sector = 0; sector < m_sectorCount; sector++) {
		unsigned int last = 2 * m_sectorCount + 1; // first + one circle + 1 to get the first
		if (sector != m_sectorCount - 1) {
			addIndex(last - m_sectorCount + sector + 1);
			addIndex(last - m_sectorCount + sector);
			addIndex(last);
		}
		else {
			addIndex(last - m_sectorCount);
			addIndex(last - m_sectorCount + sector);
			addIndex(last);
		}

	}
	std::cout << "Cylinder triangles: " << 4 * m_sectorCount << " expected, vs " << getIndicesSize() / 3 << "\n";
}
