#pragma once

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

/**
* The different error types encountered for gl shaders, in an enum for convenience.
*/
enum class ErrorType {
	COMPILATION_ERROR,
};

/**
* Handles all kinds of errors that may arise from shader loading.
*/
class ShaderErrorHandler
{
public:
	/**
	* Tries to handle the error type encountered for shader.
	* @param error type encountered
	* @param shader.
	* @return true, if error has been handled successfully.
	* @return false, if error has not been handled. Still prints.
	*/
	bool TryHandleError(GLuint& shader, ErrorType type);
	

private:
	bool handleCompilationError(GLuint& shader);
};



