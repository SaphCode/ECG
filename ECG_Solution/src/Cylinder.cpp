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

	for (int i = 0; i <= 3; i++) {
		float y = (i <= 1) ? -m_height / 2 : m_height / 2; // flipping because of wrong winding
		for (int sector = 0; sector <= m_sectorCount; sector++) { // 
			if (i == 0 || i == 3) {
				addVertex(0, y, 0);
			}
			else {
				float sectorAngle = sector * sectorStep;
				float x = m_radius * cos(sectorAngle);
				float z = m_radius * sin(sectorAngle);

				addVertex(x, y, z);
			}
		}
	}
}

void GeomShape::Cylinder::createIndices()
{
	unsigned int v1, v2;
	for (int i = 0; i <= 2; i++) {

		v1 = i * (m_sectorCount + 1);
		v2 = v1 + m_sectorCount + 1;

		for (int sector = 0; sector < m_sectorCount; sector++) {

			// indices
			//  v1--v1+1
			//  | \  |
			//  |  \ |
			//  v2--v2+1
			if (i != 2) {
				addIndex(v1 + sector);
				addIndex(v2 + sector);
				addIndex(v2 + sector + 1);
			}

			if (i != 0) { // make the circle
				addIndex(v2 + sector + 1);
				addIndex(v1 + sector + 1);
				addIndex(v1 + sector);
			}

		}
	}
	
}
