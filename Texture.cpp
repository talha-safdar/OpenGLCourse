#include "Texture.h"

Texture::Texture()
{
				textureID = 0;
				width = 0;
				height = 0;
				bitDepth = 0;
				fileLocation = NULL;
}

Texture::Texture(char* fileLoc)
{
				textureID = 0;
				width = 0;
				height = 0;
				bitDepth = 0;
				fileLocation = fileLoc;
}

void Texture::LoadTexture()
{
				unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
				if (!texData)
				{
								printf("Failed to find: %s\n", fileLocation);
								return;
				}

				glGenTextures(1, &textureID);
				glBindTexture(GL_TEXTURE_2D, textureID);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // on x-axis if over 1 repeat you can play around with the last aregument trying other ones
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // on y-axis if over 1 repeat
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // when zooming in (gl linear= blend together, gl nearest= pixelated)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // closer to image play with last argument to try other ones (see slides)

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
				glGenerateMipmap(GL_TEXTURE_2D);

				glBindTexture(GL_TEXTURE_2D, 0);

				stbi_image_free(texData);
}

void Texture::UseTexture()
{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureID);

}

void Texture::ClearTexture()
{
				glDeleteTextures(1, &textureID);
				textureID = 0;
				width = 0;
				height = 0;
				bitDepth = 0;
				fileLocation = NULL;
}

Texture::~Texture()
{
				ClearTexture(); // to make sure to free the memory
}