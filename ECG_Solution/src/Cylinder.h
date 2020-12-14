#pragma once
#include "Shape.h"
#include "Actor.h"

namespace GeomShape {
    class Cylinder :
        public Shape<Cylinder>,
        public Actor
	{
    public:
        Cylinder(glm::vec3 center, float radius, float height, glm::vec3 color, glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, int sectorCount);
        ~Cylinder();

        virtual void update() override;
        virtual void render() override;

        void setMaterial(Material material) {
            m_ka = material.ka;
            m_kd = material.kd;
            m_ks = material.ks;
            m_alpha = material.alpha;
        }

        void setShading(std::string type) {
            if (type == "Gouraud") {
                m_gouraud = true;
            }
            else if (type == "Phong") {
                m_gouraud = false;
            }
            else {
                std::cout << "Not supported: " << type << std::endl;
            }
        }

    private:
        void createVertices() override;
        void createIndices() override;
        void createNormals() override;

        float m_radius;
        float m_height;
        int m_sectorCount;
    };
}


