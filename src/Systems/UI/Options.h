#pragma once

#include "../Assets/Assets.h"

#include "./UI.h"
#include "./BaseWidgets.h"
#include "../Input/InputProfile.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <string>


namespace UI {

const std::string icons[] = {
    "left",
    "right",
    "softdrop",
    "harddrop",
    "sonicdrop",
    "rcw",
    "rccw",
    "flip"
};

const std::string labels[] = {
    "Move Left",
    "Move Right",
    "Soft Drop",
    "Hard Drop",
    "Sonic Drop",
    "Rotate Clockwise",
    "Rotate Counterclockwise",
    "Flip"
};

Element* InputOptions(InputProfile* profile){
    Element* base = new Element;
    base->size = {300.0f,32.0f*8};
    base->flags = Flags::background | Flags::border;

    // Create keyboard button
    for (int i=0; i < 8; i++){
        const char* keyName = SDL_GetKeyName((SDL_GetKeyFromScancode((SDL_Scancode)profile->keys[i])));

        Element* label = Label(keyName);
        label->position = {42,i * 32.0f};
        base->addChild(label);

        Element* button = new Element;
        button->flags = Flags::texture | Flags::border;
        button->data.texture = textureCache.get("./Resources/Textures/Icons/" + icons[i] + ".png");
        button->position = {0.0f, i * 32.0f};
        button->size = {32.0f, 32.0f};
        
        button->clickCallback = [label](int x,int y) {label->setText("Waiting for input...");};
        
        button->keyCallback = [i, label, profile](const SDL_KeyboardEvent& event) {
            const char* keyName = SDL_GetKeyName(event.keysym.sym);
            profile->keys[i] = event.keysym.scancode;
            label->setText(keyName);
            clearFocus();
        };
        
        base->addChild(button);
    }

    return base;
}



}


