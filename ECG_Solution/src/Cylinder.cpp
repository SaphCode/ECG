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

	init();
}

GeomShape::Cylinder::~Cylinder()
{
	// virtual base destructor always called
}

void GeomShape::Cylinder::update()
{
	Actor::update();
	Shape::update(m_model);
}

void GeomShape::Cylinder::render()
{
	Shape::render();
}

void GeomShape::Cylinder::createVertices()
{
	float sectorStep = 2 * M_PI / m_sectorCount;
	addVertex(0, m_height / 2, 0);
	for (int i = 0; i <= 1; i++) {
		float y = (i == 0) ? m_height / 2 : -m_height / 2; // flipping because of wrong winding
		for (int sector = 0; sector <= m_sectorCount; sector++) { // 
			float sectorAngle = sector * -sectorStep;
			float x = m_radius * cos(sectorAngle);
			float z = m_radius * sin(sectorAngle);

			addVertex(x, y, z);
		}
	}
	addVertex(0, -m_height / 2, 0);
}

void GeomShape::Cylinder::createIndices()
{
	unsigned int startOfCircle, nextCircle;

	for (int sector = 0; sector < m_sectorCount; sector++) {
		addIndex(0);
		addIndex(sector + 1);
		addIndex(sector + 2);
	}


	startOfCircle = 1;
	for (int sector = 0; sector <= m_sectorCount; sector++) {

		nextCircle = startOfCircle + m_sectorCount;

		// indices
		//  v1--v1+1
		//  | \  |
		//  |  \ |
		//  v2--v2+1
		addIndex(startOfCircle + sector);
		addIndex(nextCircle + sector);
		addIndex(nextCircle + sector + 1);

		addIndex(nextCircle + sector + 1);
		addIndex(startOfCircle + sector + 1);
		addIndex(startOfCircle + sector);

	}


	for (int sector = 0; sector < m_sectorCount; sector++) {
		unsigned int start = 1 + m_sectorCount + 1; // first + one circle + 1 to get the first
		addIndex(start + sector + 1);
		addIndex(start + sector);
		addIndex(start + m_sectorCount + 1);
	}
	
}
