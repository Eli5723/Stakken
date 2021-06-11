#pragma once

#include "../Assets/Assets.h"

#include "./UI.h"
#include "./BaseWidgets.h"
#include "../Input/InputProfile.h"
#include "Element.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <glm/fwd.hpp>
#include <string>

#include <HSV.h>

namespace UI {

const std::string icons[] = {
    "left",
    "right",
    "softdrop",
    "harddrop",
    "sonicdrop",
    "rcw",
    "rccw",
    "flip",
    "das",
    "arr",
    "droparr"
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

Element* Options(InputProfile* profile, Identity* identity){
    Element* base = new Element;
    base->size = {200.0f + RenderGame::kPieceDimensions.x * 4,32.0f*11};
    base->flags = Flags::background | Flags::border;

    Element* list = new Element;
    list->size = {200.0f,32.0f*11};
    // Create control buttons
    for (int i=0; i < 8; i++){
        const char* keyName = SDL_GetKeyName((SDL_GetKeyFromScancode((SDL_Scancode)profile->keys[i])));

        Element* label = Label(keyName);
        label->position = {32.0f,i * 32.0f};
        label->size = {list->size.x - 32.0f,32.0f};
        label->flags |= Flags::border;
        list->addChild(label);

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
        
        list->addChild(button);
    }

    base->addChild(list);

    // Das input
    Element* dasInput = NumberInput(profile->das);
    dasInput->size = {list->size.x - 32.0f,32};
    dasInput->position = {32.0f, 32.0f * 8};
    list->addChild(dasInput);

    Element* dasIcon = new Element;
    dasIcon->flags = Flags::texture | Flags::border;
    dasIcon->data.texture = textureCache.get("./Resources/Textures/Icons/" + icons[8] + ".png");
    dasIcon->position = {0.0f, 8 * 32.0f};
    dasIcon->size = {32.0f, 32.0f};
    list->addChild(dasIcon);

    Element* arrInput = NumberInput(profile->arr);
    arrInput->size = {list->size.x - 32.0f,32};
    arrInput->position = {32.0f, 32.0f * 9};
    list->addChild(arrInput);

    Element* arrIcon = new Element;
    arrIcon->flags = Flags::texture | Flags::border;
    arrIcon->data.texture = textureCache.get("./Resources/Textures/Icons/" + icons[9] + ".png");
    arrIcon->position = {0.0f, 9 * 32.0f};
    arrIcon->size = {32.0f, 32.0f};
    list->addChild(arrIcon);

    Element* dropArrInput = NumberInput(profile->dropArr);
    dropArrInput->size = {list->size.x - 32.0f,32};
    dropArrInput->position = {32.0f, 32.0f * 10};
    list->addChild(dropArrInput);

    Element* dropArrIcon = new Element;
    dropArrIcon->flags = Flags::texture | Flags::border;
    dropArrIcon->data.texture = textureCache.get("./Resources/Textures/Icons/" + icons[10] + ".png");
    dropArrIcon->position = {0.0f, 10 * 32.0f};
    dropArrIcon->size = {32.0f, 32.0f};
    list->addChild(dropArrIcon);


    Element* pieceRoot = new Element;
    for (int i = 0; i < 7; i++){
        const int columns = 4;
        int column = i % columns;
        int row = i / columns;
        glm::vec2 position = {column * RenderGame::kPieceDimensions.x, row * (RenderGame::kPieceDimensions.y+64.0f)};

        // Piece (Click to rotate)
        Element* piece = new Element;
        piece->flags = Flags::piece | Flags::border;
        piece->data.piece.type = (TileType)i;
        piece->data.piece.rotation = profile->rotation[i];
        piece->data.piece.colorTable = &identity->color_table;
        piece->size = RenderGame::kPieceDimensions;
        piece->position = position;
        piece->clickCallback = [i,profile,piece](int x, int y){
            profile->rotation[i] = (profile->rotation[i] + 1) % 4;
            piece->data.piece.rotation = profile->rotation[i];
        };
        pieceRoot->addChild(piece);

        // Paint Bucket (Drag to modify HSV)
        Element* paintBucket = new Element;
        paintBucket->flags = Flags::texture;
        paintBucket->data.texture = textureCache.get("./Resources/Textures/Icons/paint-bucket.png");
        paintBucket->position = position + glm::vec2{0,RenderGame::kPieceDimensions.y+1};
        paintBucket->size= {32.0f,32.0f};
        paintBucket->clickCallback = [i,identity](int x, int y){
            //Initializes a static float for each color table entry to the hue of that entry
            static float hue[7]{RGB2H(identity->color_table.entries[0]),RGB2H(identity->color_table.entries[1]),RGB2H(identity->color_table.entries[2]),RGB2H(identity->color_table.entries[3]),RGB2H(identity->color_table.entries[4]),RGB2H(identity->color_table.entries[5]),RGB2H(identity->color_table.entries[6])};
            hue[i] += 5;
            H2RGB(hue[i], identity->color_table.entries[i]);
        };

        pieceRoot->addChild(paintBucket);
    }


    pieceRoot->position = {list->size.x,0};
    pieceRoot->size = {RenderGame::kPieceDimensions.x * 4,list->size.y};
    pieceRoot->flags = Flags::background | border;

    base->addChild(pieceRoot);


    return base;
}



}


