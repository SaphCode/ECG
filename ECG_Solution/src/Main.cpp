/*
* Copyright 2020 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/

#include "Utils.h"
#include "ShaderHandler.h"
#include "Shader.h"
#include "MessagePrinter.h"
#include "Teapot.h"
#include "MouseInput.h"
#include "Camera.h"
#include "Coordinates.h"
#include "Sphere.h"
#include "Rectangle.h"

#include <sstream>
#include <vector>
#include <chrono>
#include <thread>
#include <filesystem>
#include <iostream>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

/*
// GLOBALS
*/
bool wireFrameMode = false;
bool culling = true;

/* --------------------------------------------- */
// Prototypes
/* --------------------------------------------- */
static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "GLFW-Error: %s\n", description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // if escape is pressed, set the window close flag to GLFW_TRUE
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        if (wireFrameMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        wireFrameMode = !wireFrameMode;
    }
    if (key == GLFW_KEY_F2 && action == GLFW_PRESS) {
        if (culling) {
            glDisable(GL_CULL_FACE);
        }
        else {
            glEnable(GL_CULL_FACE);
        }
        culling = !culling;
    }
}

static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg) {
    std::stringstream stringStream;
    std::string sourceString;
    std::string typeString;
    std::string severityString;

    switch (source) {
    case GL_DEBUG_SOURCE_API: {
        sourceString = "API";
        break;
    }
    case GL_DEBUG_SOURCE_APPLICATION: {
        sourceString = "Application";
        break;
    }
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
        sourceString = "Window System";
        break;
    }
    case GL_DEBUG_SOURCE_SHADER_COMPILER: {
        sourceString = "Shader Compiler";
        break;
    }
    case GL_DEBUG_SOURCE_THIRD_PARTY: {
        sourceString = "Third Party";
        break;
    }
    case GL_DEBUG_SOURCE_OTHER: {
        sourceString = "Other";
        break;
    }
    default: {
        sourceString = "Unknown";
        break;
    }
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR: {
        typeString = "Error";
        break;
    }
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
        typeString = "Deprecated Behavior";
        break;
    }
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
        typeString = "Undefined Behavior";
        break;
    }
    case GL_DEBUG_TYPE_PORTABILITY_ARB: {
        typeString = "Portability";
        break;
    }
    case GL_DEBUG_TYPE_PERFORMANCE: {
        typeString = "Performance";
        break;
    }
    case GL_DEBUG_TYPE_OTHER: {
        typeString = "Other";
        break;
    }
    default: {
        typeString = "Unknown";
        break;
    }
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH: {
        severityString = "High";
        break;
    }
    case GL_DEBUG_SEVERITY_MEDIUM: {
        severityString = "Medium";
        break;
    }
    case GL_DEBUG_SEVERITY_LOW: {
        severityString = "Low";
        break;
    }
    default: {
        severityString = "Unknown";
        break;
    }
    }

    stringStream << "OpenGL Error: " << msg;
    stringStream << " [Source = " << sourceString;
    stringStream << ", Type = " << typeString;
    stringStream << ", Severity = " << severityString;
    stringStream << ", ID = " << id << "]";

    return stringStream.str();
}
static void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const GLvoid* userParam)
{
    if (id == 131185 || id == 131218) return; // ignore performance warnings (buffer uses GPU memory, shader recompilation) from nvidia
    std::string error = FormatDebugOutput(source, type, id, severity, message);
    std::cout << error << std::endl;
}
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
void testUpdateRender(GLFWwindow* window, Teapot& tp) {
    glClearColor(1, 1, 1, 1);
    glViewport(0, 0, 800, 800);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    tp.update();
    tp.render();
    glfwSwapBuffers(window);
}



/* --------------------------------------------- */
// Main
/* --------------------------------------------- */

