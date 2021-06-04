#include "./Renderer.h"

#include <GL/gl3w.h>
#include <array>
#include <iostream>

#include "./Shader.h"
#include "../Assets/Font.h"

static const char* vertexShader = R"(
	#version 330 core
	layout (location = 0) in vec3 a_Pos;
	layout (location = 1) in vec4 a_Color;
	layout (location = 2) in vec2 a_TexCoord;
	layout (location = 3) in float a_TexIndex;
	layout (location = 4) in int a_view;

	uniform mat4 u_projection[4];
	uniform mat4 u_transform[4];

	out vec4 v_Color;
	out vec2 v_TexCoord;
	out float v_TexIndex;

	void main()
	{
		if (a_view == 0)
			gl_Position = u_projection[0] * u_transform[0] * vec4(a_Pos, 1.0);
		if (a_view > 0)
			gl_Position = u_projection[1] * u_transform[1] * vec4(a_Pos, 1.0);
		v_Color = a_Color;
		v_TexCoord = a_TexCoord;
		v_TexIndex = a_TexIndex;
	}
)";

static const char* fragmentShader = R"(
	#version 330 core
	out vec4 FragColor;

	in vec4 v_Color;
	in vec2 v_TexCoord;
	in float v_TexIndex;

	uniform sampler2D u_Textures[32];

	void main()
	{
		int index = int(v_TexIndex);
		if (index==0)
			FragColor = texture(u_Textures[0], v_TexCoord) * v_Color;
		else if (index==1)
			FragColor = texture(u_Textures[1], v_TexCoord) * v_Color;
		else if (index==2)
			FragColor = texture(u_Textures[2], v_TexCoord) * v_Color;
		else if (index==3)
			FragColor = texture(u_Textures[3], v_TexCoord) * v_Color;
	}
)";

static const char* sdfFragmentShader = R"(
	#version 330 core
	out vec4 FragColor;
	
	in vec4 v_Color;
	in vec2 v_TexCoord;
	in float v_TexIndex;

	uniform sampler2D u_Textures[32];

	float median(float r, float g, float b) {
		return max(min(r, g), min(max(r, g), b));
	}

	void main() {
		vec3 msd = texture(u_Textures[1], v_TexCoord).rgb;
		float sd = median(msd.r, msd.g, msd.b);
		float screenPxDistance = 64*(sd - 0.5);
		float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
		FragColor = vec4(1,1,1,opacity);
	}
)";



static const uint32_t MaxQuadCount = 1000;
static const uint32_t MaxVertexCount = MaxQuadCount * 4;
static const uint32_t MaxIndexCount = MaxQuadCount * 6;
static const uint32_t MaxTextures = 32;

struct Vertex {
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 texCoords;
	GLfloat texIndex;
	GLuint view;
};

struct RendererData {
	Shader* quadShader;
	GLint currentView = 0;


	GLuint QuadVA = 0;
	GLuint QuadVB = 0;
	GLuint QuadIB = 0;
	GLuint WhiteTexture = 0;
	uint32_t WhiteTextureSlot = 0;

	uint32_t IndexCount;

	Vertex* QuadBuffer = nullptr;
	Vertex* QuadBufferPtr = nullptr;

	std::array<uint32_t, MaxTextures> TextureSlots;
	uint32_t TextureSlotIndex = 1;
};

static RendererData s_Data;

// Driver Methods
void Renderer::Init(glm::mat4& viewProjection,glm::mat4& viewTransform) {
	s_Data.quadShader = new Shader(fragmentShader,vertexShader);
	s_Data.quadShader->fillSamplers();
	s_Data.currentView = 0;
	SetView(viewProjection,viewTransform, 0);

	s_Data.QuadBuffer = new Vertex[MaxVertexCount];
	
	glGenVertexArrays(1, &s_Data.QuadVA); 
	glBindVertexArray(s_Data.QuadVA); 
	
	glGenBuffers(1, &s_Data.QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MaxVertexCount,nullptr, GL_DYNAMIC_DRAW); 
	
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position)); 
	
	glEnableVertexAttribArray(1); 
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	
	glEnableVertexAttribArray(2); 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	
	glEnableVertexAttribArray(3); 
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texIndex));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, view));
	
	uint32_t indices[MaxIndexCount];

	uint32_t offset = 0;
	for (unsigned int i = 0; i < MaxIndexCount; i += 6) {
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;
		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;

		offset += 4;
	}
	
	glGenBuffers(1, &s_Data.QuadIB); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.QuadIB); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
	

	glGenTextures(1, &s_Data.WhiteTexture); 
	glBindTexture(GL_TEXTURE_2D, s_Data.WhiteTexture); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	uint32_t color = 0xFFFFFFFF;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color); 


	s_Data.TextureSlots[0] = s_Data.WhiteTexture;
	for (unsigned int i = 1; i < MaxTextures; i++) {
		s_Data.TextureSlots[i] = s_Data.WhiteTexture;
	}
}

