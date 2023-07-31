#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader 
{
public:
	virtual ~Shader() = default;
	virtual void Compile() = 0;
	virtual void Use()const = 0;
	virtual void SetFloat(std::string const& name,float)const = 0;
	virtual void SetInt(std::string const& name, int)const = 0;
	virtual void SetMat4(std::string const& name, glm::mat4 const&)const = 0;
	virtual void SetMat3(std::string const& name, glm::mat3 const&)const = 0;
	virtual void SetVec4(std::string const& name, glm::vec4 const&)const = 0;
	virtual void SetVec3(std::string const& name, glm::vec3 const&)const = 0;
	virtual void SetVec2(std::string const& name, glm::vec2 const&)const = 0;
};
//vsf - vertex shader filename
//fsf - fragment shader filename
Shader* make_shader(const char* vsf, const char* fsf);
