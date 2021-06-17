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
	
	static void TargetTransform(int target);
	static void UpdateProjection(const glm::mat4& projection);
	static void UpdateTransform(const glm::mat4& projection,int id);

	static void Shutdown();

	static void BeginBatch();
	static void EndBatch();
	static void Flush();

	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureId);
	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureId, const glm::vec4& color);

	static void QuadBox(const glm::vec2& position, const glm::vec2& size, const float thickness, const glm::vec4& color);
	static void BlackBox(const glm::vec2& position, const glm::vec2& size, const float thickness);

	static void DrawStr(const glm::vec2& position,float scale, const std::string& str, Font* font);
	static void DrawStrC(const glm::vec2& position,float scale, const std::string& str , Font* font);

	static void DrawNeedle(const glm::vec2& origin, float radius, float angle);
};
