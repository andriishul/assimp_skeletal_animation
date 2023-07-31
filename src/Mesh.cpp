#include <Mesh.h>
#include <glad/glad.h>
namespace ModelGraphicsPrimitives
{
	Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<texture>& textures, std::vector<unsigned>& indices):
		textures(textures),indices_num(indices.size())
	{
		glGenBuffers(1,&vbo);
		glGenBuffers(1, &ebo);
		glGenVertexArrays(1, &vao);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER,vbo);
		glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),vertices.data(),GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

		//aPos
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),0);
		glEnableVertexAttribArray(0);
		//aNormal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));
		glEnableVertexAttribArray(1);
		//aTangent
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6*sizeof(float)));
		glEnableVertexAttribArray(2);
		//aTexCoords
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(9 * sizeof(float)));
		glEnableVertexAttribArray(3);
		//aBoneIDs
		glVertexAttribIPointer(4, 4, GL_INT, sizeof(Vertex), (void*)(11 * sizeof(float)));
		glEnableVertexAttribArray(4);
		//aWeights
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(15 * sizeof(float)));
		glEnableVertexAttribArray(5);
		assert(glGetError() == GL_NO_ERROR);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		assert(glGetError() == GL_NO_ERROR);
	}
	void Mesh::Render(Shader& s)
	{
		int diffNr = 1,specNr = 1,normalNr = 1;
		std::string name;
		for (int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			if (textures[i].type == "texture_diffuse")
				name = textures[i].type + std::to_string(diffNr++);
			if (textures[i].type == "texture_specular")
				name = textures[i].type + std::to_string(specNr++);
			if (textures[i].type == "texture_normal")
				name = textures[i].type + std::to_string(normalNr++);
			s.SetInt(name,i);
			glBindTexture(GL_TEXTURE_2D,textures[i].ID);
		}

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES,indices_num,GL_UNSIGNED_INT,0);
		glBindVertexArray(0);
	}
	void Mesh::Release() const
	{
		glDeleteBuffers(1,&vbo);
		glDeleteBuffers(1,&ebo);
		glDeleteVertexArrays(1,&vao);

		for (auto texture : textures)
			glDeleteTextures(1,&texture.ID);
	}
}