#pragma once

#include <GL/gl3w.h>
#include <string>

class LinearTexture {
	public:
		std::string path;
		GLuint id;
		LinearTexture(const std::string& path);
};