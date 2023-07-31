#pragma once
#include "main/Model.h"
#include <ModelLoaderHelpers.h>
#include <map>
#include <vector>
#include <Mesh.h>
struct BoneInfo 
{
	int id;

	glm::mat4 offset;
};

struct BoneNode
{
	template<typename t>
	using vec = std::vector<t>;
	std::string name;
	glm::mat4 transformation;
	vec<BoneNode> children;
};

//In future i will also implement static model for models without animations
class AnimatedModel : public Model
{
private:
	int boneCounter;
	void ExtractBoneInfo(std::vector<ModelGraphicsPrimitives::Vertex>&,aiMesh*const&);
	std::map<std::string, BoneInfo> boneInfoMap;
	std::vector<texture> textures_loaded;
	std::vector<texture> loadMatTextures(aiMaterial*& mat, aiTextureType&& type, std::string typeName);
	std::vector<ModelGraphicsPrimitives::Mesh> meshes;
	std::string filename,directory;
	void ProcessNode(aiNode*const& node, const aiScene*& scene);
	ModelGraphicsPrimitives::Mesh ProcessMesh(aiMesh* const& mesh, const aiScene*& scene);
public:
	~AnimatedModel();
	AnimatedModel(std::string const& filename);
	std::string GetFileName()const override { return filename; }
	void Render(Shader&)override;
	friend class SkeletalAnimation;
};