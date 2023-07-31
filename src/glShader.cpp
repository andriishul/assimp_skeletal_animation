#include <shader/gl/glShader.h>
#include <iostream>
#include <fstream>
#include <format>
#include <glad/glad.h>
std::string glShader::ReadShader(const std::string& filename) const
{
	std::string all_text;
	std::string line;
	
	std::ifstream fs(filename);
	while (std::getline(fs,line)) 
	{
		all_text += line + "\r\n";
	}
	return all_text;
}

void glShader::check_compilation_status(unsigned shader, const char* errorName) const
{
	GLint success = 1;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLsizei length = 0;
		char* error = new char[1000];
		glGetShaderInfoLog(shader, 1000, &length, error);
		delete[] error;
		error = new char[length];
		glGetShaderInfoLog(shader, length, &length, error);
		fprintf(stderr, "%s\n%s\n", errorName,error);
		delete[] error;
		abort();
	}
}

void glShader::check_program_link_status() const
{
	GLint success ;
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLsizei length = 0;
		char* error = new char[1000];
		glGetProgramInfoLog(ProgramID, 1000, &length, error);
		delete[] error;
		error = new char[length];
		glGetProgramInfoLog(ProgramID, length, &length, error);
		fprintf(stderr, "PROGRAM_LINK_STATUS_ERROR\n%s\n", error);
		delete[] error;
		abort();
	}
}

glShader::~glShader()
{
	glDeleteProgram(ProgramID);
}

glShader::glShader(const std::string& vertex_shader_file, const std::string& fragment_shader_file):
	fragmentShader_code(ReadShader(fragment_shader_file)),vertexShader_code(ReadShader(vertex_shader_file)),
	ProgramID(-1)
{
	assert(!fragmentShader_code.empty() && "Fragment Shader is empty");
	assert(!vertexShader_code.empty() && "Vertex Shader is empty");
}

void glShader::Use() const
{
	if (ProgramID == -1)
		assert(ProgramID != -1 && "Maybe shader is not compiled");
	glUseProgram(ProgramID);
}

void glShader::Compile()
{
	ProgramID = glCreateProgram();
	
	unsigned vertexObject = glCreateShader(GL_VERTEX_SHADER);
	unsigned fragmentObject = glCreateShader(GL_FRAGMENT_SHADER);
	auto vert_code = vertexShader_code.c_str();
	auto frag_code = fragmentShader_code.c_str();
	
	glShaderSource(vertexObject,1,&vert_code,0);
	glShaderSource(fragmentObject, 1, &frag_code, 0);

	glCompileShader(vertexObject);
	check_compilation_status(vertexObject,"VERTEX_SHADER_ERROR");
	glCompileShader(fragmentObject);
	check_compilation_status(fragmentObject, "FRAGMENT_SHADER_ERROR");

	glAttachShader(ProgramID,vertexObject);
	glAttachShader(ProgramID,fragmentObject);
	glLinkProgram(ProgramID);
	check_program_link_status();
	glDeleteShader(vertexObject);
	glDeleteShader(fragmentObject);
	assert(glGetError() == GL_NO_ERROR);
}

void glShader::SetFloat(std::string const& name, float val) const
{
	GLint location = glGetUniformLocation(ProgramID,name.c_str());
	if (location == -1) { fprintf(stderr, std::format("uniform float {0} not found(or maybe not used in code)", name).c_str()); abort(); }
	glUniform1f(location,val);
}

void glShader::SetInt(std::string const& name, int val) const
{
	GLint location = glGetUniformLocation(ProgramID, name.c_str());
	if (location == -1) {
		fprintf(stderr, std::format("uniform int {0} not found(or maybe not used in code)", name).c_str());
		abort();
	}
	glUniform1i(location, val);
}

void glShader::SetMat4(std::string const& name, glm::mat4 const& val) const
{
	GLint location = glGetUniformLocation(ProgramID, name.c_str());
	if (location == -1) 
	{
		fprintf(stderr, std::format("uniform mat4 {0} not found(or maybe not used in code)", name).c_str());
		abort();
	} 
	glUniformMatrix4fv(location,1,GL_FALSE,&val[0][0]);
}

void glShader::SetMat3(std::string const& name, glm::mat3 const& val) const
{
	GLint location = glGetUniformLocation(ProgramID, name.c_str());
	if (location == -1)
	{
		fprintf(stderr, std::format("uniform mat3 {0} not found(or maybe not used in code)", name).c_str());
		abort();
	}
	glUniformMatrix3fv(location, 1, GL_FALSE, &val[0][0]);
}

void glShader::SetVec4(std::string const& name, glm::vec4 const& val) const
{
	GLint location = glGetUniformLocation(ProgramID, name.c_str());
	if (location == -1)
	{
		fprintf(stderr, std::format("uniform vec4 {0} not found(or maybe not used in code)", name).c_str());
		abort();
	}

	glUniform4f(location,val.x,val.y,val.z,val.w);
}

void glShader::SetVec3(std::string const& name, glm::vec3 const& val) const
{
	GLint location = glGetUniformLocation(ProgramID, name.c_str());
	if (location == -1)
	{
		fprintf(stderr, std::format("uniform vec3 {0} not found(or maybe not used in code)", name).c_str());
		abort();
	}
	glUniform3f(location, val.x, val.y, val.z);
}

void glShader::SetVec2(std::string const& name, glm::vec2 const& val) const
{
	GLint location = glGetUniformLocation(ProgramID, name.c_str());
	if (location == -1)
	{
		fprintf(stderr, std::format("uniform vec2 {0} not found(or maybe not used in code)", name).c_str());
		abort();
	}	
	glUniform2f(location, val.x, val.y);
}

