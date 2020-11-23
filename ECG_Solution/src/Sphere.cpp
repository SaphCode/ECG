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

	init();
}

GeomShape::Sphere::~Sphere() {
	// virtual base destructors are always called
}

void GeomShape::Sphere::update()
{
	Actor::update();
	Shape::update(m_model);
}

void GeomShape::Sphere::render()
{
	Shape::render();
}

void GeomShape::Sphere::createIndices() {
	unsigned int v1, v2;
	for (int i = 0; i < m_stackCount; i++) { // the first vertices are one stackAngle down. (so increase first)
		v1 = i * (m_sectorCount + 1);
		v2 = v1 + m_sectorCount + 1;
		for (int j = 0; j < m_sectorCount; j++) {

			// indices
			//  v1--v1+1
			//  | \  |
			//  |  \ |
			//  v2--v2+1
			if (i != m_stackCount) { // this is the corner to the left, we don't want this at the end
				addIndex(v1 + j);
				addIndex(v2 + j);
				addIndex(v2 + j + 1);
			}

			if (i != 0) { // this is the corner to the right, we don't want this at the beginning
				addIndex(v2 + j + 1);
				addIndex(v1 + j + 1);
				addIndex(v1 + j);
			}
		}
	}
}

void GeomShape::Sphere::createVertices() {
	float sectorStep = 2 * M_PI / m_sectorCount;
	float stackStep = M_PI / m_stackCount;

	// from top to bottom
	for (int stack = 0; stack <= m_stackCount; ++stack) {
		// get the angle from the top (0 deg)
		float stackAngle = stack * stackStep;

		float y = m_radius * cos(stackAngle);
		float xz_from_y_axis = m_radius * sin(stackAngle);

		for (int sector = 0; sector <= m_sectorCount; ++sector) { // changed: <=
			// get the angle corresponding to the sector #
			float sectorAngle = sector * sectorStep;

			float x = xz_from_y_axis * cos(sectorAngle);
			float z = xz_from_y_axis * sin(sectorAngle);

			addVertex(x, -y, z);
		}
	}
}

