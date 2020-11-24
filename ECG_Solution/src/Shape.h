#pragma once

#include <vector>
#include <iostream>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/ext.hpp"

class Shape
{
protected:
    Shape(glm::vec3 color) :
        m_pos_lq(0),
        m_model_lq(1),
        m_view_lq(2),
        m_proj_lq(3),
        m_color_lq(4),
        m_color(color)
    {}

    virtual ~Shape() {
        // free VAO and VBO
        glDeleteBuffers(1, &_vboID);
        glDeleteBuffers(1, &_iboID);
    }

    virtual void init() {
        glGenBuffers(1, &_vboID); // vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, _vboID);
        glBufferData(GL_ARRAY_BUFFER,
            m_vertices.size() * sizeof(float),
            &m_vertices[0],
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

        glVertexAttribPointer(m_pos_lq, 3, GL_FLOAT, false, 0, (void*)0); // how should the data be read

        // draw a sphere with VBO
        glDrawElements(GL_TRIANGLES,                    // primitive type
            getIndicesSize(),				// # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);                    // offset to indices

        // deactivate attrib arrays
        glDisableVertexAttribArray(m_pos_lq);
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

private:
    virtual void createVertices() = 0;
    virtual void createIndices() = 0;

    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;

    glm::vec3 m_color;

    GLuint _vboID;
    GLuint _iboID;

    const GLuint m_pos_lq;
    const GLuint m_model_lq;
    const GLuint m_view_lq;
    const GLuint m_proj_lq;
    const GLuint m_color_lq;
};

