#include <string>
#include <iostream>

#include "ShaderErrorHandler.h"
#include "MessagePrinter.h"

bool ShaderErrorHandler::TryHandleError(GLuint& shader, ErrorType type)
{
	bool success = false;
	switch (type) {
	case ErrorType::COMPILATION_ERROR:
	{
		success = handleCompilationError(shader);
		break;
	}
	}	
	return success;
}


bool ShaderErrorHandler::handleCompilationError(GLuint& shader) {
	GLint maxLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength); // query the log, this just tells how many bytes to allocate

	// The maxLength includes the NULL character
	std::vector<GLchar> infoLog(maxLength); // allocate the bytes
	glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]); // get the log

	MessagePrinter::PrintLog(infoLog);
	
	return false;
}