#pragma once

#include <SDL2/SDL_events.h>
#include <functional>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include <SDL2/SDL.h>
#include <estdint.h>

#include "../Rendering/Renderer.h"
#include "../Assets/Texture.h"
#include "../../Gameplay/RenderGame.h"

namespace UI {
    typedef void (*ClickCallback)(int, int);
    typedef void (*MouseOver)();

    enum Flags {
        none = 0,
        border = 1,
        background = 1 << 1,
        text = 1 << 2,
        texture = 1 << 3,
        piece = 1 << 4
    };

    union ElementData {
        char* text;
        Texture* texture;
        struct Piece {
            TileType type;
            int rotation;
            ColorTable* colorTable;
        } piece;
    };

    struct Element {
        Element* parent = 0;
        Element* children = 0;
        Element* next = 0;

        glm::vec2 position = {0,0};
        glm::vec2 size = {0,0};

        //ClickCallback clickCallback = nullptr;

        std::function<void(int,int)> clickCallback;
        std::function<void(const SDL_KeyboardEvent&)> keyCallback;
        std::function<void(const SDL_TextInputEvent&)> textCallback;
        std::function<void(const SDL_MouseMotionEvent&)> moveCallback;

        int flags = 0;
        ElementData data{0};

        void addChild(Element* toAdd){
            toAdd->parent = this;

            if (children == nullptr){
                children = toAdd;
            } else {
                last()->next = toAdd;
            }
        }

        // Data assignment functions
        void setText(const char* source){
            flags = Flags::text;
            if (data.text)
                delete [] data.text;

            int len  = std::strlen(source);
            data.text = new char[len+1];
            std::strncpy(data.text, source, len);
            data.text[len] = 0;
        }

        // Precondition: 1 node
        inline Element* last(){
            Element* child = children;
            while (child->next != nullptr){
                child = child->next;
            }

            return child;
        }
    };
}