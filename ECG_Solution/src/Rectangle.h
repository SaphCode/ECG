#pragma once
#include "Actor.h"
#include "Shape.h"
namespace GeomShape {
    class Rectangle :
        public Actor,
        public Shape
    {
    public:
        Rectangle(glm::vec3 center, float width, float height, float depth, glm::vec3 color, glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle);
        ~Rectangle();

        virtual void update() override;
        virtual void render() override;

    private:
        void createVertices() override;
        void createIndices() override;

        float m_width;
        float m_height;
        float m_depth;

        static GLuint _vaoID;
        static int numObjects;
    };
}


