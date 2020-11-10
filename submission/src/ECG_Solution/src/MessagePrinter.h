#pragma once

#include <vector>
#include <string>
#include <iostream>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

class MessagePrinter
{
public:
	/* Prints the log output to console.
	*/
	static void PrintLog(std::vector<GLchar> log);
};

