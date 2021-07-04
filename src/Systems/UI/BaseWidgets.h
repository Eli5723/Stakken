#pragma once

#include "./UI.h"
#include "../Assets/Assets.h"
#include "Element.h"

namespace UI {
    Element* Label(const char* text);
    Element* TextInput();
    Element* NumberInput(int &target);
}