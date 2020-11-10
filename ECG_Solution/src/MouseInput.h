#pragma once

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>


class MouseInput
{
public:
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    static glm::vec2 getMouseDelta(GLFWwindow* window);

    static double getWheelYZoom();

    static bool rightDragEvent;
    static bool leftDragEvent;

private:
    static double mouse_x0;
    static double mouse_y0;
    static double wheel_yZoom;
    

    static double window_width, window_height;
};



