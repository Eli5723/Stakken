#pragma once

#include "./Element.h"

namespace UI {

    // UI driver functions
    void Init(const glm::vec2& resolution);
    void Resize(const glm::vec2& resolution);
    void addToScreen(Element* element);


    // Recursively propogate clicks
    Element* click(Element* root, int x, int y);
    void click(int x, int y);
    bool keyCapture(const SDL_KeyboardEvent& event);
    void clearFocus();

    // Recursively render widgets
    void Render(Element* root);

    void Render();
}