#pragma once
#include "Vector3.h"
#include <vector>
#include "Texture.h"

namespace PXG
{
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

		void Draw();

		Mesh(std::vector<unsigned int> indices, std::vector<Vertex> vertices, std::vector<Texture> textures);


	private:

		unsigned int VAO, VBO, EBO;

		static std::string textureDiffuseStr;
		static std::string textureSpecularStr;


		void setupMesh();

	};

}


