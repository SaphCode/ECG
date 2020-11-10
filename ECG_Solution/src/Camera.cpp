#include "Camera.h"
#include <iostream>

#include <glm/ext.hpp>

Camera::Camera(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 upVector) :
    m_eye(std::move(eye)),
    m_lookAt(std::move(lookAt)),
    m_upVector(std::move(upVector))
{
    updateViewMatrix();
}

void Camera::setCameraView(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 upVector) 
{
    m_eye = std::move(eye); // moves the object here (this belongs to this now), maybe this is more optimal?
    m_lookAt = std::move(lookAt);
    m_upVector = std::move(upVector);

    updateViewMatrix();
}



void Camera::updateViewMatrix()
{

    glm::vec3  f = glm::normalize(m_lookAt - m_eye);
    glm::vec3  u = glm::normalize(m_upVector);
    glm::vec3  s = glm::normalize(glm::cross(f, u));
    u = m_upVector = glm::cross(s, f);

    glm::mat4 coordinateSystem( // this is left handed. if i make it right handed it is again wrong.
        s.x, s.y, s.z, 0.0,
        u.x, u.y, u.z, 0.0,
        -f.x, -f.y, -f.z, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    glm::mat4 translation( // might this matrix be wrong?
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        m_eye.x, m_eye.y, m_eye.z, 1.0
    );

    glm::mat4 M = translation * coordinateSystem;
    m_viewMatrix = glm::inverse(M);


    /*
    glm::mat4 Result(1);
    Result[0][0] = s.x;
    Result[1][0] = s.y;
    Result[2][0] = s.z;
    Result[0][1] = u.x;
    Result[1][1] = u.y;
    Result[2][1] = u.z;
    Result[0][2] = -f.x;
    Result[1][2] = -f.y;
    Result[2][2] = -f.z;
    Result[3][0] = -glm::dot(s, m_eye);
    Result[3][1] = -glm::dot(u, m_eye);
    Result[3][2] = glm::dot(f, m_eye);
    m_viewMatrix = Result;

    /*
    // translate & rotate

    glm::vec3  z = glm::normalize(m_lookAt - m_eye);
    glm::vec3  y = glm::normalize(m_upVector);
    glm::vec3  x = glm::normalize(glm::cross(z, y));
    y = m_upVector = glm::cross(x, z);

    /*glm::mat4 M( // this is left handed. if i make it right handed it is again wrong.
        x.x, y.x, -z.x, 0.0,
        x.y, y.y, -z.y, 0.0,
        x.z, y.z, -z.z, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
    glm::mat4 translation;
    translation = glm::translate(translation, -m_eye);

    glm::mat4 CameraToOrigin = glm::mat4(
        x.x, y.x, -z.x, m_eye.x,
        x.y, y.y, -z.y, m_eye.y,
        x.z, y.z, -z.z, m_eye.z,
        0.0, 0.0, 0.0, 1.0
    );

    //glm::mat4 CameraToOrigin = rotation * translation;
    m_viewMatrix = glm::inverse(CameraToOrigin);

    /*glm::mat4 translation( // might this matrix be wrong?
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        -m_eye.x, -m_eye.y, -m_eye.z, 1.0
    );

   
    /*
    // RIGHT HANDED COORDINATE SYSTEM
    glm::vec3  f = glm::normalize(m_lookAt - m_eye);
    glm::vec3  u = glm::normalize(m_upVector);
    glm::vec3  s = glm::normalize(glm::cross(f, u));
    u = m_upVector = glm::cross(s, f);

    glm::mat4 coordinateSystem( // this is left handed. if i make it right handed it is again wrong.
        s.x, s.y, s.z, 0.0,
        u.x, u.y, u.z, 0.0,
        -f.x, -f.y, -f.z, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    glm::mat4 translation( // might this matrix be wrong?
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        m_eye.x, m_eye.y, m_eye.z, 1.0
    );

    glm::mat4 M = translation * coordinateSystem;
    m_viewMatrix = glm::inverse(M);
    
    
    /*
    glm::mat4 Result(1);
    Result[0][0] = s.x;
    Result[1][0] = s.y;
    Result[2][0] = s.z;
    Result[0][1] = u.x;
    Result[1][1] = u.y;
    Result[2][1] = u.z;
    Result[0][2] = -f.x;
    Result[1][2] = -f.y;
    Result[2][2] = -f.z;
    Result[3][0] = -glm::dot(s, m_eye);
    Result[3][1] = -glm::dot(u, m_eye);
    Result[3][2] = glm::dot(f, m_eye);
    m_viewMatrix = Result;
    
    /*
    //glm::vec3 zaxis = glm::normalize(m_lookAt - m_eye);
    //glm::vec3 xaxis = glm::normalize(glm::cross(zaxis, m_upVector));
    //glm::vec3 yaxis = glm::cross(xaxis, zaxis);

    //zaxis *= -1; // negate the zaxis to go from left-handed coordinate system to right handed

    glm::lookAt(m_eye, m_lookAt, m_upVector);

    glm::vec3 direction = glm::normalize(m_lookAt - m_eye);
    glm::vec3 up = glm::normalize(m_upVector); // make sure it is really normalized
    glm::vec3 right = glm::normalize(glm::cross(direction, up));
    up = glm::cross(right, direction);
    
    
    glm::mat4 rotation = glm::mat4(
        right.x, up.x, direction.x, 0.0,
        right.y, up.y, direction.y, 0.0,
        right.z, up.z, direction.z, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
    
    glm::mat4 translation = glm::mat4(
        1.0, 0.0, 0.0, m_eye.x,
        0.0, 1.0, 0.0, m_eye.y,
        0.0, 0.0, 1.0, m_eye.z,
        0.0, 0.0, 0.0, 1.0
    );

    std::cout << "View direction x: " << direction.x << std::endl;
    std::cout << "View direction y: " << direction.y << std::endl;
    std::cout << "View direction z: " << direction.z << std::endl;
    /*
    glm::mat4 rotation = glm::mat4(
        xaxis.x, yaxis.x, zaxis.x, 0.0,
        xaxis.y, yaxis.y, zaxis.y, 0.0,
        xaxis.z, yaxis.z, zaxis.z, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
    glm::mat4 translation = glm::mat4(
        1.0, 0.0, 0.0, m_eye.x,
        0.0, 1.0, 0.0, m_eye.y,
        0.0, 0.0, 1.0, m_eye.z,
        0.0, 0.0, 0.0, 1.0
    );
    m_viewMatrix = glm::inverse(translation * rotation); //glm::inverse(

    */
}
