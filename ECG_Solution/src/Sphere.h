#pragma once

#include "Actor.h"
#include "Shape.h"
#include "Material.h"
#include <string>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

#include <vector>
namespace GeomShape {
    class Sphere :
        public Actor,
        public Shape<Sphere>
    {

    public:
        Sphere(glm::vec3 center, float radius, glm::vec3 color, glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, int sectorCount, int stackCount);
        ~Sphere();

        virtual void update() override;
        virtual void render() override;

        void setMaterial(Material material) {
            m_ka = material.ka;
            m_kd = material.kd;
            m_ks = material.ks;
            m_alpha = material.alpha;
        }


    private:
        void createVertices() override;
        void createIndices() override;
        void createNormals() override;

        float m_radius;

        int m_sectorCount;
        int m_stackCount;
    };
}

