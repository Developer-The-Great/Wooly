#pragma once
#include "Vector3.h"
#include <vector>
#include "Texture.h"

namespace PXG
{
	class Shader;

	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
		glm::vec2 UV; //TODO make Vector2 Wrapper
	};

	class Mesh
	{
	public:

		std::vector<unsigned int> Indices;
		std::vector<Vertex> Vertices;
		std::vector<Texture> Textures;

		void Draw(Shader* shader);

		Mesh(std::vector<unsigned int> indices, std::vector<Vertex> vertices, std::vector<Texture> textures);

		 

	private:

		unsigned int VAO, VBO, EBO;


		


		void setupMesh();

	};

}


