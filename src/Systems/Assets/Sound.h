#pragma once
#include <SDL2/SDL_mixer.h>

#include <string>

class Sound {
	public:
		std::string path;

		Mix_Chunk* chunk = NULL;
		Sound(const std::string& path);
        ~Sound();
		void play();
};