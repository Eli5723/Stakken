#include "./UI.h"

#include "../Assets/Assets.h"
#include <SDL2/SDL_events.h>

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

    void addToScreen(Element *element){
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

    // Try key capture on focused item
    bool keyCapture(const SDL_KeyboardEvent& event){
        if (s_Data.focus && s_Data.focus->keyCallback) {
            s_Data.focus->keyCallback(event);
            return true;
        }
        return false;
    }

    void inputCapture(const SDL_TextInputEvent& event){
        if (s_Data.focus && s_Data.focus->textCallback)
            s_Data.focus->textCallback(event);
    }

    void clearFocus(){
        s_Data.focus = 0;
    }

    // Recursively render widgets
    void Render(Element* root,glm::vec2 baseOffset){
        Element* node = root->children;
        while (node != nullptr){
            const glm::vec2 offset = baseOffset + node->position;

            if (node->flags & Flags::background)
                Renderer::DrawQuad(offset ,node->size, {0,0,0,1});

            if (node->flags & Flags::border)
                Renderer::QuadBox(offset ,node->size,1.0f, {1,1,1,1});

            if (node->flags & Flags::text)
                Renderer::DrawStr(offset , 0.5f, node->data.text,activeAssets.font);
            
            if (node->flags & Flags::texture)
                Renderer::DrawQuad(offset ,node->size, node->data.texture->id);
            
            if (node->flags & Flags::piece)
                RenderGame::DrawPiece(offset, node->data.piece.colorTable, *activeAssets.pieceTexture, node->data.piece.type, node->data.piece.rotation);

            Render(node,baseOffset + node->position);
            node = node->next;
        }
    }

    void Render(){
        Render(s_Data.screen,{0,0});
    }
}