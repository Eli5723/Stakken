#pragma once

#include <GL/gl3w.h>
#include <string>
#include <glm/glm.hpp>

class Texture {
	public:
		std::string path;
		glm::vec2 size;
		
		GLuint id;
		Texture(const char* path);
		void use(GLenum textureSlot);
};