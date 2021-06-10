#pragma once

#include <SDL2/SDL_log.h>
#include <stb_truetype.h>
#include <slurp.h>
#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <estdint.h>

#include <string>

struct Font {
    std::string path;

    struct Glyph {
        float advance;
        glm::vec2 bearing;
        glm::vec2 size;    
        glm::vec2 leftTop;
        glm::vec2 rightTop;
        glm::vec2 leftBottom;
        glm::vec2 rightBottom;
    };

    GLuint atlasId;

    float ascent;
    float descent;
    float lineGap;
    float lineHeight;

    Glyph glyphs[255];

    Font(const std::string& path);

    // Generates an atlas for a font if one has not been saved yet
    void GenerateAtlas(const std::string& path);
};