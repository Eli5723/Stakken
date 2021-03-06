#include "Font.h"
#include <SDL2/SDL_log.h>

inline void stamp(u8* target, u8* source, int x, int y, int width, int height, int stride) {
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            target[(x + i) + (y + j) * stride] = source[i + j * width];
        }
    }
}


Font::Font(const char* path){
    this->path = path;

    // TODO: Atlas Cache
    GenerateAtlas(path);
}

// Generates an atlas for a font if one has not been saved yet
void Font::GenerateAtlas(const char* path){
    
    unsigned const char* fontFile = slurpuc(path);
    if (!fontFile){
        SDL_LogWarn(0, "Failed to load font %s", path);
        return;
    }


    static stbtt_fontinfo* fontInfo = new stbtt_fontinfo;
    stbtt_InitFont(fontInfo, fontFile,0);
    
    // Calculate Atlas Size
    float scale = stbtt_ScaleForPixelHeight(fontInfo, 64);
    int totalWidth = 0;
    int maxHeight = 0;
    int x0,y0,x1,y1, width, height;
    for (int charCode = 0; charCode < 255; charCode++){
        stbtt_GetCodepointBitmapBox(fontInfo, charCode, scale, scale, &x0, &y0, &x1, &y1);
        width = x1-x0;
        height = y1-y0;
        totalWidth += width;
        maxHeight = maxHeight > height ? maxHeight : height;
    }

    if (totalWidth == 0 || maxHeight == 0) {
        SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "Failed to load font: %s", path);
        return;
    }

    // Calculate font vertical metrics
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(fontInfo, &ascent, &descent, &lineGap);
    this->ascent = (float)ascent * scale;
    this->descent = (float)descent * scale;
    this->lineGap = (float)lineGap * scale;
    this->lineHeight = (float)(ascent - descent + lineGap) * scale;



    // Allocate atlas Bitmap
    u8* atlasBitmap = new u8[totalWidth * maxHeight];

    // TODO: Improve packing algorthim
    int penX = 0;
    int offsetX, offsetY, advanceWidth, leftSideBearing;
    for (int charCode = 0; charCode < 255; charCode++){
        // Add Glyph to bitmap
        u8* bitmap = stbtt_GetCodepointBitmap(fontInfo, scale, scale, charCode, &width, &height, &offsetX, &offsetY);
        stamp(atlasBitmap,bitmap,penX,0,width,height,totalWidth);
        
        stbtt_GetCodepointHMetrics(fontInfo, charCode, &advanceWidth, &leftSideBearing);

        // Store Glyph Record
        Glyph& glyph = glyphs[charCode];
        glyph.advance = advanceWidth * scale;
        glyph.bearing = {leftSideBearing * scale, offsetY};
        glyph.size = {width, height};
        glyph.leftTop = {(float)penX/(float)totalWidth,0};
        glyph.rightTop = {(float)(penX+width)/(float)totalWidth,0};
        glyph.leftBottom = {(float)penX/(float)totalWidth,(float)height/(float)maxHeight};
        glyph.rightBottom = {(float)(penX+width)/(float)totalWidth,(float)height/(float)maxHeight};


        penX+= width;
        stbtt_FreeBitmap(bitmap,0);
    }        

    glGenTextures(1, &atlasId);
    glBindTexture(GL_TEXTURE_2D, atlasId);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, totalWidth, maxHeight, 0, GL_RED, GL_UNSIGNED_BYTE, atlasBitmap);

    delete[] atlasBitmap;
    delete[] fontFile;
}