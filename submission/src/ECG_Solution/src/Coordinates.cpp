#include "Coordinates.h"

glm::vec3 Coordinates::getCartesianFromSpherical(float r, float phi, float theta)
{
    glm::vec3 cartesian(r * sin(theta) * cos(phi), r * sin(theta) * cos(phi), r * cos(theta));
    return cartesian;
}

glm::vec3 Coordinates::getCartesianFromSpherical(glm::vec3 spherical)
{
    glm::vec3 cartesian(spherical.x * sin(spherical.z) * cos(spherical.y), spherical.x * sin(spherical.z) * cos(spherical.y), spherical.x * cos(spherical.z));
    return cartesian;
}

glm::vec3 Coordinates::getSphericalFromCartesian(glm::vec3 cartesian)
{
    glm::vec3 spherical;
    spherical.x = std::sqrt(std::pow(cartesian.x, 2) + std::pow(cartesian.y, 2) + std::pow(cartesian.z, 2));
    spherical.y = std::atan2(spherical.y, spherical.x);
    spherical.z = acos(cartesian.z / spherical.x);
    return spherical;
}
