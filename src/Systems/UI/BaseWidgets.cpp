#include "./BaseWidgets.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_stdinc.h>
#include <cstdio>

#include <cstdlib>


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


    Element* TextInput(){
        Element* textBox = new Element;
        textBox->flags = Flags::border | Flags::text | Flags::background;
        textBox->clickCallback = [](int x,int y){ printf("starting text input\n");SDL_StartTextInput();};
        const int limit = 40;
        textBox->data.text = new char[limit+1];
        std::memset(textBox->data.text,0,limit+1);

        static int cursor = 0;

        textBox->keyCallback = [textBox](const SDL_KeyboardEvent& key){
            if (key.keysym.sym == SDLK_BACKSPACE && cursor > 0){
                std::strncpy(&textBox->data.text[cursor-1],&textBox->data.text[cursor],limit - cursor + 1);
                cursor--;
                return;
            }
        };

        textBox->textCallback = [textBox](const SDL_TextInputEvent& key){
            if (cursor < limit)
                textBox->data.text[cursor++] = key.text[0];
            return;
        };

        return textBox;
    }

    Element* NumberInput(int &target){
        Element* textBox = new Element;
        textBox->flags = Flags::border | Flags::text | Flags::background;
        textBox->clickCallback = [](int x,int y){SDL_StartTextInput();};
        const int limit = 40;
        textBox->data.text = new char[limit+1];
        std::memset(textBox->data.text,0,limit+1);
        sprintf(textBox->data.text,"%d",target);
        static int cursor = strlen(textBox->data.text);

        textBox->keyCallback = [textBox,&target](const SDL_KeyboardEvent& key){
            if (key.keysym.sym == SDLK_BACKSPACE && cursor > 0){
                std::strncpy(&textBox->data.text[cursor-1],&textBox->data.text[cursor],limit - cursor + 1);
                cursor--;
                return;
            } else if (key.keysym.sym == SDLK_RETURN) {
                int value;
                sscanf(textBox->data.text, "%d", &value);
                target = value;
                printf("Value: %i\n", value);
            }
        };

        textBox->textCallback = [textBox](const SDL_TextInputEvent& key){
            if (cursor < limit && key.text[0] >= '0' && key.text[0] <= '9')
                textBox->data.text[cursor++] = key.text[0];
            return;
        };

        return textBox;
    }
}