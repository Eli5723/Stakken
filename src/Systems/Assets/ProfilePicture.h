#pragma once

#include <GL/gl3w.h>
#include <string>

class ProfilePicture {
	public:
		GLuint id;
		ProfilePicture(const std::string& path);
};