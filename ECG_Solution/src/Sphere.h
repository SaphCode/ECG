#pragma once
#include "Actor.h"
// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
#include <vector>
class Sphere :
    public Actor
{

public:
    Sphere(glm::vec3 center, float radius, glm::vec3 color, glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, int sectorCount, int stackCount);
    ~Sphere();

    virtual void update() override;
    virtual void render() override;

    const unsigned int* getIndices();
    const unsigned int getIndicesSize();

    const float* getVertices();
    const unsigned int getVerticesSize();



private:
    void createVertices();
    void createIndices();

    float m_radius;
    glm::vec3 m_color;
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    int m_sectorCount;
    int m_stackCount;
    const GLuint m_model_lq;
    const GLuint m_color_lq;

    GLuint _vboID;
    GLuint _iboID;
};
