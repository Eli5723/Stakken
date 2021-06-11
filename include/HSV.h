#pragma once
#include <algorithm>
#include <glm/glm.hpp>

void HSV2RGB(float  h, float s, float v, float& r, float& g, float& b) {
	h = fmod(fabs(h),360.0f);
	float hp = h / 60.0f;
	const float C = v * s;
	const float M = v - C;

	int color = hp;

	float X = C * (1 - fabs((fmod(hp, 2.0f) - 1.0f)));

	switch (color) {
		case 0: r = M+C; g = M+X; b = M+0; break;
		case 1: r = M+X; g = M+C; b = M+0; break;
		case 2: r = M+0; g = M+C; b = M+X; break;
		case 3: r = M+0; g = M+X; b = M+C; break;
		case 4: r = M+X; g = M+0; b = M+C; break;
		case 5: r = M+C; g = M+0; b = M+X; break;
	};
}

inline void HSV2RGB(float h, float s, float v, glm::vec3 &vector) {
	HSV2RGB(h,s,v, vector.x, vector.y, vector.z);
}

inline void HSV2RGB(float h, float s, float v, glm::vec4& vector) {
	HSV2RGB(h,s,v, vector.x, vector.y, vector.z);
}


inline void RGB2HSV(float& h, float& s, float& v, const glm::vec4& color){
	float cmax = std::max(std::max(color.r,color.g),color.b);
	float cmin = std::min(std::min(color.r,color.g),color.b);
	float dif = cmax - cmin;

	// Hue
	if (cmax == color.r){
		h = 60.0f * std::fmod(((color.g-color.b)/dif),6.0f);
	} else if (cmax == color.g){
		h = 60.0f * (((color.b - color.r)/dif)+2.0f);
	} else { // cmax == color.b
		h = 60.0f * (((color.r - color.g)/dif)+4.0f);
	}

	// Saturation;
	if (cmax == 0){
		s = 0;
	} else {
		s = dif / cmax;
	}

	// Value
	v = cmax;
}