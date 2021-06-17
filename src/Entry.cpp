#include "./Entry.h"
#include "Systems/Assets/BGShader.h"
#include "Systems/Assets/LinearTexture.h"
#include "Systems/UI/Element.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <algorithm>
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

#include "./Globals.h"
#include "./Systems/Configuration.h"

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

#include "./Gameplay/Replay.h"
#include "./Systems/Input/ReplayMapper.h"

#include <cstdio>
#include <time.h>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

glm::vec2 GameResolution = {1280.0f,720.0f};

glm::vec2 Resolution{WINDOW_WIDTH,WINDOW_HEIGHT};

bool Running = true;

struct CientState {
    Game* game;
    Identity* identity;
    InputProfile* inputProfile;
    KeyboardMapper* keyboard;


    Game* secondGame;
    ReplayMapper* replayMapper;

    UI::Element* optionsUI;

    vector<Game*> otherGames;
    vector<Identity*> otherPlayers;
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

    // Initialize Audio
    if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,4,2048) < 0){
        SDL_Log("Failed to load audio.");
    }

    // Initialize Asset Systems
    BGShader::Init();
    ScreenQuad::Init();

    //Load configuration
    loadConfiguration();
    // activeAssets.bgShader = shaderCache.get(shaderList.files[0].c_str());
    // activeAssets.pieceTexture = 0;//textureCache.get(textu;reList.files[5])
    // activeAssets.font =  fontCache.get(fontList.files[1].c_str());

    activeAssets.sound_lock = soundCache.get("./Resources/Sounds/place.wav");
    activeAssets.sound_lineclear = soundCache.get("./Resources/Sounds/clear.WAV");

    // Initialize Client state
    clientState.identity = new Identity(defaultIdentity);
    clientState.identity->pfp = new LinearTexture("./Resources/Textures/pfp/eeli.png");
    clientState.inputProfile = new InputProfile("Default");
    clientState.keyboard = new KeyboardMapper(*clientState.inputProfile);
    clientState.game = new Game(clientState.inputProfile, clock());


    clientState.optionsUI = UI::Options(clientState.inputProfile, clientState.identity);
    UI::addToScreen(clientState.optionsUI);
    clientState.optionsUI->enabled = false;
    clientState.optionsUI->position = {RenderGame::kGameDimensions.x + RenderGame::kGaps, 100.0f};


    // Replay Test
    clientState.game->recorder = new Recorder();

    Replay* testReplay = new Replay("./Replays/test.rep");
    clientState.secondGame = new Game(clientState.inputProfile,testReplay->seed);
    clientState.replayMapper = new ReplayMapper(*testReplay);
    

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

    OnCleanup();
    return 0;
}

// Main loop
void 
OnLoop(int dt){

    // Update the game if it's being played
    if (clientState.game->state == Game::GameState::Playing){
        clientState.keyboard->update(dt);
        clientState.game->ApplyInput(clientState.keyboard->buffer);
        clientState.keyboard->buffer.flush();
        clientState.game->Update(dt);
    }

    if (clientState.secondGame->state == Game::GameState::Playing) {
        clientState.replayMapper->update(dt);
        clientState.secondGame->ApplyInput(clientState.replayMapper->buffer);
        clientState.replayMapper->flush();
        clientState.secondGame->Update(dt);
    }
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
    Renderer::TargetTransform(0);

    RenderGame::DrawGame({RenderGame::kGaps,64.0f},*clientState.game,*clientState.identity, activeAssets.pieceTexture);


    RenderGame::DrawGame({GameResolution.x - RenderGame::kGaps - RenderGame::kGameDimensions.x,64.0f},*clientState.secondGame,*clientState.identity, activeAssets.pieceTexture);


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

    case SDL_MOUSEMOTION:
        UI::moveCapture(event.motion);
    break;

    case SDL_MOUSEBUTTONUP:
        UI::endDrag();
    break;

    case SDL_TEXTINPUT:
        OnTextInput(event.text);
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
            default:
            break;
        }
        return;
    }

    // Allow the UI to capture input
    if (UI::keyCapture(key))
        return;
    
    // Function keys
    switch (key.keysym.scancode){
        case SDL_SCANCODE_F1:
            clientState.optionsUI->enabled = !clientState.optionsUI->enabled;
        return;
        break;
        case SDL_SCANCODE_F2:
            clientState.game->Reset(clock());
            clientState.game->state = Game::GameState::Playing;        
            return;
        break;
        case SDL_SCANCODE_F5:
            activeAssets.nextShader();
        break;
        case SDL_SCANCODE_F6:
            activeAssets.nextPieceTexture();
        break;
        case SDL_SCANCODE_F7:
            activeAssets.nextFont();
        break;
        case SDL_SCANCODE_F8:
            RenderGame::cycleOutlineStyles();
        break;

        case SDL_SCANCODE_F9:
        if (clientState.game->recorder)
            clientState.game->recorder->save(" ");
        break;
        
        default:
        break;
    }

    // Process game key events if the game is being palyed
    if (clientState.game->state == Game::GameState::Playing && key.repeat == 0){
        clientState.keyboard->keyEvents(key.keysym.scancode);
    }
}

void
OnClick(const SDL_Event& event){
    UI::click(event.button.x,event.button.y);
}

void
OnTextInput(const SDL_TextInputEvent& input){
    UI::inputCapture(input);
}

void 
OnResize(int width, int height){
    Resolution.x = width;
    Resolution.y = height;
    glViewport(0,0,width,height);

    glm::vec2 scaleVec = Resolution / GameResolution ;
    float scaleFactor = std::min(scaleVec.x,scaleVec.y);

    glm::vec2 effectiveSize = GameResolution  * scaleFactor; 
    glm::vec2 offset = (Resolution /2.0f) - (effectiveSize/2.0f) ;

    glm::mat4 viewTransform = glm::mat4(1.0f);
    viewTransform = glm::scale(viewTransform, glm::vec3{ scaleFactor,scaleFactor,0 });
    viewTransform = glm::translate(viewTransform, glm::vec3{ offset.x/scaleFactor,offset.y / scaleFactor,0 });
    Renderer::UpdateTransform(viewTransform, 0);

    glm::mat4 viewProjection = glm::ortho(0.0f, Resolution.x, Resolution.y, 0.0f, -1.0f, 1.0f);
    Renderer::UpdateProjection(viewProjection);

    UI::Resize(Resolution);
}

void OnCleanup(){
    Renderer::Shutdown();
    saveConfiguration();
}
