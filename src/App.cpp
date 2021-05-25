#include "App.h"

#include <GL/gl3w.h>
#include <SDL2/SDL.h>

#include "./Systems/Input/KeyboardMapper.h"
#include "./Systems/Input/InputBuffer.h"
#include "./Systems/Input/InputProfile.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

int x = 0;
Stakken::Stakken(int argc, const char** argv){
    Running = true;
}

int Stakken::OnExecute(){
    if (OnInit() == false)
        return -1;

    SDL_Event event;

    InputProfile testProfile;
    KeyboardMapper testMapper(testProfile);

    int frameBegin = SDL_GetTicks();
    int frameDelta = 0;

    while (Running) {
        int newTime = SDL_GetTicks();
        frameDelta = newTime - frameBegin; 
        frameBegin = newTime;

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

                break;

           }
        }

        OnLoop(frameDelta);
        OnRender();
    }

    return 0;
}

bool Stakken::OnInit(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    gl3wInit();

    window = SDL_CreateWindow("Stakken",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GL_CreateContext(window);

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(window);

    if(Surf_Display ==  NULL) {
        return false;
    }

    return true;
}

void Stakken::OnEvent(SDL_Event* Event){
    

}

void Stakken::OnLoop(int dt){
    
}

void Stakken::OnRender(){
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    

    SDL_GL_SwapWindow(window);
}

void Stakken::OnCleanup(){
    
}
