#include <main/Animation.h>
#include <component/SkeletalAnimation.hpp>

std::vector<Animation*> GetModelAnimations(Model*& model)
{
	std::vector<Animation*> animations;
	int count = GetAnimationCount(model->GetFileName());
	
	
	for (int i = 0; i < count; i++)
	{
		animations.push_back(new SkeletalAnimation(i, model));
	}

	return animations;
}
