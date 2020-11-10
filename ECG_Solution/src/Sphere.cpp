#include "Sphere.h"

#define _USE_MATH_DEFINES
#include <math.h>

Sphere::Sphere(glm::vec3 center, float radius, glm::vec3 color, glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, int sectorCount, int stackCount) :
	Actor(center, scale, rotationAxis, rotationAngle),
	m_radius(radius),
	m_color(color),
	m_sectorCount(sectorCount),
	m_stackCount(stackCount)
{
	createVertices();
	createIndices();
}

void Sphere::createIndices() {

}

void Sphere::createVertices() {
	float sectorStep = 2 * M_PI / m_sectorCount;
	float stackStep = M_PI / m_stackCount;

	// from top to bottom
	for (int stack = 0; stack <= m_stackCount; stack++) {
		// get the angle from the top (0 deg)
		float stackAngle = stack * stackStep;

		float z = m_radius * cos(stackAngle);
		float xy_from_z_axis = m_radius * sin(stackAngle);

		for (int sector = 0; sector < m_sectorCount; sector++) { // dont need <= (last pos = first pos)
			// get the angle corresponding to the sector #
			float sectorAngle = sector * sectorStep;

			float x = xy_from_z_axis * cos(sectorAngle);
			float y = xy_from_z_axis * sin(sectorAngle);

			m_vertices.push_back(x);
			m_vertices.push_back(y);
			m_vertices.push_back(z);

		}
	}
}
