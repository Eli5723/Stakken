#include "./States.h"

struct MenuState : State {
    void OnInput();
    void OnLoop(int dt);
    void OnRender(int dt);

    void OnEvent(const SDL_Event& event){
        
    }

    void OnClick(const SDL_Event& event){

    }

    void OnTextInput(const SDL_TextInputEvent& input){

    }

    void OnCleanup();
};