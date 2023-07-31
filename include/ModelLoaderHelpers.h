#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace _internal_helpers
{
	glm::vec3 to_vec3(aiVector3D const& vec);
	glm::mat4 to_mat4(aiMatrix4x4& mat);
	glm::quat to_quat(aiQuaternion&);
	aiQuaternion to_aiQuat(glm::quat& quat);
}
