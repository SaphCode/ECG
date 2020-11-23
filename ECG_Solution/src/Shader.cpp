#include "Shader.h"

#include <fstream>
#include <sstream>

Shader::Shader(ShaderType type, std::string pathToSource) :
	m_type(type)
{
	m_source = readSourceFromFile(pathToSource);
}

std::string Shader::readSourceFromFile(std::string path)
{
	std::ifstream file(path);
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

const std::string Shader::SHADER_DIR = "../assets/";
