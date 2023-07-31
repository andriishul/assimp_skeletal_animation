#pragma once
#include <string>
#include <typeinfo>
class Model 
{
public:
	virtual ~Model() = default;
	virtual std::string GetFileName()const = 0;
	std::string GetTypeName()const 
	{
		return std::string(typeid(*this).name()).erase(0,6);
	}
	virtual void Render(class Shader&) = 0;
};

/*Get the count of animations available for the model.A result of - 1 indicates that the model does not support animation.*/
int GetAnimationCount(std::string const&);
Model* LoadModelFromFile(std::string const&);