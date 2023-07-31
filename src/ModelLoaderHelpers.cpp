#include  <ModelLoaderHelpers.h>
#include <glad/glad.h>
namespace _internal_helpers
{
	glm::vec3 to_vec3(aiVector3D const& vec)
	{
		return glm::vec3(vec.x, vec.y, vec.z);
	}
	glm::mat4 to_mat4(aiMatrix4x4& mat)
	{
		glm::mat4 m;
		m[0][0] = mat.a1;
		m[0][1] = mat.b1;
		m[0][2] = mat.c1;
		m[0][3] = mat.d1;

		m[1][0] = mat.a2;
		m[1][1] = mat.b2;
		m[1][2] = mat.c2;
		m[1][3] = mat.d2;

		m[2][0] = mat.a3;
		m[2][1] = mat.b3;
		m[2][2] = mat.c3;
		m[2][3] = mat.d3;

		m[3][0] = mat.a4;
		m[3][1] = mat.b4;
		m[3][2] = mat.c4;
		m[3][3] = mat.d4;
		return m;
	}
	glm::quat to_quat(aiQuaternion& quat)
	{
		return glm::quat(quat.w,quat.x,quat.y,quat.z);
	}
	aiQuaternion to_aiQuat(glm::quat& quat)
	{
		return aiQuaternion(quat.w,quat.x,quat.y,quat.z);
	}
}
