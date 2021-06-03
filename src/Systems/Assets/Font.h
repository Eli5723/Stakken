#pragma once

#include <cstdio>
#include <cstring>

#include <glm/glm.hpp>

#include "./LinearTexture.h"

typedef unsigned char Ascii;



class Font {
    public:
    struct Glyph {
        Ascii charcode;
        float advance;
        glm::vec2 topLeft;
        glm::vec2 topRight;
        glm::vec2 bottomLeft;
        glm::vec2 bottomRight;
        glm::vec2 size;
    };

    LinearTexture* atlas;
    Glyph* glyphs;
    float lineHeight = 64;

    Font(const std::string& path){
        // Load atlas
        atlas = new LinearTexture(path + ".png");
        glyphs = new Glyph[255];

        FILE* fp = fopen((path+".csv").c_str(),"r");

        int ascii;
        float advance;
        float left;
        float bottom;
        float right;
        float top;
        
        float leftA;
        float bottomA;
        float rightA;
        float topA;
        float width = 396.0f;
        float height = 396.0f;

        int count = 0;
        while(count < 94) {
            fscanf(fp, "%i%*c%f%*c%f%*c%f%*c%f%*c%f%*c%f%*c%f%*c%f%*c%f*[^\n]", &ascii, &advance, &left, &bottom, &right, &top,&leftA, &bottomA, &rightA, &topA);
            Glyph& glyph = glyphs[ascii];

            left = leftA / width;
            right = rightA / width;
            bottom = (height - bottomA) / height;
            top = (height - topA) / height;

            glyph.charcode = (Ascii)ascii;
            glyph.advance = advance;
            glyph.topLeft = {left,top};
            glyph.topRight = {right,top};
            glyph.bottomLeft = {left,bottom};
            glyph.bottomRight = {right,bottom};
            glyph.size = {rightA - leftA, topA - bottomA};
            count++;
            printf("Char: %c (%i)\n  Advance:%f\n  Left:%f\n  Bottom:%f\n  Right:%f\n  Top:%f\n  Size:%f,%f\n", ascii,count, advance, left, bottom, right, top,glyphs[ascii].size.x,glyphs[ascii].size.y);
        }

        fclose(fp);
    }


    ~Font(){
        delete atlas;
        delete [] glyphs;
    }
    
};