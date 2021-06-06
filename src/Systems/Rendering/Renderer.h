#pragma once

#include <glm/fwd.hpp>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Assets/Font.h"


class Renderer {
public:
	static void Init(const glm::vec2& resolution);
	static void SetResolution(const glm::vec2& resoltion);
	static void SetView(const glm::vec2& position, float scale, int id);
	static void TargetView(int target);

	static void Shutdown();

	static void BeginBatch();
	static void EndBatch();
	static void Flush();

	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureId);
	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureId, const glm::vec4& color);

	static void QuadBox(const glm::vec2& position, const glm::vec2& size, const float thickness, const glm::vec4& color);
	static void BlackBox(const glm::vec2& position, const glm::vec2& size, const float thickness);


	static void DrawStr(const glm::vec2& position,float scale, std::string str, Font* font);
};