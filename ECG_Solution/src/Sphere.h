#pragma once
#include "Actor.h"
#include <vector>
class Sphere :
    public Actor
{

public:
    Sphere(glm::vec3 center, float radius, glm::vec3 color, glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, int sectorCount, int stackCount);
    ~Sphere();

    virtual void update() override;
    virtual void render() override;

private:
    void createVertices();
    void createIndices();

    float m_radius;
    glm::vec3 m_color;
    std::vector<float> m_vertices;
    std::vector<float> m_indices;
    int m_sectorCount;
    int m_stackCount;
};
