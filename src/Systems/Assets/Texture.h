#pragma once

#include <GL/gl3w.h>
#include <string>

class Texture {
	public:
		GLuint id;
		Texture(const std::string& path);
		void use(GLenum textureSlot);
};