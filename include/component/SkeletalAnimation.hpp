#pragma once
#include <AnimatedModel.hpp>
#include <main/Animation.h>

namespace SkeletalSystem {

	template<typename t>
	struct Frame
	{
		float time;
		t value;
	};

	struct PositionFrame : Frame<glm::vec3>
	{
		PositionFrame(float time, aiVector3D const& vec)
		{
			this->time = time;
			value = _internal_helpers::to_vec3(vec);
		}
	};

	struct ScaleFrame : Frame<glm::vec3>
	{
		ScaleFrame(float time, aiVector3D const& vec)
		{
			this->time = time;

			value = _internal_helpers::to_vec3(vec);
		}
	};

	struct RotationFrame : Frame<glm::quat>
	{
		RotationFrame(float time, aiQuaternion const& vec)
		{
			this->time = time;

			value = _internal_helpers::to_quat((aiQuaternion&)vec);
		}
	};

	class Bone 
	{
	private:
		glm::mat4 local_transformation;
		int id;
		std::string name;
		std::vector<PositionFrame> positionFrames;
		std::vector<ScaleFrame> scaleFrames;
		std::vector<RotationFrame> rotationFrames;
	public:
		Bone() = delete;
		Bone(std::string const& name,int id,aiNodeAnim*&);
		float GetScaleFactor(float animTime,float lastTimestamp,float nextTimestamp)const 
		{
			return (animTime - lastTimestamp) /
				(nextTimestamp - lastTimestamp);
		}
		int GetPositionIndex(float animTime)const;
		int GetScaleIndex(float animTime)const;
		int GetRotationIndex(float animTime)const;

		inline glm::mat4 InterpolatePosition(float animTime);
		inline glm::mat4 InterpolateRotation(float animTime);
		inline glm::mat4 InterpolateScale(float animTime);
		void Update(float animTime);
		auto GetName()const { return name; }
		int GetID()const { return id; }
		auto GetLocalTransformation()const { return local_transformation; }
	};
}

class SkeletalAnimation : public Animation
{
private:
	std::vector<glm::mat4> finalBonesMatrices;
	float Duration,CurrentTime,TicksPerSecond;
	std::string name;
	std::vector<SkeletalSystem::Bone> bones;
	BoneNode RootNode;
	std::map<std::string, BoneInfo>& boneInfoMap;
	SkeletalSystem::Bone* FindBone(std::string const& name)const;
	void CalculateBoneTransform(BoneNode&,glm::mat4 const&);
	void ReadMissingBones(aiAnimation*);
	void ReadSkeletonHierarchy(BoneNode&,aiNode*const&);
public:
	SkeletalAnimation(int,Model*&); 
	void Update(float deltaTime) override;
	std::string GetName()const override { return name; }
	void PlayFrame(Shader&) override;
};
