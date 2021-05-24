#include "App.h"

#include <SDL2/SDL_opengl.h>

#include "./Systems/InputProfile.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;


Stakken::Stakken(int argc, const char** argv){
    Running = true;
}

int Stakken::OnExecute(){

    if (OnInit() == false)
        return -1;

    SDL_Event event;

    while (Running){
        while(SDL_PollEvent(&event)){
           OnEvent(&event);

           switch(event.type){
                case SDL_QUIT:
                    Running = false;
                    break;
                case SDL_WINDOWEVENT: {
                    switch (event.window.event){
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                        SDL_Log("Window %d moved to %d %d", event.window.windowID, event.window.data1, event.window.data2);
                        break;
                        case SDL_WINDOWEVENT_MOVED:
                        SDL_Log("Window %d moved to %d %d", event.window.windowID, event.window.data1, event.window.data2);
                        break;
                    }
                }

                case SDL_KEYDOWN:
                switch(event.key.keysym.scancode){
                    case SDL_SCANCODE_UP:
                    break;
                    case SDL_SCANCODE_DOWN:
                    break;
                    case SDL_SCANCODE_RIGHT:
                    break;
                    case SDL_SCANCODE_LEFT:
                    break;
                    default:
                    break;
                }
                break;

           }
        }

        OnLoop();
        OnRender();
        SDL_Delay(16);
    }


    return 0;
}

bool Stakken::OnInit(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    window = SDL_CreateWindow("Stakken",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    Surf_Display = SDL_GetWindowSurface(window);

    SDL_FillRect(Surf_Display, NULL, SDL_MapRGB(Surf_Display->format,0xFF,0xFF,0xFF));
    SDL_UpdateWindowSurface(window);

    if(Surf_Display ==  NULL) {
        return false;
    }
    return true;
}

void Stakken::OnEvent(SDL_Event* Event){
    


}

void Stakken::OnLoop(){

}

void Stakken::OnRender(){
    SDL_FillRect(Surf_Display, NULL, SDL_MapRGB(Surf_Display->format,0xFF,0xFF,0xFF));
    SDL_UpdateWindowSurface(window);
}

void Stakken::OnCleanup(){
    
}
