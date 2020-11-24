#include "Rectangle.h"

int numObjects = 0;

GeomShape::Rectangle::Rectangle(glm::vec3 center, float width, float height, float depth, glm::vec3 color, glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle)
	:
	Actor(center, scale, rotationAxis, rotationAngle),
	Shape(color),
	m_width(width),
	m_height(height),
	m_depth(depth)
{
	createVertices();
	createIndices();

	if (numObjects == 0) {
		std::cout << "Generating VAO for Rectangle." << std::endl;
		glGenVertexArrays(1, &_vaoID);
	}
	numObjects += 1;

	init();
}

GeomShape::Rectangle::~Rectangle()
{
	// virtual base destructors are always called after destructing derived
	if (numObjects == 1) {
		std::cout << "Deleting VAO for Rectangle." << std::endl;
		glDeleteVertexArrays(1, &_vaoID);
	}
	numObjects -= 1;
	assert(numObjects >= 0);
}

void GeomShape::Rectangle::update()
{
	Actor::update();
	Shape::update(m_model);
}

void GeomShape::Rectangle::render()
{
	// bind VAO
	glBindVertexArray(_vaoID); // bind
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
	addVertex(-x, y, z);
	addVertex(-x, -y, z);
	addVertex(x, -y, z);

	addVertex(x, y, -z);
	addVertex(-x, y, -z);
	addVertex(-x, -y, -z);
	addVertex(x, -y, -z);

	std::cout << "Rectangle vertices: " << 8 << " expected vs " << getVerticesSize() / 3 << "\n";
}

void GeomShape::Rectangle::createIndices()
{
	// front face
	addIndex(0);
	addIndex(1);
	addIndex(2);

	addIndex(2);
	addIndex(3);
	addIndex(0);

	// up face
	addIndex(0);
	addIndex(4);
	addIndex(5);

	addIndex(5);
	addIndex(1);
	addIndex(0);

	// right face
	addIndex(4);
	addIndex(0);
	addIndex(3);

	addIndex(3);
	addIndex(7);
	addIndex(4);

	// back face
	addIndex(5);
	addIndex(4);
	addIndex(7);

	addIndex(7);
	addIndex(6);
	addIndex(5);

	// left face
	addIndex(1);
	addIndex(5);
	addIndex(6);

	// down face
	addIndex(6);
	addIndex(7);
	addIndex(3);

	addIndex(3);
	addIndex(2);
	addIndex(6);

	// left face last piece
	addIndex(6);
	addIndex(2);
	addIndex(1);

	std::cout << "Rectangle triangles: " << 12 << " expected vs " << getIndicesSize() / 3 << "\n";
}
