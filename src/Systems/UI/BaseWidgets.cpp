#include "./BaseWidgets.h"

namespace UI {

    Element* Button(const ClickCallback& callback, Texture* texture){
        Element* button = new Element;
        button->clickCallback = callback;
        button->data.texture = texture;
        button->flags = Flags::texture;
        return button;
    }

    Element* Label(const char* text){
        Element* label = new Element;
        label->setText(text);
        return label;
    }

}