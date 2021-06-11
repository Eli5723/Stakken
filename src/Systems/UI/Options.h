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
        static float hue[7];
        static float saturation[7];
        static float value[7];
        // Extract HSV information from the profile's color tables
        RGB2HSV(hue[i], saturation[i], value[i], identity->color_table.entries[i]);
        
        Element* h = new Element;
        h->flags = Flags::texture;
        h->data.texture = textureCache.get("./Resources/Textures/Icons/hue.png");
        h->position = position + glm::vec2{0,RenderGame::kPieceDimensions.y+1};
        h->size= {32.0f,32.0f};
        h->moveCallback = [i,identity](const SDL_MouseMotionEvent& event){
            //Initializes a static float for each color table entry to the hue of that entry
            hue[i] = hue[i] + event.yrel*2;

            HSV2RGB(hue[i],saturation[i],value[i], identity->color_table.entries[i]);
        };

        printf("H S V : %d, %f, %f, %f,\n",i,hue[i], saturation[i],value[i]);

        pieceRoot->addChild(h);

        Element* s = new Element;
        s->flags = Flags::texture;
        s->data.texture = textureCache.get("./Resources/Textures/Icons/saturation.png");
        s->position = position + glm::vec2{32.0f,RenderGame::kPieceDimensions.y+1};
        s->size= {32.0f,32.0f};
        s->moveCallback = [i,identity](const SDL_MouseMotionEvent& event){
            //Initializes a static float for each color table entry to the hue of that entry
            saturation[i] = fmax(0,fmin(1,saturation[i] - event.yrel*.025));

            HSV2RGB(hue[i],saturation[i],value[i], identity->color_table.entries[i]);
        };

        pieceRoot->addChild(s);

        Element* v = new Element;
        v->flags = Flags::texture;
        v->data.texture = textureCache.get("./Resources/Textures/Icons/value.png");
        v->position = position + glm::vec2{64.0f,RenderGame::kPieceDimensions.y+1};
        v->size= {32.0f,32.0f};
        v->moveCallback = [i,identity](const SDL_MouseMotionEvent& event){
            //Initializes a static float for each color table entry to the hue of that entry
            value[i] = fmax(0,fmin(1,value[i] - event.yrel*.005));

            HSV2RGB(hue[i],saturation[i],value[i], identity->color_table.entries[i]);
        };

        pieceRoot->addChild(v);
    }


    pieceRoot->position = {list->size.x,0};
    pieceRoot->size = {RenderGame::kPieceDimensions.x * 4,list->size.y};
    pieceRoot->flags = Flags::background | border;

    base->addChild(pieceRoot);


    return base;
}



}


