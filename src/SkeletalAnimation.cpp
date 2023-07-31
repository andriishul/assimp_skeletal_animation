#include <component/SkeletalAnimation.hpp>
#include <thread>

SkeletalSystem::Bone* SkeletalAnimation::FindBone(std::string const& name) const
{
	for (int i = 0; i < bones.size(); i++)
	{
		if (name == bones[i].GetName())
			return (SkeletalSystem::Bone*)&bones[i];
	}
	return nullptr;
}

void SkeletalAnimation::CalculateBoneTransform(BoneNode& node, glm::mat4 const& parent = glm::mat4(1.0f))
{
	std::string name = node.name.c_str();
	glm::mat4 transformation = node.transformation;

	SkeletalSystem::Bone* bone = FindBone(name);
	if (bone)
	{
		bone->Update(CurrentTime);
		transformation = bone->GetLocalTransformation();
	}
	glm::mat4 globalTransformation = parent * transformation;
	if (boneInfoMap.find(name) != boneInfoMap.end())
	{
		finalBonesMatrices[boneInfoMap[name].id] = globalTransformation 
			* boneInfoMap[name].offset;
	}

	for (size_t i = 0; i < node.children.size(); i++)
		CalculateBoneTransform(node.children[i], globalTransformation);
}

void SkeletalAnimation::ReadMissingBones(aiAnimation* animation)
{
	for (int i = 0; i < animation->mNumChannels; i++)
	{
		auto channel = animation->mChannels[i];
		std::string node_name = channel->mNodeName.C_Str();
		if (boneInfoMap.find(node_name) == boneInfoMap.end())
		{
			boneInfoMap[node_name].id = boneInfoMap.size();
		}
		bones.push_back(SkeletalSystem::Bone(node_name,boneInfoMap[node_name].id,channel));
	}
}

void SkeletalAnimation::ReadSkeletonHierarchy(BoneNode& bone_node, aiNode* const& node)
{
	bone_node.name = node->mName.C_Str();
	bone_node.transformation = _internal_helpers::to_mat4(node->mTransformation);

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		BoneNode b_node;
		ReadSkeletonHierarchy(b_node,node->mChildren[i]);
		bone_node.children.push_back(b_node);
	}
}

SkeletalAnimation::SkeletalAnimation(int index,Model*& model):
	boneInfoMap(dynamic_cast<AnimatedModel*>(model)->boneInfoMap),
	CurrentTime(0)
{
	AnimatedModel* animatedModel = dynamic_cast<AnimatedModel*>(model);
	finalBonesMatrices.resize(animatedModel->boneCounter,glm::mat4(1.0f));
	if (model->GetTypeName() != "AnimatedModel" || !animatedModel)
	{
		fprintf(stderr, "Model has no animations\nmodel_type: %s\n", model->GetTypeName());
		abort();
	}
	
	Assimp::Importer importer;
	auto scene = importer.ReadFile(model->GetFileName().c_str(),aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
	auto animation = *(scene->mAnimations + index);
	Duration = animation->mDuration;
	TicksPerSecond = animation->mTicksPerSecond;
	name = animation->mName.C_Str();
	ReadSkeletonHierarchy(RootNode,scene->mRootNode);
	ReadMissingBones(animation);
}

void SkeletalAnimation::Update(float deltaTime)
{
	if (!RootNode.children.empty())
	{
		CurrentTime += TicksPerSecond * deltaTime;
		CurrentTime = fmod(CurrentTime, Duration);
		
		CalculateBoneTransform(RootNode);
	}
}

void SkeletalAnimation::PlayFrame(Shader& shader)
{
	for (int i = 0; i < finalBonesMatrices.size(); i++)
	{
		shader.SetMat4("finalBonesMatrices[" + std::to_string(i) + "]", finalBonesMatrices[i]);
	}
}

SkeletalSystem::Bone::Bone(std::string const& name, int id, aiNodeAnim*& channel)
:name(name),id(id)
{
	for (size_t i = 0; i < channel->mNumPositionKeys; i++)
	{
		positionFrames.push_back({ (float)channel->mPositionKeys[i].mTime,channel->mPositionKeys[i].mValue});
	}
	for (size_t i = 0; i < channel->mNumRotationKeys; i++)
	{
		rotationFrames.push_back({ (float)channel->mRotationKeys[i].mTime,channel->mRotationKeys[i].mValue });
	}
	for (size_t i = 0; i < channel->mNumScalingKeys; i++)
	{
		scaleFrames.push_back({ (float)channel->mScalingKeys[i].mTime,channel->mScalingKeys[i].mValue });
	}
}

int SkeletalSystem::Bone::GetPositionIndex(float animTime)const
{
	for (int i = 1; i < positionFrames.size(); i++)
	{
		if (positionFrames[i + 1].time > animTime)
			return i;
	}
	abort();
}

int SkeletalSystem::Bone::GetScaleIndex(float animTime)const
{
	for (int i = 1; i < scaleFrames.size(); i++)
	{
		if (scaleFrames[i + 1].time > animTime)
			return i;
	}
	abort();
}

int SkeletalSystem::Bone::GetRotationIndex(float animTime)const
{
	for (int i = 1; i < rotationFrames.size(); i++)
	{
		if (rotationFrames[i + 1].time > animTime)
			return i;
	}
	abort();
}

inline glm::mat4 SkeletalSystem::Bone::InterpolatePosition(float animTime) 
{
	if (positionFrames.size() == 1)
		return glm::translate(glm::mat4(1.0),positionFrames[0].value);
	
	int first = GetPositionIndex(animTime);
	int second = first + 1;
	
	float scaleFactor = GetScaleFactor(animTime, positionFrames[second].time, positionFrames[first].time);

	return glm::translate(glm::mat4{1.f}, positionFrames[first].value + scaleFactor * (positionFrames[second].value - positionFrames[first].value));
}

inline glm::mat4 SkeletalSystem::Bone::InterpolateRotation(float animTime)
{
	if (rotationFrames.size() == 1)
		return glm::mat4_cast(rotationFrames[0].value);

	int first = GetRotationIndex(animTime);
	int second = first + 1;

	float scaleFactor = GetScaleFactor(animTime, rotationFrames[second].time, rotationFrames[first].time);

	aiQuaternion out;
	aiQuaternion::Interpolate(out, _internal_helpers::to_aiQuat(rotationFrames[first].value),_internal_helpers::to_aiQuat(rotationFrames[second].value),scaleFactor);

	return glm::mat4_cast(_internal_helpers::to_quat(out));
}

inline glm::mat4 SkeletalSystem::Bone::InterpolateScale(float animTime)
{
	if (scaleFrames.size() == 1)
		return glm::scale(glm::mat4(1.0), scaleFrames[0].value);

	int first = GetScaleIndex(animTime);
	int second = first + 1;

	float scaleFactor = GetScaleFactor(animTime, scaleFrames[second].time, scaleFrames[first].time);

	return glm::scale(glm::mat4{ 1.f }, scaleFrames[first].value + scaleFactor * (scaleFrames[second].value - scaleFrames[first].value));
}

void SkeletalSystem::Bone::Update(float animTime)
{
	auto position = InterpolatePosition(animTime);
	auto rotation = InterpolateRotation(animTime);
	auto scale = InterpolateScale(animTime);

	local_transformation = position * rotation * scale;
}