void Renderer::TargetView(int target){
	s_Data.currentView = target;
}

void Renderer::SetView(glm::mat4& viewProjection,glm::mat4& viewTransform, int offset){
	s_Data.quadShader->use();
	s_Data.quadShader->setMat4Offset("u_projection[0]",viewProjection, offset);
	s_Data.quadShader->setMat4Offset("u_transform[0]",viewTransform, offset);
}

void Renderer::Shutdown() {
	delete[] s_Data.QuadBuffer;
	glDeleteVertexArrays(1,&s_Data.QuadVA); 
	glDeleteBuffers(1,&s_Data.QuadVB); 
	glDeleteBuffers(1,&s_Data.QuadIB); 
	glDeleteTextures(1, &s_Data.WhiteTexture);
	
}
void Renderer::BeginBatch() {
	s_Data.QuadBufferPtr = s_Data.QuadBuffer;
	s_Data.IndexCount = 0;
}

void Renderer::EndBatch() {
	GLsizeiptr size = (char*)s_Data.QuadBufferPtr - (char*)s_Data.QuadBuffer;
	glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.QuadBuffer);
}

void Renderer::Flush() {
	s_Data.quadShader->use();

	for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
		glActiveTexture((GLenum)GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, s_Data.TextureSlots[i]);
	}

	glBindVertexArray(s_Data.QuadVA);
	glDrawElements(GL_TRIANGLES, s_Data.IndexCount, GL_UNSIGNED_INT, nullptr);

	s_Data.IndexCount = 0;
	s_Data.TextureSlotIndex = 1;
}

