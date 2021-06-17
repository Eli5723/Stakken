#pragma once

#include <GL/gl3w.h>
#include <string>

#
class BGShader {
    public:
        std::string path;
        static void Init();

        GLuint ID;
        BGShader(const char* path);
        void use();
        void setVec2(const char* name, float* value);
        void setFloat(const char* name, GLfloat value);
        void draw(float* res, float time);
};