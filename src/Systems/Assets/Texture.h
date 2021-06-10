#pragma once

#include <GL/gl3w.h>
#include <string>

class Texture {
	public:
		std::string path;
		
		GLuint id;
		Texture(const std::string& path);
		void use(GLenum textureSlot);
};