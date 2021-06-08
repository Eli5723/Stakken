#include "Widgets.h"
#include "../Assets/Assets.h"

namespace UI {

    struct Data {
        Element* screen;
        Element* focus;
    } s_Data;

    // UI driver functions
    void Init(const glm::vec2& resolution){
        s_Data.screen = new Element;
        s_Data.screen->size = resolution;
    }

    void Resize(const glm::vec2& resolution){
        s_Data.screen->size = resolution;
    }

    void addToScreen(Element* element){
        s_Data.screen->addChild(element);
    }

    // Recursively propogate clicks
    Element* click(Element* root, int x, int y){
        Element* node = root->children;
        Element* clicked = 0;
        if (node){
            while (node != nullptr){
                //Propogate clicks to the lowest hit element in the chain
                if (x >= node->position.x && x <= node->position.x + node->size.x && y >= node->position.y && y <= node->position.y + node->size.y) {
                    return click(node,x - node->position.x, y - node->position.y);
                }

                node = node->next;
            }
        } else {
            if (root->clickCallback){
                root->clickCallback(x,y);
                return root;
            }
        }
        return clicked;
    }

    void click(int x, int y){
        Element* clicked = click(s_Data.screen,x,y);
        s_Data.focus = clicked;
    }

    // Recursively render widgets
    void Render(Element* root){
        Element* node = root->children;
        while (node != nullptr){
            Renderer::DrawQuad(root->position + node->position,node->size, {0,0,0,0});
            Renderer::QuadBox(root->position + node->position,node->size,1.0f, {1,1,1,1});

            if (node->flags & Flags::text)
                Renderer::DrawStr(root->position + node->position, 0.5f, node->data.text,activeAssets.font);
            Render(node);
            node = node->next;
        }
    }

    void Render(){
        Render(s_Data.screen);
    }

    Element* Button(const ClickCallback& callback, const char* text){
        Element* button = new Element;
        button->clickCallback = callback;
        button->data.text = text;
        button->flags = Flags::text;
        return button;
    }
}