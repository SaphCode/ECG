#pragma once

#include "Light.h"

class PointLight :
    public Light
{
public:
    PointLight() {

    }
    PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 attenuation)
        : Light(ambient, diffuse, specular),
        m_position(position),
        m_attenuation(attenuation) {

    }

    ~PointLight() {}

    glm::vec3 getPosition() const {
        return m_position;
    }

    glm::vec3 getAttenuation() const {
        return m_attenuation;
    }

private:
    glm::vec3 m_position;
    glm::vec3 m_attenuation;

};

