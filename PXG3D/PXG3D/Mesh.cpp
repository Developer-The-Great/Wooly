#include "Mesh.h"
#include <glad/glad.h>


namespace PXG
{
	std::string Mesh::textureDiffuseStr = "texture_diffuse";
	std::string Mesh::textureSpecularStr = "texture_specular";

	void Mesh::Draw()
	{
		//assume shader.Use() was already called before calling mesh.Draw()


		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	Mesh::Mesh(std::vector<unsigned int> indices, std::vector<Vertex> vertices, std::vector<Texture> textures)
	{
		this->Textures = textures;
		this->Indices = indices;
		this->Vertices = vertices;
		setupMesh();
	}
	void Mesh::setupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);


		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,UV));

		glBindVertexArray(0);

	}
}

