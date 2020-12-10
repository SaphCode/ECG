#include "Rectangle.h"
#include <map>

GeomShape::Rectangle::Rectangle(glm::vec3 center, float width, float height, float depth, glm::vec3 color, glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle)
	:
	Actor(center, scale, rotationAxis, rotationAngle),
	Shape(color),
	m_width(width),
	m_height(height),
	m_depth(depth)
{
	createVertices();
	createNormals();
	createIndices();

	if (m_numObjects == 0) {
		std::cout << "Generating VAO for Rectangle." << std::endl;
		glGenVertexArrays(1, &m_vaoID);
	}
	m_numObjects += 1;

	init();
}

GeomShape::Rectangle::~Rectangle()
{
	// virtual base destructors are always called after destructing derived
	if (m_numObjects == 1) {
		std::cout << "Deleting VAO for Rectangle." << std::endl;
		glDeleteVertexArrays(1, &m_vaoID);
	}
	m_numObjects -= 1;
	assert(m_numObjects >= 0);
}

void GeomShape::Rectangle::update()
{
	Actor::update();
	Shape::update(m_model);
}

void GeomShape::Rectangle::render()
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

void GeomShape::Rectangle::createVertices()
{
	float x = m_width / 2;
	float y = m_height / 2;
	float z = m_depth / 2;

	addVertex(x, y, z);
	addVertex(x, y, z);
	addVertex(x, y, z);

	addVertex(-x, y, z);
	addVertex(-x, y, z);
	addVertex(-x, y, z);

	addVertex(-x, -y, z);
	addVertex(-x, -y, z);
	addVertex(-x, -y, z);

	addVertex(x, -y, z);
	addVertex(x, -y, z);
	addVertex(x, -y, z);

	addVertex(x, y, -z);
	addVertex(x, y, -z);
	addVertex(x, y, -z);

	addVertex(-x, y, -z);
	addVertex(-x, y, -z);
	addVertex(-x, y, -z);

	addVertex(-x, -y, -z);
	addVertex(-x, -y, -z);
	addVertex(-x, -y, -z);

	addVertex(x, -y, -z);
	addVertex(x, -y, -z); 
	addVertex(x, -y, -z);

	std::cout << "Rectangle vertices: " << 8 * 3 << " expected vs " << getVerticesSize() / 3 << "\n";
}

void GeomShape::Rectangle::createNormals() {
	const float nx = 1;
	const float ny = 1;
	const float nz = 1;

	addNormal(nx, 0, 0);
	addNormal(0, ny, 0);
	addNormal(0, 0, nz);

	addNormal(-nx, 0, 0);
	addNormal(0, ny, 0);
	addNormal(0, 0, nz);

	addNormal(-nx, 0, 0);
	addNormal(0, -ny, 0);
	addNormal(0, 0, nz);

	addNormal(nx, 0, 0);
	addNormal(0, -ny, 0);
	addNormal(0, 0, nz);

	addNormal(nx, 0, 0);
	addNormal(0, ny, 0);
	addNormal(0, 0, -nz);

	addNormal(-nx, 0, 0);
	addNormal(0, ny, 0);
	addNormal(0, 0, -nz);

	addNormal(-nx, 0, 0);
	addNormal(0, -ny, 0);
	addNormal(0, 0, -nz);

	addNormal(nx, 0, 0);
	addNormal(0, -ny, 0);
	addNormal(0, 0, -nz);
}

void GeomShape::Rectangle::createIndices()
{
	unsigned int counter = 0;
	unsigned int adjust = 6;

	std::map<unsigned int, unsigned int> mapping;
	for (int i = 0; i < 8; i++) {
		mapping[i] = 3 * i;
	}


	// front face
	addIndicesLoop(mapping[0], mapping[1], mapping[2]);
	addIndicesLoop(mapping[2], mapping[3], mapping[0]);

	// up face
	addIndicesLoop(mapping[0], mapping[4], mapping[5]);
	addIndicesLoop(mapping[5], mapping[1], mapping[0]);

	// right face
	addIndicesLoop(mapping[4], mapping[0], mapping[3]);
	addIndicesLoop(mapping[3], mapping[7], mapping[4]);

	// back face
	addIndicesLoop(mapping[5], mapping[4], mapping[7]);
	addIndicesLoop(mapping[7], mapping[6], mapping[5]);

	// left face (other later)
	addIndicesLoop(mapping[1], mapping[5], mapping[6]);

	// down face
	addIndicesLoop(mapping[6], mapping[7], mapping[3]);
	addIndicesLoop(mapping[3], mapping[2], mapping[6]);
	
	// left face last piece
	addIndicesLoop(mapping[6], mapping[2], mapping[1]);

	std::cout << "Rectangle triangles: " << 12 * 3 << " expected vs " << getIndicesSize() / 3 << "\n";
}


void GeomShape::Rectangle::addIndicesLoop(unsigned int i1, unsigned int i2, unsigned int i3) {
	for (int i = 0; i < 3; i++) {
		addIndices(i1 + i, i2 + i, i3 + i);
	}
}