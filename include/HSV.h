#pragma once
#include <algorithm>
#include <glm/glm.hpp>

void H2RGB(float  h, float& r, float& g, float& b) {
	h = fmod(h,360.0f);
	float hp = h / 60.0f;
	const float C = 1;

	int color = hp;

	float X = 1 - fabs((fmod(hp, 2.0f) - 1.0f) );

	switch (color) {
		case 0: r = C; g = X; b = 0; break;
		case 1: r = X; g = C; b = 0; break;
		case 2: r = 0; g = C; b = X; break;
		case 3: r = 0; g = X; b = C; break;
		case 4: r = X; g = 0; b = C; break;
		case 5: r = C; g = 0; b = X; break;
	};
}

inline void H2RGB(float h, glm::vec3 &vector) {
	H2RGB(h, vector.x, vector.y, vector.z);
}

inline void H2RGB(float h, glm::vec4& vector) {
	H2RGB(h, vector.x, vector.y, vector.z);
}


inline float RGB2H(const glm::vec4& color){
	float cmax = std::max(std::max(color.r,color.g),color.b);
	float cmin = std::min(std::min(color.r,color.g),color.b);
	float dif = cmax - cmin;

	float hue;
	if (cmax == color.r){
		hue = 60.0f * std::fmod(((color.g-color.b)/dif),6.0f);
	} else if (cmax == color.g){
		hue = 60.0f * (((color.b - color.r)/dif)+2.0f);
	} else { // cmax == color.b
		hue = 60.0f * (((color.r - color.g)/dif)+4.0f);
	}
	return hue;
}