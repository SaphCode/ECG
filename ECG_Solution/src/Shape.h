#pragma once

#include <vector>
#include <iostream>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/ext.hpp"

template <typename T>
class Shape
{
public:
    virtual void setDirectionalLight(DirectionalLight dirLight) {
        m_dirLight = dirLight;
    }

    virtual void setPointLight(PointLight pointLight) {
        m_pointLight = pointLight;
    }

protected:
    Shape(glm::vec3 color) :
        m_pos_lq(0),
        m_model_lq(1),
        m_normal_lq(5),
        m_view_lq(2),
        m_proj_lq(3),
        m_color_lq(4),
        m_color(color),
        m_interleavedStride(24),
        m_direction_lq(6),
        m_dir_ambient_lq(7),
        m_dir_diffuse_lq(8),
        m_dir_specular_lq(9),
        m_pointLight_position_lq(10),
        m_pointLight_attConst_lq(11),
        m_pointLight_attLin_lq(12),
        m_pointLight_attQuad_lq(13),
        m_pointLight_ambient_lq(14),
        m_pointLight_diffuse_lq(15),
        m_pointLight_specular_lq(16),
        m_ka(0.1),
        m_kd(0.9),
        m_ks(0.3),
        m_alpha(10),
        m_alpha_lq(17),
        m_ka_lq(18),
        m_kd_lq(19),
        m_ks_lq(20)
    {}

    virtual ~Shape() {
        // free VAO and VBO
        std::cout << "Deleting VBOs." << std::endl;
        glDeleteBuffers(1, &_vboID);
        glDeleteBuffers(1, &_iboID);
    }

    void light(DirectionalLight dirS, PointLight pointS) {

        // set material
        glUniform1f(m_alpha_lq, m_alpha);
        glUniform1f(m_ka_lq, m_ka);
        glUniform1f(m_kd_lq, m_kd);
        glUniform1f(m_ks_lq, m_ks);

        //for (auto& s : dirSources) {
        glm::vec3 direction = dirS.getDirection();
        glm::vec3 ambient = dirS.getAmbient();
        glm::vec3 diffuse = dirS.getDiffuse();
        glm::vec3 specular = dirS.getSpecular();
        glUniform3fv(m_direction_lq, 1, glm::value_ptr(direction));
        glUniform3fv(m_dir_ambient_lq, 1, glm::value_ptr(ambient));
        glUniform3fv(m_dir_diffuse_lq, 1, glm::value_ptr(diffuse));
        glUniform3fv(m_dir_specular_lq, 1, glm::value_ptr(specular));
        //}

        unsigned int it = 0;
        //for (auto& s : pointSources) {
        glm::vec3 position = pointS.getPosition();
        ambient = pointS.getAmbient();
        diffuse = pointS.getDiffuse();
        specular = pointS.getSpecular();
        glm::vec3 attenuation = pointS.getAttenuation();
        glUniform3fv(m_pointLight_position_lq + it, 1, glm::value_ptr(position));
        glUniform3fv(m_pointLight_ambient_lq + it, 1, glm::value_ptr(ambient));
        glUniform3fv(m_pointLight_diffuse_lq + it, 1, glm::value_ptr(diffuse));
        glUniform3fv(m_pointLight_specular_lq + it, 1, glm::value_ptr(specular));
        glUniform1f(m_pointLight_attConst_lq + it, attenuation.x);
        glUniform1f(m_pointLight_attLin_lq + it, attenuation.y);
        glUniform1f(m_pointLight_attQuad_lq + it, attenuation.z);
        //it += 7;
        //}
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
        light(m_dirLight, m_pointLight);
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

    void buildInterleavedVertices() {
        size_t vertices = m_vertices.size();
        assert(vertices == m_normals.size());

        m_interleavedVertices.clear();
        for (int i = 0; i < vertices; i += 3) {
            m_interleavedVertices.push_back(m_vertices[i]);
            m_interleavedVertices.push_back(m_vertices[i+1]);
            m_interleavedVertices.push_back(m_vertices[i+2]);

            m_interleavedVertices.push_back(m_normals[i]);
            m_interleavedVertices.push_back(m_normals[i+1]);
            m_interleavedVertices.push_back(m_normals[i+2]);
        }
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

    float m_ka, m_kd, m_ks, m_alpha;

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

    DirectionalLight m_dirLight;
    PointLight m_pointLight;

    const GLuint m_pos_lq;
    const GLuint m_model_lq;
    const GLuint m_normal_lq;
    const GLuint m_view_lq;
    const GLuint m_proj_lq;
    const GLuint m_color_lq;
    const GLuint m_alpha_lq;
    const GLuint m_ka_lq;
    const GLuint m_kd_lq;
    const GLuint m_ks_lq;

    const GLuint m_direction_lq;
    const GLuint m_dir_ambient_lq;
    const GLuint m_dir_diffuse_lq;
    const GLuint m_dir_specular_lq;

    const GLuint m_pointLight_position_lq, m_pointLight_attConst_lq, m_pointLight_attLin_lq,
        m_pointLight_attQuad_lq, m_pointLight_ambient_lq, m_pointLight_diffuse_lq, m_pointLight_specular_lq;
};

template <typename T> int Shape<T>::m_numObjects(0);
template <typename T> GLuint Shape<T>::m_vaoID;

