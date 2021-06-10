#pragma once

#include "./UI.h"
#include "../Assets/Assets.h"
#include "Element.h"

namespace UI {
    Element* Button(const ClickCallback& callback, Texture* texture);
    Element* Label(const char* text);
}