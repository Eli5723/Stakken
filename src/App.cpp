#include "App.h"

#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "./Systems/Input/KeyboardMapper.h"
#include "./Systems/Input/InputBuffer.h"
#include "./Systems/Input/InputProfile.h"

#include "./Systems/Assets/Sound.h"
#include "./Systems/Assets/Texture.h"
#include "./Systems/Assets/BGShader.h"

#include "./Systems/Rendering/Renderer.h"
#include "./Systems/Rendering/ScreenQuad.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

BGShader* testShader;
float testRes[2]{640.0f,480.0f};

glm::vec2 box{0,0};
glm::vec2 boxVel{100,0};

Stakken::Stakken(int argc, const char** argv){
    Running = true;
}

bool Stakken::OnInit(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    // Initialize Graphics
    gl3wInit();

    window = SDL_CreateWindow("Stakken",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GL_CreateContext(window);

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(window);

    glm::mat4 testProjection = glm::ortho(0.0f,(float)WINDOW_WIDTH,(float)WINDOW_HEIGHT,0.0f,-1.0f,1.0f);
    glm::mat4 testTransform = glm::mat4(1);
    
    Renderer::Init(testProjection,testTransform);

    // Initialize Asset Systems
    BGShader::Init();
    ScreenQuad::Init();

    testShader = new BGShader("./BGShaders/Ocean.frag");

    // Initialize Audio
    if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,4,2048) < 0){
        SDL_Log("Failed to load audio.");
    }

    return true;
}

int Stakken::OnExecute(){
    if (OnInit() == false)
        return -1;

    SDL_Event event;

    InputProfile testProfile;
    testProfile.save();
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
                        SDL_Log("Window %d resized to to %dx%d", event.window.windowID, event.window.data1, event.window.data2);
                        testRes[0] = event.window.data1;
                        testRes[1] = event.window.data2;
                        glViewport(0,0,event.window.data1,event.window.data2);
                        break;
                        case SDL_WINDOWEVENT_MOVED:
                        SDL_Log("Window %d moved to %d,%d", event.window.windowID, event.window.data1, event.window.data2);
                        break;
                    }
                }

                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode){
                        case SDL_SCANCODE_A:
                        default:
                        break;
                    }
                break;
                default:
                break;
           }
        }

        OnLoop(frameDelta);
        OnRender();
    }

    return 0;
}

void Stakken::OnEvent(SDL_Event* Event){
    

}

void Stakken::OnLoop(int dt){
    float delta = ((float)dt / 1000.0f);
    boxVel.y += 30.0f*delta;

    box += boxVel *delta;
}

void Stakken::OnRender(){
    float fTime = (float)SDL_GetTicks()/1000.0f; 

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw Background
    testShader->draw(testRes,fTime);

    // Draw Gameplay
    Renderer::BeginBatch();

    Renderer::DrawQuad(box,glm::vec2{20,20},glm::vec4{1,1,1,1});

    Renderer::EndBatch();
    Renderer::Flush();

    SDL_GL_SwapWindow(window);
}

void Stakken::OnCleanup(){
    
}
