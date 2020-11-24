#pragma once
#include "Shape.h"
#include "Actor.h"

namespace GeomShape {
	class Cylinder :
		public Shape,
        public Actor
	{
    public:
        Cylinder(glm::vec3 center, float radius, float height, glm::vec3 color, glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, int sectorCount);
        ~Cylinder();

        virtual void update() override;
        virtual void render() override;

    private:
        void createVertices();
        void createIndices();

        float m_radius;
        float m_height;
        int m_sectorCount;
		
		static GLuint _vaoID;
        static int numObjects;
    };
}


