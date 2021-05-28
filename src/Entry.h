#pragma once

#include <SDL2/SDL.h>



int OnExecute();
bool OnInit();
void OnLoop(int dt);
void OnRender();
void OnCleanup();

// Event Handling
void OnEvent(SDL_Event& Event);

void OnInput(SDL_Event& event);

void OnResize(int width, int height);