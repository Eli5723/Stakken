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
#include "./Systems/Assets/ProfilePicture.h"

#include "./Systems/Rendering/Renderer.h"
#include "./Systems/Rendering/ScreenQuad.h"

#include "./Systems/Identity.h"

#include "./Gameplay/Game.h"
#include "./Gameplay/RenderGame.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

enum class Space {
    MainGame,
    OtherGames,
    Window,
};

glm::vec2 Resolution{WINDOW_WIDTH,WINDOW_HEIGHT};
struct View {
    Space id;
    
    glm::vec2 canvasSize;

    glm::vec2 position;
    float scale;
    glm::mat4 projection;
    glm::mat4 transform;

    View (Space id, const glm::vec2& canvasSize){
        this->id = id;
        this->canvasSize = canvasSize;
        projection = glm::ortho(0.0f,canvasSize.x,canvasSize.y,0.0f,-1.0f,1.0f);
        transform = glm::mat4(1);
    }

    void Calculate(){
        scale = fmin(Resolution.x / canvasSize.x, Resolution.y / canvasSize.y   );
    }

    void Position(const glm::vec2& position){
        this->position = position;
        transform = glm::mat4(1);
        transform = glm::scale(transform,{scale,scale,1.0f});
        transform = glm::translate(transform,glm::vec3{position.x/scale,position.y/scale,0});

        projection = glm::ortho(0.0f,Resolution.x,Resolution.y,0.0f,-1.0f,1.0f);
    }
};

View MainGameView(Space::MainGame,{1280.0f, 720.0f});
View OtherGameView(Space::OtherGames,{1280.0f*2, 720.0f*2});

// Pointers
BGShader* BackgroundShader;


bool Running = true;
SDL_Window* window;

// Test variables
Game* testGame;
Identity testIdentity{defaultIdentity};
KeyboardMapper* keyboard;
Texture* testTexture;

bool OnInit(){
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

    MainGameView.Calculate();
    Renderer::Init(MainGameView.projection,MainGameView.transform);

    OtherGameView.Calculate();
    OtherGameView.Position({(RenderGame::kGameDimensions.x + RenderGame::kGaps*5) * MainGameView.scale,720.0f - RenderGame::kGameDimensions.y - (float)(RenderGame::kGaps*2)});
    Renderer::SetView(OtherGameView.projection,OtherGameView.transform,1);

    // Initialize Asset Systems
    BGShader::Init();
    ScreenQuad::Init();

    BackgroundShader = new BGShader("./BGShaders/Ocean.frag");

    // Initialize Audio
    if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,4,2048) < 0){
        SDL_Log("Failed to load audio.");
    }

    return true;
}

int OnExecute(){
    if (OnInit() == false)
        return -1;

    
    // TEST CODE
    InputProfile testProfile;
    testProfile.save();
    keyboard = new KeyboardMapper(testProfile);
    testTexture = new Texture("./PieceTextures/TGF2.png");
    testGame = new Game();
    testIdentity.pfp = new ProfilePicture("./Textures/eeli.png");


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
        OnRender();

        //SDL_Log("FPS: %f\n",1000.0f / (float)frameDelta);
    }

    return 0;
}

void OnLoop(int dt){
    keyboard->update(dt);
    testGame->ApplyInput(keyboard->buffer);
    keyboard->buffer.flush();
    testGame->Update(dt);
}

void OnRender(){
    float fTime = (float)SDL_GetTicks()/1000.0f; 

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    //Draw Background
    BackgroundShader->draw(&Resolution.x,fTime);

    // Draw Gameplay
    Renderer::BeginBatch();

    Renderer::TargetView(0);
    RenderGame::SetPixelTickness(ceil(1.0f/MainGameView.scale));
    RenderGame::DrawGame({RenderGame::kGaps*2                    , 720.0f - RenderGame::kGameDimensions.y - RenderGame::kGaps*2}, *testGame, testIdentity, *testTexture);
    
    // const int columns = 10;
    const glm::vec2 kgameAreaBounds({(1280.0f - (RenderGame::kGameDimensions.x + RenderGame::kGaps)) *2,RenderGame::kGameDimensions.y*2});
    

    // Draw other games
    float otherWidth = 1280.0f;
    Renderer::TargetView(1);
    RenderGame::SetPixelTickness(ceilf(1.0f/OtherGameView.scale));

    const int columns = 2;
    float columnOffset = floorf(otherWidth/(float)columns);
    for (int i=0;i<4;i++){
        int row = i / columns;
        int column = i % columns;
        RenderGame::DrawGame({columnOffset * column, (RenderGame::kGameDimensions.y + RenderGame::kMargin) * row}, *testGame, testIdentity, *testTexture);
    }

    Renderer::EndBatch();
    Renderer::Flush();

    SDL_GL_SwapWindow(window);
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
        OnInput(event);
    break;

    default:
    break;
    }
}

void OnInput(SDL_Event& event){
    switch (event.key.keysym.scancode){

        default:
        break;
    }
    
    keyboard->keyEvents(event.key.keysym.scancode);
}

void OnResize(int width, int height){
    Resolution.x = width;
    Resolution.y = height;
    glViewport(0,0,width,height);

    MainGameView.Calculate();
    MainGameView.Position({0,0});
    Renderer::SetView(MainGameView.projection,MainGameView.transform,0);

    OtherGameView.Calculate();
    OtherGameView.Position({(RenderGame::kGameDimensions.x + RenderGame::kGaps*3) * MainGameView.scale,720.0f - RenderGame::kGameDimensions.y - (float)(RenderGame::kGaps*2)});
    Renderer::SetView(OtherGameView.projection,OtherGameView.transform,1);

    SDL_Log("Window resized");
}

void OnCleanup(){
    
}
