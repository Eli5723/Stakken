#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include <estdint.h>

#include "../Rendering/Renderer.h"
#include "../Assets/Texture.h"

namespace UI {

    typedef void (*ClickCallback)(int, int);
    typedef void (*MouseOver)();

    enum Flags {
        none = 0,
        text = 1,
        texture = 1 << 1
    };

    union ElementData {
        const char* text;
    };

    struct Element {
        Element* parent = 0;
        Element* children = 0;
        Element* next = 0;

        glm::vec2 position = {0,0};
        glm::vec2 size = {0,0};

        ClickCallback clickCallback;

        int flags = 0;
        ElementData data;

        void addChild(Element* toAdd){
            toAdd->parent = this;

            if (children == nullptr){
                children = toAdd;
            } else {
                last()->next = toAdd;
            }
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

    // UI driver functions
    void Init(const glm::vec2& resolution);
    void Resize(const glm::vec2& resolution);
    void addToScreen(Element* element);

    // Recursively propogate clicks
    Element* click(Element* root, int x, int y);

    void click(int x, int y);
    // Recursively render widgets
    void Render(Element* root);
    void Render();


    Element* Button(const ClickCallback& callback, const char* text);
}