#pragma once
#include "Light.h"
class DirectionalLight :
    public Light
{
public:

    DirectionalLight() {

    }
    DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction)
        : Light(ambient, diffuse, specular),
        m_direction(direction)
    {

    }

    glm::vec3 getDirection() const {
        return m_direction;
    }

    ~DirectionalLight() {

    }

private:
    glm::vec3 m_direction;
    
};