// Queue Methods
void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
	//If we run out of indices or textures slots, draw everything and reset the buffers
	if (s_Data.IndexCount >= MaxIndexCount || s_Data.TextureSlotIndex > 31) {
		EndBatch();
		Flush();
		BeginBatch();
	}

	s_Data.QuadBufferPtr->position = glm::vec3(position.x,position.y,0);
	s_Data.QuadBufferPtr->color = color;
	s_Data.QuadBufferPtr->texCoords = { 0.0f,0.0f };
	s_Data.QuadBufferPtr->texIndex = s_Data.WhiteTextureSlot;
	s_Data.QuadBufferPtr->view = s_Data.currentView;
	s_Data.QuadBufferPtr++;

	s_Data.QuadBufferPtr->position = glm::vec3(position.x + size.x, position.y, 0);
	s_Data.QuadBufferPtr->color = color;
	s_Data.QuadBufferPtr->texCoords = { 1.0f,0.0f };
	s_Data.QuadBufferPtr->texIndex = s_Data.WhiteTextureSlot;
	s_Data.QuadBufferPtr->view = s_Data.currentView;
	s_Data.QuadBufferPtr++;

	s_Data.QuadBufferPtr->position = glm::vec3(position.x + size.x, position.y + size.y, 0);
	s_Data.QuadBufferPtr->color = color;
	s_Data.QuadBufferPtr->texCoords = { 1.0f,1.0f };
	s_Data.QuadBufferPtr->texIndex = s_Data.WhiteTextureSlot;
	s_Data.QuadBufferPtr->view = s_Data.currentView;
	s_Data.QuadBufferPtr++;

	s_Data.QuadBufferPtr->position = glm::vec3(position.x, position.y + size.y, 0);
	s_Data.QuadBufferPtr->color = color;
	s_Data.QuadBufferPtr->texCoords = { 0.0f,1.0f };
	s_Data.QuadBufferPtr->texIndex = s_Data.WhiteTextureSlot;
	s_Data.QuadBufferPtr->view = s_Data.currentView;
	s_Data.QuadBufferPtr++;

	s_Data.IndexCount += 6;
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureId) {
	
	//If we run out of indices or textures slots, draw everything and reset the buffers
	if (s_Data.IndexCount >= MaxIndexCount || s_Data.TextureSlotIndex > 31) {
		EndBatch();
		Flush();
		BeginBatch();
	}

	constexpr glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };
	
	//Find the first open texture slot and insert the texture
	float textureIndex = 0.0f;
	for (uint32_t i=1; i < s_Data.TextureSlotIndex; i++) {
		if (s_Data.TextureSlots[i] == textureId) {
			textureIndex = (float)i;
			break;
		}
	}

	if (textureIndex == 0.0f) {
		textureIndex = (float)s_Data.TextureSlotIndex;
		s_Data.TextureSlots[(int)textureIndex] = textureId;
		s_Data.TextureSlotIndex++;
	}

	s_Data.QuadBufferPtr->position = glm::vec3(position.x, position.y, 0);
	s_Data.QuadBufferPtr->color = color;
	s_Data.QuadBufferPtr->texCoords = { 0.0f,0.0f };
	s_Data.QuadBufferPtr->texIndex = textureIndex;
	s_Data.QuadBufferPtr->view = s_Data.currentView;
	s_Data.QuadBufferPtr++;

	s_Data.QuadBufferPtr->position = glm::vec3(position.x + size.x, position.y, 0);
	s_Data.QuadBufferPtr->color = color;
	s_Data.QuadBufferPtr->texCoords = { 1.0f,0.0f };
	s_Data.QuadBufferPtr->texIndex = textureIndex;
	s_Data.QuadBufferPtr->view = s_Data.currentView;
	s_Data.QuadBufferPtr++;

	s_Data.QuadBufferPtr->position = glm::vec3(position.x + size.x, position.y + size.y, 0);
	s_Data.QuadBufferPtr->color = color;
	s_Data.QuadBufferPtr->texCoords = { 1.0f,1.0f };
	s_Data.QuadBufferPtr->texIndex = textureIndex;
	s_Data.QuadBufferPtr->view = s_Data.currentView;
	s_Data.QuadBufferPtr++;

	s_Data.QuadBufferPtr->position = glm::vec3(position.x, position.y + size.y, 0);
	s_Data.QuadBufferPtr->color = color;
	s_Data.QuadBufferPtr->texCoords = { 0.0f,1.0f };
	s_Data.QuadBufferPtr->texIndex = textureIndex;
	s_Data.QuadBufferPtr->view = s_Data.currentView;
	s_Data.QuadBufferPtr++;

	s_Data.IndexCount += 6;
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureId, const glm::vec4& color) {

	//If we run out of indices or textures slots, draw everything and reset the buffers
	if (s_Data.IndexCount >= MaxIndexCount || s_Data.TextureSlotIndex > 31) {
		EndBatch();
		Flush();
		BeginBatch();
	}

	float textureIndex = 0.0f;
	for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
		if (s_Data.TextureSlots[i] == textureId) {
			textureIndex = (float)i;
			break;
		}
	}

	if (textureIndex == 0.0f) {
		textureIndex = (float)s_Data.TextureSlotIndex;
		s_Data.TextureSlots[(int)textureIndex] = textureId;
		s_Data.TextureSlotIndex++;
	}

	s_Data.QuadBufferPtr->position = glm::vec3(position.x, position.y, 0);
	s_Data.QuadBufferPtr->color = color;
	s_Data.QuadBufferPtr->texCoords = { 0.0f,0.0f };
	s_Data.QuadBufferPtr->texIndex = textureIndex;
	s_Data.QuadBufferPtr->view = s_Data.currentView;
	s_Data.QuadBufferPtr++;

	s_Data.QuadBufferPtr->position = glm::vec3(position.x + size.x, position.y, 0);
	s_Data.QuadBufferPtr->color = color;
	s_Data.QuadBufferPtr->texCoords = { 1.0f,0.0f };
	s_Data.QuadBufferPtr->texIndex = textureIndex;
	s_Data.QuadBufferPtr->view = s_Data.currentView;
	s_Data.QuadBufferPtr++;

	s_Data.QuadBufferPtr->position = glm::vec3(position.x + size.x, position.y + size.y, 0);
	s_Data.QuadBufferPtr->color = color;
	s_Data.QuadBufferPtr->texCoords = { 1.0f,1.0f };
	s_Data.QuadBufferPtr->texIndex = textureIndex;
	s_Data.QuadBufferPtr->view = s_Data.currentView;
	s_Data.QuadBufferPtr++;

	s_Data.QuadBufferPtr->position = glm::vec3(position.x, position.y + size.y, 0);
	s_Data.QuadBufferPtr->color = color;
	s_Data.QuadBufferPtr->texCoords = { 0.0f,1.0f };
	s_Data.QuadBufferPtr->texIndex = textureIndex;
	s_Data.QuadBufferPtr->view = s_Data.currentView;
	s_Data.QuadBufferPtr++;

	s_Data.IndexCount += 6;
}

