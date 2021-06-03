#pragma once

#include <glm/glm.hpp>
#include <string>
#include "Assets/LinearTexture.h"

#define rgbToFloat(r,g,b) {(float)r/255.0f,(float)g/255.0f,(float)b/255.0f,1}

struct ColorTable {
	glm::vec4 entries[8];
};

struct Identity {
	ColorTable color_table;
	std::string name;
	LinearTexture* pfp;
};

const Identity defaultIdentity {{{
	rgbToFloat(237,41,57), //T
	rgbToFloat(255,121,0), //O
	rgbToFloat(254,203,0), //L
	rgbToFloat(105,190,40), //J
	rgbToFloat(0,159,218), //Z
	rgbToFloat(0,101,189), //S
	rgbToFloat(149,45,152),  //I
	rgbToFloat(40,40,40)}},
	"Guest",
	0
};