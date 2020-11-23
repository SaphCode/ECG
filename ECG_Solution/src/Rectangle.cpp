#include "Rectangle.h"

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

	init();
}

GeomShape::Rectangle::~Rectangle()
{
	// virtual base destructors are always called
}

void GeomShape::Rectangle::update()
{
	Actor::update();
	Shape::update(m_model);
}

void GeomShape::Rectangle::render()
{
	Shape::render();
}

void GeomShape::Rectangle::createVertices()
{
	float x = m_width / 2;
	float y = -m_height / 2;
	float z = m_depth / 2;

	addVertex(x, y, z);
	addVertex(-x, y, z);
	addVertex(-x, -y, z);
	addVertex(x, -y, z);

	addVertex(x, y, -z);
	addVertex(-x, y, -z);
	addVertex(-x, -y, -z);
	addVertex(x, -y, -z);
}

void GeomShape::Rectangle::createIndices()
{
	// left face
	addIndex(1);
	addIndex(4);
	addIndex(5);

	addIndex(5);
	addIndex(2);
	addIndex(1);

	// front face
	addIndex(0);
	addIndex(1);
	addIndex(2);

	addIndex(2);
	addIndex(3);
	addIndex(0);

	// back face
	addIndex(4);
	addIndex(7);
	addIndex(6);

	addIndex(6);
	addIndex(5);
	addIndex(4);

	// right face
	addIndex(7);
	addIndex(0);
	addIndex(3);

	addIndex(3);
	addIndex(6);
	addIndex(7);

	// up face
	addIndex(7);
	addIndex(4);
	addIndex(1);

	addIndex(1);
	addIndex(0);
	addIndex(7);

	// down face
	addIndex(3);
	addIndex(2);
	addIndex(5);

	addIndex(5);
	addIndex(6);
	addIndex(3);
}
