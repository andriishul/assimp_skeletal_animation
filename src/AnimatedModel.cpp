#include <AnimatedModel.hpp>
#include <ModelLoaderHelpers.h>
void AnimatedModel::ExtractBoneInfo(std::vector<ModelGraphicsPrimitives::Vertex>& vertices, aiMesh*const& mesh)
{
	int boneID = -1;
	for (unsigned i = 0; i < mesh->mNumBones; i++)
	{
		aiBone* bone = *(mesh->mBones + i);
		//bone name
		std::string name = bone->mName.C_Str();
		if (boneInfoMap.find(name) == boneInfoMap.end())
		{
			BoneInfo boneInfo;
			boneInfo.id = boneCounter++;
			boneInfo.offset = _internal_helpers::to_mat4(bone->mOffsetMatrix);

			boneInfoMap[name] = boneInfo;
			boneID = boneInfo.id;
		}
		else 
			boneID = boneInfoMap[name].id;

		for (unsigned j = 0; j < bone->mNumWeights; j++)
		{
			aiVertexWeight weight = bone->mWeights[j];
			//setting boneIDs and weights
			vertices[weight.mVertexId](boneID,weight.mWeight);
			assert(weight.mVertexId <= vertices.size());
		}
	}
}

std::vector<texture> AnimatedModel::loadMatTextures(aiMaterial*& mat, aiTextureType&& type, std::string typeName)
{
		std::vector<texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString name;
			mat->GetTexture(type, i, &name);
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.data(), name.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				texture texture;
				std::string filepath = directory + "/" + name.C_Str();
				texture = load_texture2D(filepath);
				texture.type = typeName;
				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}
		}
		return textures;
}

void AnimatedModel::ProcessNode(aiNode* const& node, const aiScene*& scene)
{
	for (unsigned i = 0; i < node->mNumMeshes; i++)
	{
		meshes.push_back(ProcessMesh(scene->mMeshes[*(node->mMeshes+i)],scene));
	}

	for (unsigned i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(*(node->mChildren+i),scene);
	}
}

ModelGraphicsPrimitives::Mesh AnimatedModel::ProcessMesh(aiMesh* const& mesh, const aiScene*& scene)
{	
	std::vector <texture> textures;
	std::vector <unsigned>indices;
	std::vector<ModelGraphicsPrimitives::Vertex> vertices;
	for (unsigned i = 0; i < mesh->mNumVertices; i++)
	{
		ModelGraphicsPrimitives::Vertex vertex;
		vertex.aPos =_internal_helpers::to_vec3(mesh->mVertices[i]);
		if (mesh->HasNormals())
			vertex.aNormal = _internal_helpers::to_vec3(mesh->mNormals[i]);
		if (mesh->HasTangentsAndBitangents())
			vertex.aTangent = _internal_helpers::to_vec3(mesh->mTangents[i]);
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.aTexCoords = vec;
		}
		vertices.push_back(vertex);
	}
	for (unsigned i = 0; i < mesh->mNumFaces; i++)
	{
		auto face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	if (mesh->mMaterialIndex >= 0)
	{
		auto material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<texture> diffuseMaps = loadMatTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<texture> specularMaps = loadMatTextures(material,
			aiTextureType_SPECULAR, "texture_specular");

		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		std::vector<texture> normalMaps = loadMatTextures(material,
			aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}
	ExtractBoneInfo(vertices,mesh);
	return ModelGraphicsPrimitives::Mesh(vertices,textures,indices);
}

AnimatedModel::~AnimatedModel()
{
	for (auto mesh : meshes)
		mesh.Release();
}

AnimatedModel::AnimatedModel(std::string const& filename):
	filename(filename),boneCounter(0)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename,aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_JoinIdenticalVertices);

	if (!scene->mRootNode || !scene)
	{
		fprintf(stderr,"ASSIMP_ERROR : %s\n",importer.GetErrorString());
		abort();
	}

	if (std::string{ filename }.find("/") != std::string::npos)
		directory = std::string{ filename }.substr(0, std::string{ filename }.find_last_of('/'));
	if (std::string{ filename }.find("\\") != std::string::npos)
		directory = std::string{ filename }.substr(0, std::string{ filename }.find_last_of("\\"));
	
	ProcessNode(scene->mRootNode,scene);
	textures_loaded.clear();
}

void AnimatedModel::Render(Shader& s)
{
	for (auto mesh : meshes)
		mesh.Render(s);
}
