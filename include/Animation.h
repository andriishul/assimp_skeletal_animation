#pragma once
#include <vector>
#include <string>
class Animation 
{
public:
	virtual ~Animation() = default;
	virtual void Update(float deltaTime) = 0;
	virtual std::string GetName() const = 0;

};

std::vector<Animation*> GetModelAnimations(class Model*&);