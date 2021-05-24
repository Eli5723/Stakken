#pragma once

#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>

#include <SDL2/SDL.h>

#include "./TetrisInput.h"

namespace fs = std::filesystem;

struct InputProfile {
    std::string name;
    
    // Repeat settings
    int das;
    int arr;
    int dropArr;

    // Piece movement settings
    union {
        int keys[8];
        struct {
            int left;
            int right;
            int softDrop;
            int hardDrop;
            int sonicDrop;
            int rcw;
            int rccw;
            int flip;
        };
    };

    // Default Profile
    InputProfile(){
        name = "Default";
    
        das = 5;
        arr = 5;
        dropArr = 5;

        // Piece Movement settings
        left = SDLK_a;
        right = SDLK_d;
        
        softDrop = SDLK_KP_5;
        hardDrop = SDLK_SPACE;
        sonicDrop = SDLK_LSHIFT;
        
        rcw = SDLK_KP_4;
        rccw = SDLK_KP_6;
        flip = SDLK_KP_8;
    }
    // Load existing profile from file
    InputProfile(const std::string& name){
        this->name = name;
        
        std::string path = "./profiles/" + name + ".pro";

        std::ifstream input(path);
        input >> das;
        input >> arr;
        input >> dropArr;
        input.ignore();

        std::string keyname;
        for (int i=0; i<8; i++) {
            std::getline(input,keyname);
            keys[i] = SDL_GetKeyFromName(keyname.c_str());
        }

    }

    void save() {
        std::string path = "./profiles/" + name + ".pro";
        std::ofstream out(path, std::ios::trunc | std::ios::out);
        write(out);
        SDL_Log("Wrote %s to %s",name.c_str(), path.c_str()); 
    }

    void print(){
        std::cout << "Profile: " << name << '\n';
        write(std::cout);
    }

    void write(std::ostream& out) {
        out << das << '\n';
        out << arr << '\n';
        out << dropArr << '\n';

        for (int i=0; i<8; i++)
            out << SDL_GetKeyName(keys[i]) << '\n';
    }
};