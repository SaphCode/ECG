#pragma once
#include "Actor.h"
#include "Shape.h"
namespace GeomShape {
    class Rectangle :
        public Actor,
        public Shape<Rectangle>
    {
    public:
        Rectangle(glm::vec3 center, float width, float height, float depth, glm::vec3 color, glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle);
        ~Rectangle();

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

        void addIndicesLoop(unsigned int i1, unsigned int i2, unsigned int i3);

        float m_width;
        float m_height;
        float m_depth;
    };
}


