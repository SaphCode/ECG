#pragma once

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "ShaderErrorHandler.h"

#include <string>
#include <vector>


/* Handles shader related activities: loading, checking for errors, deleting, attaching to program. */
class ShaderHandler
{
public:
	/* Default Constructor */
	ShaderHandler() {}

	/**
	* Tries to load all shaders.
	* @param
	* shaders to load
	* @return
	* false, has deleted all the shaders (no memory leak)
	* true, has loaded all shaders successfully. can now link program
	*/
	bool TryLoadShaders(std::vector<Shader> shaders);

	/* Attaches all loaded shaders of @param program.
	*/
	void AttachToProgram();

	/* Detaches all loaded shaders of @param program.
	*/
	void DetachFromProgram();

	/* Deletes all shaders registered in member variable m_loadedShaders.
	*/
	void DeleteAllShaders();

	GLuint& getProgram() {
		return m_program;
	}

	void setProgram(GLuint program) {
		m_program = program;
	}
	
private:
	std::string m_shader_dir;
	std::vector<GLuint> m_loadedShaders;
	ShaderErrorHandler m_seh;

	GLuint m_program;

	void compileShader(GLuint shader);
		
};

