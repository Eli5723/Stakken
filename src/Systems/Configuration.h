#pragma once

#include "Assets/Assets.h"

#include <SDL2/SDL_log.h>
#include <cstdio>
#include <slurp.h>
#include <cstring>

void loadConfiguration(){
    char* configFile = slurp("./settings.cfg");
    
    if (!configFile){
        SDL_LogWarn(0, "Failed to load settings file.\n");
        return;
    }

    int n;
    char** tokens = slurpCFG(configFile, n);
    
    printf("%i Tokens: \n",n);
    for (int i=0; i < n/2; i++)
        printf("%s| %s\n",tokens[i*2],tokens[i*2+1]);
    
    activeAssets.bgShader = shaderCache.get(tokens[1]);
    activeAssets.pieceTexture = textureCache.get(tokens[3]);
    activeAssets.font = fontCache.get(tokens[5]);

    delete [] configFile;
}

void saveConfiguration(){
    FILE* configFile = fopen("./settings.cfg", "wb");

    fprintf(configFile,"BackgroundShader=%s\n", activeAssets.bgShader ?  activeAssets.bgShader->path.c_str() : "");
    fprintf(configFile,"PieceTexture=%s\n", activeAssets.pieceTexture ? activeAssets.pieceTexture->path.c_str() : "");
    fprintf(configFile,"Font=%s\n", activeAssets.font ? activeAssets.font->path.c_str() : "");

    fclose(configFile);
}