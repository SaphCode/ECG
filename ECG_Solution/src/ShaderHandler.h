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
	void AttachToProgram(GLuint& program);

	/* Detaches all loaded shaders of @param program.
	*/
	void DetachFromProgram(GLuint& program);

	/* Deletes all shaders registered in member variable m_loadedShaders.
	*/
	void DeleteAllShaders();
	
private:
	std::string m_shader_dir;
	std::vector<std::unique_ptr<GLuint>> m_loadedShaders;
	ShaderErrorHandler m_seh;

	void compileShader(GLuint shader);
		
};

