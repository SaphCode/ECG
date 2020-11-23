#pragma once

#include <string>

enum class ShaderType {
	FRAGMENT,
	VERTEX
};

/**
* Represents a shader with a type and a source code.
*/
class Shader
{
public:
	/**
	* Creates a Shader of type, reading the source code from the path given.
	* @param type: the type of the shader, eg Vertex, Fragment, ..
	* @param pathToSource: relative path to source code of shader
	*/
	Shader(ShaderType type, std::string pathToSource);

	ShaderType getType() const {
		 return m_type;
	}
	std::string getSource() const {
		 return m_source;
	}

	static const std::string SHADER_DIR;

private:
	const ShaderType m_type;
	std::string m_source;
	
	std::string readSourceFromFile(std::string path);

};

