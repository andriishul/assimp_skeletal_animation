#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader/Shader.hpp>
#include <texture.hpp>
namespace ModelGraphicsPrimitives {
	struct Vertex
	{
		glm::vec3 aPos, aNormal, aTangent;
		glm::vec2 aTexCoords;
		int aBoneIDs[4] = { -1,-1,-1,-1 };
		float aWeights[4] = { 0,0,0,0 };
		void operator()(int bone_id, float weight) 
		{
			for (int i = 0; i < 4; i++)
			{
				if (aBoneIDs[i] < 0)
				{
					*(aBoneIDs + i) = bone_id;
					*(aWeights + i) = weight;
					break;
				}
			}
		}
	};


	class Mesh
	{
	private:
		std::vector<texture> textures;
		unsigned vao, vbo, ebo;
		int indices_num;
	public:
		Mesh(std::vector<Vertex>& vertices, std::vector<texture>& textures, std::vector<unsigned>& indices);
		void Render(Shader&);
		void Release()const;
	};
}