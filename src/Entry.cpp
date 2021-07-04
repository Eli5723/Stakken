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
#include <eutility.h>

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
#include "./Gameplay/ReplayViewer.h"

#include <cstdio>
#include <time.h>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

glm::vec2 GameResolution = {1280.0f,720.0f};
glm::vec2 Resolution{WINDOW_WIDTH,WINDOW_HEIGHT};
float scaleFactor;
glm::vec2 offset;

bool Running = true;

struct CientState {
    Game* game;
    Identity* identity;
    InputProfile* inputProfile;
    KeyboardMapper* keyboard;


    ReplayViewer* replayViewer;

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

    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

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


    // The main board always has a replay player
    clientState.game->recorder = new Recorder();

    

    Replay* testReplay = new Replay("./Replays/DEMO.rep");
    Game* secondGame = new Game(clientState.inputProfile,testReplay->seed);
    ReplayMapper* replayMapper = new ReplayMapper(*testReplay);
    
    ReplayViewer* viewer = new ReplayViewer(*testReplay,*replayMapper, *secondGame);
    clientState.replayViewer = viewer;

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

        OnLoop(frameDelta,newTime);
        OnRender(frameDelta,newTime);
    }

    OnCleanup();
    return 0;
}

// Main loop
void 
OnLoop(int dt, int time){
    
    // Update the game if it's being played
    if (clientState.game->state == Game::GameState::Playing){
        clientState.keyboard->update(dt);
        clientState.game->ApplyInput(clientState.keyboard->buffer);
        clientState.keyboard->buffer.flush();
        clientState.game->Update(dt);
    }

    clientState.replayViewer->advance(dt);
}

static int selectedItem = 0;
const int kItems = 4;
void 
OnRender(int dt, int time){
    float fTime = (float)SDL_GetTicks()/1000.0f; 

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    //Draw Background
    activeAssets.bgShader->draw(&Resolution.x,fTime);

    // Draw Gameplay
    Renderer::BeginBatch();
    
    Renderer::TargetTransform(0);
    RenderGame::DrawGame({RenderGame::kGaps,64.0f},*clientState.game,*clientState.identity, activeAssets.pieceTexture);
    RenderGame::DrawGame({GameResolution.x - RenderGame::kGaps - RenderGame::kGameDimensions.x,64.0f},clientState.replayViewer->game,*clientState.identity, activeAssets.pieceTexture);


    // Menu Test Code
    // Texture* logo = textureCache.get("./Resources/Textures/logo.png");
    // glm::vec2 logoPosition = glm::vec2{GameResolution.x / 2.0f, GameResolution.y / 2.5f} - logo->size / 2.0f;
    // Renderer::DrawQuad(logoPosition + glm::vec2{4,4}, logo->size, logo->id, {0, 0, 0, 1});
    // Renderer::DrawQuad(logoPosition, logo->size, logo->id, {1, 1, 1, 1});

    // float intensity = sinf(time*2 / 1000.0f)*.25f + .5f;
    // float stride = activeAssets.font->lineHeight*.5f;
    // glm::vec2 cursor = GameResolution / 2.0f;
    // Renderer::DrawStrBacked(cursor, 1.0f,"Online", activeAssets.font, (selectedItem == 0 ? glm::vec4{intensity,intensity,intensity,1} : glm::vec4{1,1,1,1}) );
    // cursor.y+= stride;
    // Renderer::DrawStrBacked(cursor, 1.0f,"Challenge", activeAssets.font,(selectedItem == 1 ? glm::vec4{intensity,intensity,intensity,1} : glm::vec4{1,1,1,1}));
    // cursor.y+= stride;
    // Renderer::DrawStrBacked(cursor, 1.0f,"Settings", activeAssets.font,(selectedItem == 2 ? glm::vec4{intensity,intensity,intensity,1} : glm::vec4{1,1,1,1}));
    // cursor.y+= stride;
    // Renderer::DrawStrBacked(cursor, 1.0f,"Quit", activeAssets.font,(selectedItem == 3 ? glm::vec4{intensity,intensity,intensity,1} : glm::vec4{1,1,1,1}));
    


    Renderer::TargetTransform(1);
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

    case SDL_DROPFILE:{
        printf("File path: %s\n", event.drop.file);
        /*
        Replay* testReplay = new Replay(event.drop.file);
        clientState.secondGame->Reset(testReplay->seed);
        clientState.replayMapper->setReplay(*testReplay);

        //= new Game(clientState.inputProfile,testReplay->seed);
        clientState.replayMapper = new ReplayMapper(*testReplay);
        clientState.secondGame->state = Game::GameState::Playing;

        */
        SDL_free(event.drop.file);
    }break;

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
        case SDL_SCANCODE_LEFT:
            selectedItem = (selectedItem - 1);
            if (selectedItem < 0){
                selectedItem = 3;
            }
        break;
        case SDL_SCANCODE_RIGHT:
            selectedItem = (selectedItem + 1) % 4;
        break;

        case SDL_SCANCODE_UP:
            selectedItem = (selectedItem - 1);
            if (selectedItem < 0){
                selectedItem = 3;
            }
        break;
        case SDL_SCANCODE_DOWN:
            selectedItem = (selectedItem + 1) % 4;
        break;


        case SDL_SCANCODE_F1:
            clientState.optionsUI->enabled = !clientState.optionsUI->enabled;
        return;
        break;
        case SDL_SCANCODE_F2:
            clientState.game->Reset(clock());
            clientState.game->state = Game::GameState::Playing;        
            return;
        break;
        case SDL_SCANCODE_F3:
            clientState.game->Lose();        
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

        // Save replay of current game with the path being the current timestamp
        case SDL_SCANCODE_F9: {
            char* replayName = timestring();

            if (clientState.game->recorder)
                clientState.game->recorder->save(replayName);

            delete [] replayName;
        } break;
        
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
    scaleFactor = std::min(scaleVec.x,scaleVec.y);

    glm::vec2 effectiveSize = GameResolution  * scaleFactor; 
    offset = (Resolution /2.0f) - (effectiveSize/2.0f) ;

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