void Renderer::QuadBox(const glm::vec2& position, const glm::vec2& size, const float thickness, const glm::vec4& color) {
	DrawQuad(position + glm::vec2(-thickness,-thickness), { size.x + 2*thickness, thickness }, color);
	DrawQuad(position + glm::vec2(-thickness, size.y), { size.x + 2*thickness, thickness}, color);
	DrawQuad(position + glm::vec2(-thickness, 0), { thickness,size.y }, color);
	DrawQuad(position + glm::vec2(size.x, 0), { thickness,size.y}, color);
}
	
void Renderer::BlackBox(const glm::vec2& position, const glm::vec2& size, const float thickness) {
	DrawQuad(position, size, {0,0,0,.7 });
	DrawQuad(position + glm::vec2(-thickness, -thickness), { size.x + 2 * thickness, thickness }, { 1,1,1,1 });
	DrawQuad(position + glm::vec2(-thickness, size.y), { size.x + 2 * thickness, 2 }, { 1,1,1,1 });
	DrawQuad(position + glm::vec2(-thickness, 0), { 2,size.y }, { 1,1,1,1 });
	DrawQuad(position + glm::vec2(size.x, 0), { 2,size.y }, { 1,1,1,1 });
}

void Renderer::DrawStr(const glm::vec2& position,float scale, std::string str, Font* font) {

	//If we run out of indices or textures slots, draw everything and reset the buffers
	if (s_Data.IndexCount >= MaxIndexCount || s_Data.TextureSlotIndex > 31) {
		EndBatch();
		Flush();
		BeginBatch();
	}

	const glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };

	//Find the first open texture slot and insert the texture
	float textureIndex = 0.0f;
	for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
		if (s_Data.TextureSlots[i] == font->atlasId) {
			textureIndex = (float)i;
			break;
		}
	}

	if (textureIndex == 0.0f) {
		textureIndex = (float)s_Data.TextureSlotIndex;
		s_Data.TextureSlots[(int)textureIndex] =  font->atlasId;
		s_Data.TextureSlotIndex++;
	}

	//Queue a quad for each 

	float penX = 0;
	float penY = 0;

	for (int i = 0; i < str.length(); i++) {

		char charCode = str[i];

		if (charCode == '\n') {
			penX = 0;
			penY += font->lineHeight * scale;
			continue;
		}

		Font::Glyph& glyph = font->glyphs[charCode];

		Vertex* currentVertex = s_Data.QuadBufferPtr;

		s_Data.QuadBufferPtr->position = glm::vec3(position.x + penX, position.y + penY, 0) + glm::vec3(0,glyph.bearing.y+10, 0);
		s_Data.QuadBufferPtr->color = color;
		s_Data.QuadBufferPtr->texCoords = glyph.leftTop;
		s_Data.QuadBufferPtr->texIndex = textureIndex;
		s_Data.QuadBufferPtr->view = s_Data.currentView;
		s_Data.QuadBufferPtr++;

		s_Data.QuadBufferPtr->position = glm::vec3(position.x + penX + glyph.size.x * scale, penY + position.y, 0) + glm::vec3(0,glyph.bearing.y+10, 0);
		s_Data.QuadBufferPtr->color = color;
		s_Data.QuadBufferPtr->texCoords = glyph.rightTop;
		s_Data.QuadBufferPtr->texIndex = textureIndex;
		s_Data.QuadBufferPtr->view = s_Data.currentView;
		s_Data.QuadBufferPtr++;

		s_Data.QuadBufferPtr->position = glm::vec3(position.x + penX + (glyph.size.x * scale), penY + position.y + glyph.size.y * scale, 0) + glm::vec3(0,glyph.bearing.y+10, 0);
		s_Data.QuadBufferPtr->color = color;
		s_Data.QuadBufferPtr->texCoords = glyph.rightBottom;
		s_Data.QuadBufferPtr->texIndex = textureIndex;
		s_Data.QuadBufferPtr->view = s_Data.currentView;
		s_Data.QuadBufferPtr++;

		s_Data.QuadBufferPtr->position = glm::vec3(position.x + penX, penY + position.y + glyph.size.y * scale, 0) + glm::vec3(0,glyph.bearing.y+10, 0);
		s_Data.QuadBufferPtr->color = color;
		s_Data.QuadBufferPtr->texCoords = glyph.leftBottom;
		s_Data.QuadBufferPtr->texIndex = textureIndex;
		s_Data.QuadBufferPtr->view = s_Data.currentView;
		s_Data.QuadBufferPtr++;

		s_Data.IndexCount += 6;
		penX += glyph.advance * scale;

	}
}