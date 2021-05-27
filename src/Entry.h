#pragma once

#include <SDL2/SDL.h>



int OnExecute();
bool OnInit();
void OnEvent(SDL_Event& Event);
void OnLoop(int dt);
void OnRender();
void OnCleanup();

void OnResize(int width, int height);
