#pragma once

#include <GL/gl3w.h>
#include <string>

class LinearTexture {
	public:
		GLuint id;
		LinearTexture(const std::string& path);
};