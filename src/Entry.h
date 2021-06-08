#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>



int OnExecute();
bool OnInit();
void OnLoop(int dt);
void OnRender();
void OnCleanup();

// Event Handling
void OnEvent(const SDL_Event& Event);

void OnInput(const SDL_Event& event);

void OnResize(int width, int height);

void OnClick(const SDL_Event& event);