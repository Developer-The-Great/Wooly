#pragma once

#include <string>
#include <glad/glad.h>
#include "Debug.h"

#include "stb_image.h"

namespace PXG
{
	struct Texture
	{
		unsigned int id;
		std::string type;
		std::string path;

		static int GenerateTextureID(std::string path,std::string directory)
		{
			//get filename
			std::string filename = std::string(path);

			filename = directory + '/' + filename;

			Debug::Log(Verbosity::Info, "TextureID Filename {0} ", filename);

			unsigned int TextureID;

			glGenTextures(1, &TextureID);

			int width, height, nrComponents;
			//get image data
			unsigned char * data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

			if (data)
			{
				GLenum format;

				if (nrComponents == 1)
				{
					format = GL_RED;
				}
				else if (nrComponents == 3)
				{
					format = GL_RGB;
				}
				else if (nrComponents == 4)
				{
					format = GL_RGBA;
				}

				glBindTexture(GL_TEXTURE_2D, TextureID);

				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(data);

			}
			else
			{
				std::cerr << "ERROR::Model::Cannot generate image data" << std::endl;
				stbi_image_free(data);
			}

			return TextureID;
		}
	};
}