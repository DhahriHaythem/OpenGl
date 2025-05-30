#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include <iostream>

Texture2D::Texture2D()
	:mTexture(0)
{
}

Texture2D::~Texture2D()
{
}

bool Texture2D::loadTexture(const string& filename, bool generateMipMaps)
{
	int width, height, components;

	unsigned char* imageData = stbi_load(filename.c_str(),&width,&height,&components,STBI_rgb_alpha);

	if (imageData == NULL)
	{
		std::cerr << "Error loading texture '" << filename << "'" << std::endl;
		return false;
	}

	//invert image
	int widthInBytes = width * 4;
	unsigned char* top = NULL;
	unsigned char* bottom = NULL;
	unsigned char temp = 0;
	int halfHeight = height / 2;
	for (int row = 0; row < halfHeight; row++)
	{
		top = imageData + row * widthInBytes;
		bottom = imageData + (height - row-1) * widthInBytes;
		for (int col = 0; col < widthInBytes; col++)
		{
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	//Texture wraping mode S=U T=V
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Texture filtring/sampling
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	if (generateMipMaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(imageData);

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void Texture2D::bind(GLuint texUnit)
{
	glBindTexture(GL_TEXTURE_2D, mTexture);
}
