#pragma once
#include <shader/Shader.hpp>

///TODO: add geometry shader
class glShader : public Shader 
{
private:
	std::string vertexShader_code,fragmentShader_code;
	unsigned ProgramID;
	std::string ReadShader(const std::string& filename)const;
	void check_compilation_status(unsigned shader,const char* errorName)const;
	void check_program_link_status()const;
public:
	~glShader();
	glShader(const std::string& vertex_shader_file, const std::string& fragment_shader_file);
	void Use()const override;
	void Compile() override;
	void SetFloat(std::string const& name, float)const override;
	void SetInt(std::string const& name, int)const override;
	void SetMat4(std::string const& name, glm::mat4 const&)const override;
	void SetMat3(std::string const& name, glm::mat3 const&)const override;
	void SetVec4(std::string const& name, glm::vec4 const&)const override;
	void SetVec3(std::string const& name, glm::vec3 const&)const override;
	void SetVec2(std::string const& name, glm::vec2 const&)const override;
};