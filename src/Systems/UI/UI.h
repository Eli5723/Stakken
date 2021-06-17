#pragma once

#include "./Element.h"
#include <SDL2/SDL_events.h>

namespace UI {

    // UI driver functions
    void Init(const glm::vec2& resolution);
    void Resize(const glm::vec2& resolution);
    void addToScreen(Element* element);


    // Recursively propogate clicks
    Element* click(Element* root, int x, int y);
    void click(int x, int y);
    bool keyCapture(const SDL_KeyboardEvent& event);
    void inputCapture(const SDL_TextInputEvent& event);
    void clearFocus();

    void moveCapture(const SDL_MouseMotionEvent& event);
    void SetIgnoreMovement();
    void endDrag();

    // Recursively render widgets
    void Render(Element* root);

    void Render();
}