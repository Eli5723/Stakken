#include <iostream>

enum class ScreenMode {
    Windowed,
    Fullscreen,
    BorderlessFullscreen
};


struct Configuration {
    int screenMode;

    char* pieceTexture = 0;
    char* bgShader = 0;
    char* background = 0;

    char* sound_lineclear = 0;
    char* sound_sonicdrop = 0;
    char* sound_harddrop = 0;
    char* sound_lock = 0;

    char* sound_combo5 = 0;
    char* sound_combo7 = 0;
    char* sound_combo10 = 0;
    char* sound_combo12 = 0;
    char* sound_combo14 = 0;

    void load(const char* path){

    }

    void save(const char* path){

    }
};