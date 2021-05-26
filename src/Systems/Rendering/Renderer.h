#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// #include "Systems/Assets/Font.h"

class Renderer {
public:
	static void Init(glm::mat4& viewProjection,glm::mat4& viewTransform);
	static void Shutdown();

	static void BeginBatch();
	static void EndBatch();
	static void Flush();

	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureId);
	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureId, const glm::vec4& color);

	static void QuadBox(const glm::vec2& position, const glm::vec2& size, const float& thickness, const glm::vec4& color);
	static void BlackBox(const glm::vec2& position, const glm::vec2& size, const float& thickness);


	// static void DrawStr(const glm::vec2& position,float scale, std::string str, Font* font);
};