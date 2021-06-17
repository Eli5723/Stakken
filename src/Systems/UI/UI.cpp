#include "./UI.h"

#include "../Assets/Assets.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_stdinc.h>

namespace UI {

    struct Data {
        Element* screen;

        Element* focus = 0;
        Element* drag = 0;
        glm::vec2 dragStartingPosition;

        bool ignoreMovement = 0;
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
    Element* click(Element* current, int x, int y){
        if (!current->enabled)
            return 0;

        Element* child = current->children;
        Element* clicked = 0;
        if (child){
            while (child != nullptr){
                //Propogate clicks to the lowest hit element in the chain
                if ((child->flags & Flags::structural) || (x >= child->position.x && x <= child->position.x + child->size.x && y >= child->position.y && y <= child->position.y + child->size.y)) {
                    return click(child,x - child->position.x, y - child->position.y);
                }

                child = child->next;
            }
        } else {
            if (current->clickCallback){
                current->clickCallback(x,y);
                return current;
            } else if (current->moveCallback){
                // Used to hide the cursor while dragging so that the user can focus
                SDL_SetRelativeMouseMode(SDL_TRUE);
                s_Data.drag = current;
                s_Data.dragStartingPosition = current->resolveLocation() + glm::vec2{x,y};
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

    void moveCapture(const SDL_MouseMotionEvent& event){
        if (s_Data.ignoreMovement){
            s_Data.ignoreMovement = false;
            return;
        }

        if (s_Data.drag && s_Data.drag->moveCallback)
            s_Data.drag->moveCallback(event);
    }

    void SetIgnoreMovement() {
        s_Data.ignoreMovement = true;
    };

    void endDrag(){
        if (s_Data.drag){
            SDL_SetRelativeMouseMode(SDL_FALSE);
            SDL_WarpMouseInWindow(0, s_Data.dragStartingPosition.x, s_Data.dragStartingPosition.y);
        }

        s_Data.drag = 0;
    }

    void clearFocus(){
        s_Data.focus = 0;
    }

    // Recursively render widgets
    void Render(Element* root,glm::vec2 baseOffset){
        Element* node = root->children;
        while (node != nullptr){
            if (node->enabled == false){
                node = node->next;
                continue;
            }

            const glm::vec2 offset = baseOffset + node->position;

            if (node->flags & Flags::background)
                Renderer::DrawQuad(offset ,node->size, {0,0,0,.9f});

            if (node->flags & Flags::border)
                Renderer::QuadBox(offset ,node->size,1.0f, {1,1,1,1});

            if (node->flags & Flags::text)
                Renderer::DrawStr(offset , 0.5f, node->data.text,activeAssets.font);
            
            if (node->flags & Flags::texture)
                Renderer::DrawQuad(offset ,node->size, node->data.texture->id);
            
            if (node->flags & Flags::piece)
                RenderGame::DrawPiece(offset, node->data.piece.colorTable, activeAssets.pieceTexture, node->data.piece.type, node->data.piece.rotation);

            
            
            
            
            Render(node,baseOffset + node->position);
            node = node->next;
        }
    }

    void Render(){
        Render(s_Data.screen,{0,0});
    }
}