#pragma once

enum class ErrorType {
    COMPILATION_ERROR,
};

#include <vector>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

class IErrorHandler
{

public:
    virtual ~IErrorHandler() {}
    virtual bool TryHandleError(std::vector<GLchar> log, ErrorType type) = 0; //, ErrorType type) = 0;

protected:
    virtual void PrintLog(std::vector<GLchar> log) = 0;  //, ErrorType type) = 0;

};




