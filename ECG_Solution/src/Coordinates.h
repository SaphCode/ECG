#pragma once

#include <glm/glm.hpp>

class Coordinates
{
public:
    static glm::vec3 getCartesianFromSpherical(float r, float phi, float theta);
    static glm::vec3 getCartesianFromSpherical(glm::vec3 spherical);

    static glm::vec3 getSphericalFromCartesian(glm::vec3 cartesian);
    
};




