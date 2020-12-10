#include "Cylinder.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include<map>

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

void GeomShape::Cylinder::createNormals() {
	float sectorStep = 2 * M_PI / m_sectorCount;
	float lengthInv = 1 / m_radius;
	addNormal(0, 1.0f, 0);
	for (int i = 0; i <= 1; i++) {
		float y = (i == 0) ? m_height / 2 : -m_height / 2; // flipping because of wrong winding
		for (int sector = 0; sector < m_sectorCount; sector++) { // 
			float sectorAngle = sector * -sectorStep;
			float x = m_radius * cosf(sectorAngle);
			float z = m_radius * sinf(sectorAngle);
			
			float nx = x * lengthInv;
			float nz = z * lengthInv;
			float ny = (i == 0) ? 1.0f : -1.0f;
			
			addNormal(0.f, ny, 0.f);
			addNormal(nx, 0.f, nz);
		}
	}
	addNormal(0, -1.0f, 0);
}

void GeomShape::Cylinder::createVertices()
{
	float sectorStep = 2 * M_PI / m_sectorCount;
	addVertex(0, m_height / 2, 0);
	for (int i = 0; i <= 1; i++) {
		float y = (i == 0) ? m_height / 2 : -m_height / 2; // flipping because of wrong winding
		for (int sector = 0; sector < m_sectorCount; sector++) { // 
			float sectorAngle = sector * -sectorStep;
			float x = m_radius * cosf(sectorAngle);
			float z = m_radius * sinf(sectorAngle);

			addVertex(x, y, z);
			addVertex(x, y, z);
		}
	}
	addVertex(0, -m_height / 2, 0);
	std::cout << "Cylinder vertices: " << 2 * m_sectorCount * 2 + 2 << " expected, vs " << getVerticesSize() / 3 << "\n";
}

void GeomShape::Cylinder::createIndices()
{
	unsigned int startOfCircle, nextCircle;

	unsigned int offset = 2;

	std::map<unsigned int, unsigned int> mapping;

	mapping[0] = 0;
	for (int i = 1; i < 2 * m_sectorCount + 1; i++) {
		mapping[i] = i + (i - 1);
		std::cout << "(" << i << " -> " << mapping[i] << ")" << std::endl;
	}
	mapping[2 * m_sectorCount + 1] = 2 * 2 * m_sectorCount + 1;

	for (int sector = 0; sector < m_sectorCount; sector++) {
		if (sector != m_sectorCount - 1) {
			addIndices(mapping[0], 2 * sector + mapping[1], 2 * sector + mapping[2]);
			addIndices(mapping[0], 2 * sector + mapping[1] + 1, 2 * sector + mapping[2] + 1);
		}
		else {
			addIndices(mapping[0], 2 * sector + mapping[1], mapping[1]);
			addIndices(mapping[0], 2 * sector + mapping[1] + 1, mapping[1] + 1);
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
			addIndices(mapping[ul], mapping[ll], mapping[lr]);
			addIndices(mapping[ul] + 1, mapping[ll] + 1, mapping[lr] + 1);

			addIndices(mapping[lr], mapping[ur], mapping[ul]);
			addIndices(mapping[lr] + 1, mapping[ur] + 1, mapping[ul] + 1);
		}
		else {
			addIndices(mapping[ul], mapping[ll], mapping[lr - m_sectorCount]);
			addIndices(mapping[ul] + 1, mapping[ll] + 1, mapping[lr - m_sectorCount] + 1);

			addIndices(mapping[lr - m_sectorCount] + 1, mapping[ur - m_sectorCount] + 1, mapping[ul] + 1);
		}


	}


	for (int sector = 0; sector < m_sectorCount; sector++) {
		unsigned int last = 2 * m_sectorCount + 1; // first + one circle + 1 to get the first
		if (sector != m_sectorCount - 1) {
			addIndices(mapping[last - m_sectorCount + sector + 1], mapping[last - m_sectorCount + sector], mapping[last]);
			addIndices(mapping[last - m_sectorCount + sector + 1] + 1, mapping[last - m_sectorCount + sector] + 1, mapping[last]);
		}
		else {
			addIndices(mapping[last - m_sectorCount], mapping[last - m_sectorCount + sector], mapping[last]);
			addIndices(mapping[last - m_sectorCount] + 1, mapping[last - m_sectorCount + sector] + 1, mapping[last]);
		}

	}
	std::cout << "Cylinder triangles: " << 4 * m_sectorCount << " expected, vs " << getIndicesSize() / 3 << "\n";
}
