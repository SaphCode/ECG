#include "Sphere.h"

#define _USE_MATH_DEFINES
#include <math.h>

Sphere::Sphere(glm::vec3 center, float radius, glm::vec3 color, glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, int sectorCount, int stackCount) :
	Actor(center, scale, rotationAxis, rotationAngle),
	m_radius(radius),
	m_color(color),
	m_sectorCount(sectorCount),
	m_stackCount(stackCount),
	m_model_lq(0),
	m_color_lq(4)
{
	createVertices();
	createIndices();

	
	glGenBuffers(1, &_vboID); // vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER,
		m_vertices.size() * sizeof(float),
		&m_vertices[0],
		GL_STATIC_DRAW);

	glGenBuffers(1, &_iboID); // index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		m_indices.size() * sizeof(unsigned int),
		&m_indices[0],
		GL_STATIC_DRAW);
}

Sphere::~Sphere() {
	// free VAO and VBO
	glDeleteBuffers(1, &_vboID);
	glDeleteBuffers(1, &_iboID);

	glDeleteVertexArrays(1, &vao)
}

void Sphere::update()
{
	Actor::update();
	glUniformMatrix4fv(m_model_lq, 1, GL_FALSE, glm::value_ptr(m_model));
	glUniform3fv(m_color_lq, 1, glm::value_ptr(m_color));
}

void Sphere::render()
{
	// bind VBOs
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID);

	// activate attrib arrays
	glEnableVertexAttribArray(attribVertex);
	glEnableVertexAttribArray(attribNormal);
	glEnableVertexAttribArray(attribTexCoord);

	// set attrib arrays with stride and offset
	int stride = sphere.getInterleavedStride();     // should be 32 bytes
	glVertexAttribPointer(attribVertex, 3, GL_FLOAT, false, stride, (void*)0);
	glVertexAttribPointer(attribNormal, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(attribTexCoord, 2, GL_FLOAT, false, stride, (void*)(sizeof(float) * 6));

	// draw a sphere with VBO
	glDrawElements(GL_TRIANGLES,                    // primitive type
		sphere.getIndexCount(),          // # of indices
		GL_UNSIGNED_INT,                 // data type
		(void*)0);                       // offset to indices

// deactivate attrib arrays
	glDisableVertexAttribArray(attribVertex);
	glDisableVertexAttribArray(attribNormal);
	glDisableVertexAttribArray(attribTexCoord);

	// unbind VBOs
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/*
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), &m_vertices[0]);

	glDrawElements(GL_TRIANGLES, (unsigned int)m_indices.size(), GL_UNSIGNED_INT, m_indices.data());

	glDisableClientState(GL_VERTEX_ARRAY);
	*/
}

void Sphere::createIndices() {
	unsigned int v1, v2;
	for (int i = 0; i < m_stackCount; ++i) { // the first vertices are one stackAngle down. (so increase first)
		v1 = i * m_sectorCount;
		v2 = (i + 1) * m_sectorCount;
		for (int j = 0; j < m_sectorCount; j++) {

			// indices
			//  v1--v1+1
			//  | \  |
			//  |  \ |
			//  v2--v2+1
			if (i != (m_stackCount - 1)) { // this is the corner to the left, we don't want this at the end
				m_indices.push_back(v1 + j);
				m_indices.push_back(v2 + j);
				m_indices.push_back(v2 + j + 1);
			}

			if (i != 0) { // this is the corner to the right, we don't want this at the beginning
				m_indices.push_back(v2 + j + 1);
				m_indices.push_back(v1 + j + 1);
				m_indices.push_back(v1 + j);
			}
		}
	}
}

const unsigned int* Sphere::getIndices() {
	return m_indices.data();
}

const unsigned int Sphere::getIndicesSize() {
	return (unsigned int)m_indices.data() * sizeof(unsigned int);
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

const float* Sphere::getVertices() {
	return m_vertices.data();
}

const unsigned int Sphere::getVerticesSize() {
	return (unsigned int)m_vertices.data() * sizeof(float);
}
