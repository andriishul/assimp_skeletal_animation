#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
class Animation 
{
public:
	virtual ~Animation() = default;
	virtual void Update(float deltaTime) = 0;
	virtual std::string GetName() const = 0;
	virtual void PlayFrame(class Shader&) = 0;
};

std::vector<Animation*> GetModelAnimations(class Model*&);