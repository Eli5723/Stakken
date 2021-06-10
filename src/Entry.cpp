#include "./Entry.h"
#include "Systems/Assets/BGShader.h"
#include "Systems/Assets/LinearTexture.h"

#include <SDL2/SDL_events.h>
#include <vector>
using std::vector;

#include <GL/gl3w.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "./Systems/Input/KeyboardMapper.h"
#include "./Systems/Input/InputBuffer.h"
#include "./Systems/Input/InputProfile.h"

#include "./Systems/Assets/Assets.h"

#include "./Systems/Rendering/Renderer.h"
#include "./Systems/Rendering/ScreenQuad.h"

#include "./Systems/Identity.h"

#include "./Gameplay/Game.h"
#include "./Gameplay/RenderGame.h"

#include "./Systems/UI/UI.h"
#include "./Systems/UI/Options.h"

#include <cstdio>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
glm::vec2 Resolution{WINDOW_WIDTH,WINDOW_HEIGHT};

bool Running = true;
SDL_Window* window;

struct CientState {
    Game* game;
    Identity* identity;
    InputProfile* inputProfile;
    KeyboardMapper* keyboard;

    vector<Game*> otherGames;
    vector<Identity*> otherPlayers;

    int currentBGShader = 0;
    int currentTexture = 0;
} clientState;

// Init / Entry
bool 
OnInit(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    // Initialize Graphics
    gl3wInit();

    window = SDL_CreateWindow("Stakken",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(0);
    glClearColor(0,0,0,1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    Resolution = glm::vec2{WINDOW_WIDTH,WINDOW_HEIGHT};
    glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);

    Renderer::Init(Resolution);

    UI::Init(Resolution);

    // Initialize Asset Systems
    BGShader::Init();
    ScreenQuad::Init();
    activeAssets.bgShader = shaderCache.get(shaderList.files[clientState.currentBGShader%shaderList.files.size()]);
    activeAssets.pieceTexture = textureCache.get(textureList.files[5]);
    activeAssets.font =  fontCache.get(fontList.files[1]);

    // Initialize Audio
    if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,4,2048) < 0){
        SDL_Log("Failed to load audio.");
    }

    // Initialize Client state
    clientState.game = new Game();
    clientState.identity = new Identity(defaultIdentity);
    clientState.identity->pfp = new LinearTexture("./Resources/Textures/eeli.png");
    clientState.inputProfile = new InputProfile;
    clientState.keyboard = new KeyboardMapper(*clientState.inputProfile);

    UI::Element* inputOptions = UI::InputOptions(clientState.inputProfile);
    inputOptions->position = {600,32};
    UI::addToScreen(inputOptions);


    return true;
}

int 
OnExecute(){
    if (OnInit() == false)
        return -1;

    int frameBegin = SDL_GetTicks();
    int frameDelta = 0;

    SDL_Event event;
    while (Running) {
        int newTime = SDL_GetTicks();
        frameDelta = newTime - frameBegin; 
        frameBegin = newTime;

        while(SDL_PollEvent(&event)){
           OnEvent(event);
        }

        OnLoop(frameDelta);
        OnRender(frameDelta);
    }

    return 0;
}

// Main loop
void 
OnLoop(int dt){
    clientState.keyboard->update(dt);
    clientState.game->ApplyInput(clientState.keyboard->buffer);
    clientState.keyboard->buffer.flush();
    
    clientState.game->Update(dt);
}

void 
OnRender(int dt){
    float fTime = (float)SDL_GetTicks()/1000.0f; 

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    //Draw Background
    activeAssets.bgShader->draw(&Resolution.x,fTime);

    // Draw Gameplay
    Renderer::BeginBatch();
    Renderer::TargetView(0);

    RenderGame::DrawGame({RenderGame::kGaps,64.0f},*clientState.game,*clientState.identity,*activeAssets.pieceTexture);

    UI::Render();

    Renderer::EndBatch();
    Renderer::Flush();

    SDL_GL_SwapWindow(window);
}

// Event Handling
void 
OnEvent(const SDL_Event& event){
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
        OnInput(event.key);
    break;

    case SDL_MOUSEBUTTONDOWN:
        OnClick(event);
    break;

    default:
    break;
    }
}

void 
OnInput(const SDL_KeyboardEvent& key){

    // Keyboard Shorcuts
    if (key.keysym.mod & KMOD_ALT) {
        switch (key.keysym.scancode){
            case SDL_SCANCODE_RETURN: {
                int fullscreenStatus = SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN;
                SDL_SetWindowFullscreen(window, fullscreenStatus ? 0 : SDL_WINDOW_FULLSCREEN);
            } break;
            
            // Toggle Borderless
            case SDL_SCANCODE_B: {
                int borderStatus = SDL_GetWindowFlags(window) & SDL_WINDOW_BORDERLESS;
                SDL_SetWindowBordered(window, borderStatus ? SDL_TRUE : SDL_FALSE);
            } break;

            case SDL_SCANCODE_E: {
                int flags = SDL_GetWindowFlags(window);
                if(flags & SDL_WINDOW_FULLSCREEN)         printf("SDL_WINDOW_FULLSCREEN\n");
                if(flags & SDL_WINDOW_OPENGL)             printf("SDL_WINDOW_OPENGL\n");
                if(flags & SDL_WINDOW_SHOWN)              printf("SDL_WINDOW_SHOWN\n");
                if(flags & SDL_WINDOW_HIDDEN)             printf("SDL_WINDOW_HIDDEN\n");
                if(flags & SDL_WINDOW_BORDERLESS)         printf("SDL_WINDOW_BORDERLESS\n");
                if(flags & SDL_WINDOW_RESIZABLE)          printf("SDL_WINDOW_RESIZABLE\n");
                if(flags & SDL_WINDOW_MINIMIZED)          printf("SDL_WINDOW_MINIMIZED\n");
                if(flags & SDL_WINDOW_MAXIMIZED)          printf("SDL_WINDOW_MAXIMIZED\n");
                if(flags & SDL_WINDOW_INPUT_GRABBED)      printf("SDL_WINDOW_INPUT_GRABBED\n");
                if(flags & SDL_WINDOW_INPUT_FOCUS)        printf("SDL_WINDOW_INPUT_FOCUS\n");
                if(flags & SDL_WINDOW_MOUSE_FOCUS)        printf("SDL_WINDOW_MOUSE_FOCUS\n");
                if(flags & SDL_WINDOW_FULLSCREEN_DESKTOP) printf("SDL_WINDOW_FULLSCREEN_DESKTOP\n");
                if(flags & SDL_WINDOW_FOREIGN)            printf("SDL_WINDOW_FOREIGN\n");
            } break;

            default:
            break;
        }
        return;
    }

    // Allow the UI to capture input
    if (UI::keyCapture(key))
        return;
    
    // Send input to the board
    clientState.keyboard->keyEvents(key.keysym.scancode);
}

void
OnClick(const SDL_Event& event){
    UI::click(event.button.x,event.button.y);
}

void 
OnResize(int width, int height){
    Resolution.x = width;
    Resolution.y = height;
    glViewport(0,0,width,height);
    Renderer::SetResolution({width,height});

    UI::Resize(Resolution);
}

void OnCleanup(){
    Renderer::Shutdown();
}
