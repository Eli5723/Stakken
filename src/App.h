#pragma once

#include <SDL2/SDL.h>

class Stakken {
    private:
        bool Running;
        SDL_Window* window;
        SDL_Surface* Surf_Display;

    public:
        Stakken(int argc, const char** argv);
        int OnExecute();
        
        bool OnInit();
        void OnEvent(SDL_Event* Event);
        void OnLoop();
        void OnRender();
        void OnCleanup();

};