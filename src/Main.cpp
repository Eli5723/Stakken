#include "App.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main(int argc, const char** argv){
    Stakken game = Stakken(argc,argv);
    return game.OnExecute();
}