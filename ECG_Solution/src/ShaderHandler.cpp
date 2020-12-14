#include <iostream>

#include "ShaderHandler.h"


bool ShaderHandler::TryLoadShaders(std::vector<Shader> shaders) {
	// Create an empty vertex shader handle
	for (const auto& shader : shaders) {
		GLuint glShader;
		switch (shader.getType()) {
		case ShaderType::FRAGMENT:
		{
			glShader = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		}
		case ShaderType::VERTEX:
		{
			glShader = glCreateShader(GL_VERTEX_SHADER);
			break;
		}
		default:
		{
			std::cout << "Not implemented this type yet." << std::endl;
			return false;
			break;
		}
		}

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		std::string shaderSource(shader.getSource());
		const GLchar* source = (const GLchar*)shaderSource.c_str();
		glShaderSource(glShader, 1, &source, 0);

		// Compile the shader
		compileShader(glShader); // see above 

		// need to check for error since shader compilation failure is not an OpenGL Error.
		GLint isCompiled = 0;
		glGetShaderiv(glShader, GL_COMPILE_STATUS, &isCompiled); // check for error. if isCompiled is GL_FALSE, most recent compilation as failed. otherwise, succeeded.
		
		if (isCompiled == GL_FALSE) {
			bool handledSuccessfully = m_seh.TryHandleError(glShader, ErrorType::COMPILATION_ERROR);
			if (!handledSuccessfully) {
				DeleteAllShaders(); // never leak memory
				return false;
			}
		}
	}
	return true;
	
}

void ShaderHandler::AttachToProgram()
{
	for (auto& shader : m_loadedShaders) {
		glAttachShader(m_program, shader);
	}
}

void ShaderHandler::DetachFromProgram()
{
	for (auto& shader : m_loadedShaders) {
		glDetachShader(m_program, shader);
	}
}


void ShaderHandler::DeleteAllShaders() {
	// Delete all shaders
	for (auto& shader : m_loadedShaders) {
		glDeleteShader(shader);
	}
	std::cout << "Deleted all shaders." << std::endl;
}

/*
* In addition to calling glCompileShader, this registers the shader to the loaded shaders.
*/
void ShaderHandler::compileShader(GLuint shader) {
	glCompileShader(shader);
	m_loadedShaders.push_back(shader);
}
