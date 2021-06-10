#include "./Texture.h"

#include <GL/gl3w.h>
#include <stb_image.h>

Texture::Texture(const std::string& path) {
	this->path = path;

	//Load Image
	int width, height, nrChannels;
	unsigned char* imageData = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
	if (!imageData)
	{
		printf(("Failed to load texture: %s\n"),path.c_str());
	}

	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imageData);
}

void Texture::use(GLenum textureSlot) {
	glActiveTexture(textureSlot);
	glBindTexture(GL_TEXTURE_2D, id);
}