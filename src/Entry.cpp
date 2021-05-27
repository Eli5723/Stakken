#include "./Entry.h"

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
float testRes[2]{WINDOW_WIDTH,WINDOW_HEIGHT};

glm::vec2 box{0,64};
glm::vec2 boxVel{200,0};

bool Running = true;
SDL_Window* window;

bool OnInit(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    // Initialize Graphics
    gl3wInit();

    window = SDL_CreateWindow("Stakken",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GL_CreateContext(window);

    glClearColor(0,0,0,1);

    glm::mat4 testProjection = glm::ortho(0.0f,testRes[0],testRes[1],0.0f,-1.0f,1.0f);
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

int OnExecute(){
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
           OnEvent(event);
        }

        OnLoop(frameDelta);
        OnRender();
    }

    return 0;
}

void OnEvent(SDL_Event& event){
    switch(event.type){
    
    case SDL_QUIT:
        Running = false;
        break;
    
    case SDL_WINDOWEVENT: {
        switch (event.window.event){
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                OnResize(event.window.data1,event.window.data2);
            break;
        }
    }

    case SDL_KEYDOWN:
        switch (event.key.keysym.scancode){

            default:
            break;
        }
    break;

    default:
    break;
    }
}

void OnLoop(int dt){
    // float delta = ((float)dt / 1000.0f);
}

void OnRender(){
    float fTime = (float)SDL_GetTicks()/1000.0f; 

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw Background
    testShader->draw(testRes,fTime);

    // Draw Gameplay
    Renderer::BeginBatch();

    Renderer::EndBatch();
    Renderer::Flush();

    // Draw Other Boards
    Renderer::BeginBatch();

    Renderer::EndBatch();
    Renderer::Flush();

    SDL_GL_SwapWindow(window);
}

// Window Events
void OnResize(int width, int height){
    testRes[0] = width;
    testRes[1] = height;
    glViewport(0,0,width,height);
    glm::mat4 viewProjection = glm::ortho(0.0f,(float)width,(float)height,0.0f,-1.0f,1.0f);
    glm::mat4 viewTransform = glm::mat4(1);
    Renderer::SetView(viewProjection,viewTransform);
}

void OnCleanup(){
    
}
