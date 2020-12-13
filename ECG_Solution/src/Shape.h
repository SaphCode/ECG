#pragma once

#include <vector>
#include <iostream>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/ext.hpp"

template <typename T>
class Shape
{
protected:
    Shape(glm::vec3 color) :
        m_pos_lq(0),
        m_model_lq(1),
        m_normal_lq(5),
        m_view_lq(2),
        m_proj_lq(3),
        m_color_lq(4),
        m_color(color),
        m_interleavedStride(24)
    {}

    virtual ~Shape() {
        // free VAO and VBO
        std::cout << "Deleting VBOs." << std::endl;
        glDeleteBuffers(1, &_vboID);
        glDeleteBuffers(1, &_iboID);
    }

    virtual void init() {
        glGenBuffers(1, &_vboID); // vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, _vboID);
        glBufferData(GL_ARRAY_BUFFER,
            m_interleavedVertices.size() * sizeof(float),
            &m_interleavedVertices[0],
            GL_STATIC_DRAW);

        glGenBuffers(1, &_iboID); // index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            m_indices.size() * sizeof(unsigned int),
            &m_indices[0],
            GL_STATIC_DRAW);
    }

    virtual void update(const glm::mat4& model) {
        glUniformMatrix4fv(m_model_lq, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(m_color_lq, 1, glm::value_ptr(m_color));
    }

    virtual void draw() {
        // activate attrib arrays
        glEnableVertexAttribArray(m_pos_lq); // should read data from vbo // here will be the layout qualifier
        glEnableVertexAttribArray(m_normal_lq);

        glVertexAttribPointer(
            m_pos_lq,
            3,
            GL_FLOAT,
            GL_FALSE,
            m_interleavedStride,
            (void*)0
        ); // how should the data be read
        glVertexAttribPointer(
            m_normal_lq,
            3,
            GL_FLOAT,
            GL_FALSE,
            m_interleavedStride,
            (void*)(sizeof(float) * 3)
        );

        // draw a sphere with VBO
        glDrawElements(GL_TRIANGLES,                    // primitive type
            m_indices.size(),				// # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);                    // offset to indices

        // deactivate attrib arrays
        glDisableVertexAttribArray(m_pos_lq);
        glDisableVertexAttribArray(m_normal_lq);
    }

    const unsigned int* getIndices() {
        return m_indices.data();
    }

    unsigned int getIndicesSize() const {
        return m_indices.size();
    }

    const float* getVertices() {
        return m_vertices.data();
    }

    unsigned int getVerticesSize() const {
        return m_vertices.size();
    }

    void addVertex(float x, float y, float z) {
        m_vertices.push_back(x);
        m_vertices.push_back(y);
        m_vertices.push_back(z);
    }

    void addIndex(int index) {
        m_indices.push_back(index);
    }

    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3) {
        m_indices.push_back(i1);
        m_indices.push_back(i2);
        m_indices.push_back(i3);
    }

    void addNormal(float nx, float ny, float nz) {
        m_normals.push_back(nx);
        m_normals.push_back(ny);
        m_normals.push_back(nz);
    }

    void bindVBO() {

        // bind VBOs
        glBindBuffer(GL_ARRAY_BUFFER, _vboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID);
    }

    void unbindVBO() {
        // unbind VBOs
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    static GLuint m_vaoID;
    static int m_numObjects;

private:
    virtual void createVertices() = 0;
    virtual void createIndices() = 0;
    virtual void createNormals() = 0;

    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<float> m_normals;
    std::vector<float> m_interleavedVertices;

    glm::vec3 m_color;

    const unsigned int m_interleavedStride;

    GLuint _vboID;
    GLuint _iboID;

    const GLuint m_pos_lq;
    const GLuint m_model_lq;
    const GLuint m_normal_lq;
    const GLuint m_view_lq;
    const GLuint m_proj_lq;
    const GLuint m_color_lq;
};

template <typename T> int Shape<T>::m_numObjects(0);
template <typename T> GLuint Shape<T>::m_vaoID;