int main(int argc, char** argv)
{
    std::string assets("assets/");
    std::filesystem::path cwd = std::filesystem::current_path();
    std::string laptop("ECG_Solution");

    if (cwd.string().find(laptop)) {
        assets = "../assets/";
    }
    std::cout << cwd.string() << std::endl;
	/* --------------------------------------------- */
	// Load settings.ini
	/* --------------------------------------------- */
    INIReader reader(assets + "settings.ini");

	// load dimensions of window
	const int width = reader.GetInteger("window", "width", 800);
	const int height = reader.GetInteger("window", "height", 800);
	// load title
	const std::string window_title = reader.Get("window", "title", "ECG 2020");

	/* --------------------------------------------- */
	// Init framework
	/* --------------------------------------------- */

	// Init GLFW
	// set error callback for GLFW
	glfwSetErrorCallback(glfw_error_callback);

	glewExperimental = true; // needed for core profile
	if (!glfwInit()) {
		// Initialization failed, I don't think it does the callback in this case.
		// Exit for safety.
		exit(EXIT_FAILURE);
	}

	#if _DEBUG
		// Create a debug OpenGL context or tell your OpenGL library (GLFW, SDL) to do so.
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	#endif

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // max for my graphics card, can't specify a double here, as it will get converted to int 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4); // min for course
    glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow* window = glfwCreateWindow(width, height, window_title.c_str(), NULL, NULL);
	if (!window) {
        
        glfwTerminate();
        EXIT_WITH_ERROR("Failed to create window.\n"); // see post: https://tuwel.tuwien.ac.at/mod/forum/discuss.php?d=199777, this now terminates glfw itself
		//exit(EXIT_FAILURE);
	}
	
	// set key callback for window
	glfwSetKeyCallback(window, key_callback);
	// make the context current
	glfwMakeContextCurrent(window);

	// Swap interval for buffer swapping, see also after drawing.
	// Don't waste computing power, swap 1 is recommended. Can also lead to other problems.
	glfwSwapInterval(1);

	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
        glfwDestroyWindow(window);
        glfwTerminate();
		EXIT_WITH_ERROR("Failed to initialize GLEW.\n");
	}

    #if _DEBUG
        // Register your callback function.
        glDebugMessageCallback(DebugCallback, NULL);
        // Enable synchronous callback. This ensures that your callback function is called
        // right after an error has occurred. 
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    #endif

    /* --------------------------------------------- */
    // Create and compile shaders
    /* --------------------------------------------- */

    // Creating a uniform color.


    //GLuint vao, vbo[1];

    //const int color_nrows = std::extent<GLfloat[3][3], 0>::value;
    //const int color_ncols = std::extent<GLfloat[3][3], 1>::value;
    //const int color_elements = color_nrows * color_ncols;
    //const u_int color_size = sizeof(GLfloat) * color_elements;

    
    
    //const u_int ex_color_lq = 1;
    

    /*
    glGenVertexArrays(1, &vao); // allocate & assigne vao
    glBindVertexArray(vao); // bind as current used obj
    glGenBuffers(1, vbo); // allocate and assign 1 vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // first vbo is active buffer now
    glBufferData(GL_ARRAY_BUFFER, color_size, firstTeapotColors, GL_STATIC_DRAW); // copy vertex data to buffer
    glVertexAttribPointer(ex_color_lq, color_ncols, GL_FLOAT, GL_FALSE, 0, 0); // 3 cols per vertex, should be ncols,
                                                                     // specifies where this data is going (1 here)
    glEnableVertexAttribArray(ex_color_lq);
    */
    

    std::string shaderDir = assets;
    ShaderHandler sh;

    Shader vertexShader(ShaderType::VERTEX, shaderDir + "simpleGeometry.vert");
    Shader fragmentShader(ShaderType::FRAGMENT, shaderDir + "simpleGeometry.frag");

    std::vector<Shader> shaders;
    shaders.push_back(vertexShader);
    shaders.push_back(fragmentShader);

    // Once the shaders are compiled, can link them in program.
    // Get a program object.
    GLuint program = glCreateProgram();

    bool success = sh.TryLoadShaders(shaders);
    if (!success) {
        std::string msg("Loading shaders failed.");
        glDeleteProgram(program);
        glfwDestroyWindow(window);
        glfwTerminate();
        EXIT_WITH_ERROR(msg);
    }
    else {
        sh.AttachToProgram(program);
    }

    // WARNING: can only do the following:
    // https://www.khronos.org/opengl/wiki/Shader_Compilation
    // Before linking!!!!!

    // glBindAttribLocation should be unneccessary bc were using layout qualifiers

    // Link the program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);
        // Don't leak shaders either.
        sh.DeleteAllShaders();

        // Use the infoLog as you see fit.
        MessagePrinter::PrintLog(infoLog);

        glfwDestroyWindow(window);
        glfwTerminate();

        // In this simple program, we'll just leave
        EXIT_WITH_ERROR("Program linking failed.");
    }

	// Init Framework
	if (!initFramework()) {
        sh.DetachFromProgram(program);
        // We don't need the program anymore.
        glDeleteProgram(program);
        // Don't leak shaders either.
        sh.DeleteAllShaders();

        glfwDestroyWindow(window);
        glfwTerminate();
		EXIT_WITH_ERROR("Failed to init framework"); // see post: https://tuwel.tuwien.ac.at/mod/forum/discuss.php?d=199777, this now terminates glfw itself
	}

    // Mouse input
    glfwSetMouseButtonCallback(window, MouseInput::mouse_button_callback);
    glfwSetScrollCallback(window, MouseInput::scroll_callback);

    glEnable(GL_CULL_FACE);


	/* --------------------------------------------- */
	// Initialize scene and render loop
	/* --------------------------------------------- */
	{
        // making a camera that moves on a sphere of radius 6
        glm::vec3 upVector_world = glm::vec3(0.0, 1.0, 0.0);

        glm::vec3 zHat(0.0, 0.0, 1.0);

        glm::vec3 eye = glm::vec3(0.0, 0.0, 6.0);
        glm::vec3 lookAt = glm::vec3(0.0, 0.0, 0.0); // looking at origin
        Camera camera(eye, lookAt, upVector_world);

        // uniform color
        const glm::vec3 color_red(0.8, 0.1, 0.2); // red
        const glm::vec3 color_blue(0.4, 0.3, 0.8); // blue

        GeomShape::Sphere sphere(glm::vec3(0,0,0), 0.6, glm::vec3(0.4,0.3,0.8), glm::vec3(1,1.7,1), zHat, 0.f, 16, 8);
        GeomShape::Rectangle rect(glm::vec3(0, 0, 0), 1.2, 2.0, 1.2, glm::vec3(0.8, 0.1, 0.2), glm::vec3(1, 1, 1), upVector_world, 45.f);
        rect.setPosition(glm::vec3(2.f, 0.f, 0.f));

        std::vector<std::unique_ptr<Actor>> actors;
        actors.push_back(std::make_unique<GeomShape::Sphere>(std::move(sphere)));
        actors.push_back(std::make_unique<GeomShape::Rectangle>(std::move(rect)));

        float dPhi = (2 * 3.14159) / width;
        float dTheta = (2 * 3.14159) / height;

        // projection matrix
        // load camera params
        float cam_fov = reader.GetReal("camera", "fov", 60.0); // this is an angle in deg
        float cam_near = reader.GetReal("camera", "near", 0.1);
        float cam_far = reader.GetReal("camera", "far", 100.0);      

        float wheel_yZoom = 0;

        glEnable(GL_DEPTH_TEST);

        double wheelYZoom = MouseInput::getWheelYZoom();

		// as long as the user doesn't close the window
		while (!glfwWindowShouldClose(window)) {
			// Clear the screen
			glClearColor(1, 1, 1, 1);
            glViewport(0, 0, width, height);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // This usually happens at the start of every frame, before input is processed, by using the following GLFW function:
            // glfwPollEvents(): This is the best choice when rendering continually, like most games do. 
            glfwPollEvents(); // If instead you only need to update your rendering once you have received new input, glfwWaitEvents is a better choice.

            // here get changes to projection matrix, but there shouldn't be any (in another version, zoom could affect fov)

            glm::mat4 projectionMatrix = glm::perspective(glm::radians(cam_fov), (float)width / (float)height, cam_near, cam_far);
            
            // here compute the new camera position and stuff

            glm::vec4 cam_pos = glm::vec4(camera.getEye(), 1.0);
            glm::vec4 cam_pivot = glm::vec4(camera.getLookAt(), 1.0);

            glm::vec2 mouseDelta = MouseInput::getMouseDelta(window);
            if (MouseInput::leftDragEvent) {
                float deltaPhi = mouseDelta.x * dPhi;
                float deltaTheta = mouseDelta.y * dTheta;
                float cosAngle = glm::dot(camera.getViewDir(), upVector_world); // here something is wrong
                if (cosAngle * sgn(-deltaTheta) > 0.99f)
                    deltaTheta = 0;

                glm::mat4 rotationMatrixPhi(1.0f);
                rotationMatrixPhi = glm::rotate(rotationMatrixPhi, -deltaPhi, upVector_world); // here something is wrong.
                cam_pos = (rotationMatrixPhi * (cam_pos - cam_pivot)) + cam_pivot;

                glm::mat4 rotationMatrixTheta(1.0f);
                rotationMatrixTheta = glm::rotate(rotationMatrixTheta, -deltaTheta, camera.getRightVector());
                cam_pos = (rotationMatrixTheta * (cam_pos - cam_pivot)) + cam_pivot;
            }

            double zoomDiff = MouseInput::getWheelYZoom() - wheelYZoom;
            wheelYZoom = MouseInput::getWheelYZoom();
            if (zoomDiff != 0) {
                cam_pos = glm::vec4((1 + zoomDiff) * glm::vec3(cam_pos), 1.0);
            }

            if (MouseInput::rightDragEvent) {
                glm::mat4 T;
                glm::vec3 translate = 0.03f * mouseDelta.x * camera.getRightVector() + 0.03f * -mouseDelta.y * camera.getUpVector();
                T = glm::translate(T, -translate);
                cam_pos = T * cam_pos;
                cam_pivot = T * cam_pivot;
            }

            camera.setCameraView(cam_pos, cam_pivot, upVector_world);
            glm::mat4 viewMatrix = camera.getViewMatrix();
            
            glUseProgram(program);
            u_int view_lq = 2;
            u_int projection_lq = 3;
            glUniformMatrix4fv(view_lq, 1, GL_FALSE, glm::value_ptr(viewMatrix));
            glUniformMatrix4fv(projection_lq, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

            for (auto& a : actors) {
                a->update();
                a->render();
            }
         
            glUseProgram(0);

			// Swap buffers: back buffer is for rendering, front for drawing, when you have drawn, you need to swap
			glfwSwapBuffers(window);
		}

        glUseProgram(0);
        //glDisableVertexAttribArray(ex_color_lq);
        sh.DetachFromProgram(program);
        glDeleteProgram(program);
        sh.DeleteAllShaders();
        //glDeleteBuffers(1, vbo);
        //glDeleteVertexArrays(1, &vao);
        // after window should close, actually close it
        glfwDestroyWindow(window);

	}

	/* --------------------------------------------- */
	// Destroy framework
	/* --------------------------------------------- */

	destroyFramework();

	/* --------------------------------------------- */
	// Destroy context and exit
	/* --------------------------------------------- */

    // terminate GLFW
    glfwTerminate(); // window is destroyed too here

	return EXIT_SUCCESS;
}






