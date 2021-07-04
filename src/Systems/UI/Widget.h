
#include <SDL2/SDL_events.h>
#include <functional>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include <SDL2/SDL.h>
#include <estdint.h>

struct Widget {
    std::function<void(int,int)> clickCallback;
    std::function<void(const SDL_KeyboardEvent&)> keyCallback;
    std::function<void(const SDL_TextInputEvent&)> textCallback;
    std::function<void(const SDL_MouseMotionEvent&)> dragCallback;

    bool enabled = true;

    Widget* parent = 0;
    Widget* children = 0;
    Widget* next = 0;

    glm::vec2 position = {0,0};
    glm::vec2 size = {0,0};

    glm::vec2 resolveLocation(){
        glm::vec2 pos = position;
        Widget* node = parent;
        while (node){
            pos += node->position;
            node = node->parent;
        }

        return pos;
    }

    void addChild(Widget* toAdd){
        toAdd->parent = this;

        if (children == nullptr){
            children = toAdd;
        } else {
            last()->next = toAdd;
        }
    }

    inline Widget* last(){
        Widget* child = children;
        while (child->next != nullptr){
            child = child->next;
        }

        return child;
    }

    virtual void render(glm::vec2 position){
        
    }

    void RecursiveRender(glm::vec2 baseOffset){
        Widget* child = children;
        render(position + baseOffset);
        
        while (child != nullptr){
            if (child->enabled == false){
                child = child->next;
                continue;
            }
            
            child->RecursiveRender(baseOffset + child->position);
            child = child->next;
        }
    }
};


