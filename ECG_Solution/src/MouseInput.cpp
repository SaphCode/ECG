#include "MouseInput.h"
#include <iostream>

void MouseInput::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    wheel_yZoom -= 0.1f * yoffset;
}

glm::vec2 MouseInput::getMouseDelta(GLFWwindow* window)
{
    glm::vec2 delta;
    if (leftDragEvent || rightDragEvent) {
        double mouse_x, mouse_y;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        delta = glm::vec2(mouse_x - mouse_x0, mouse_y - mouse_y0);
    }
    else delta = glm::vec2(0.0, 0.0);
    glfwGetCursorPos(window, &mouse_x0, &mouse_y0);
    return delta;
}

double MouseInput::getWheelYZoom()
{
    return wheel_yZoom;
}

void MouseInput::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            leftDragEvent = true;
            glfwGetCursorPos(window, &mouse_x0, &mouse_y0);
        }
        else if (action == GLFW_RELEASE) {
            leftDragEvent = false;
        }
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            rightDragEvent = true;
            glfwGetCursorPos(window, &mouse_x0, &mouse_y0);
        }
        else if (action == GLFW_RELEASE) {
            rightDragEvent = false;
        }
    }
}

double MouseInput::mouse_x0 = 0.0;
double MouseInput::mouse_y0 = 0.0;
double MouseInput::wheel_yZoom = 1;
bool MouseInput::rightDragEvent = false;
bool MouseInput::leftDragEvent = false;

