#include "./Sound.h"

#include <SDL2/SDL.h>

#include <filesystem>

Sound::Sound(const std::string& location){
    chunk = Mix_LoadWAV(location.c_str());
    if (!chunk){
        SDL_Log("Failed to load sound at %s", location.c_str());
    }
}

Sound::~Sound(){
    Mix_FreeChunk(chunk);
}

void Sound::play(){
    Mix_PlayChannel(-1,chunk,0);
}