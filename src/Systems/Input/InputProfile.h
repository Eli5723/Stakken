#pragma once

#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>

#include <SDL2/SDL.h>

namespace fs = std::filesystem;

struct InputProfile {
    const std::string kProfileDirectory = "./Profiles/";
    const std::string kFileExtension = ".pro";
    
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
    
        das = 1000;
        arr = 500;
        dropArr = 200;

        // Piece Movement settings
        left = SDL_SCANCODE_A;
        right = SDL_SCANCODE_D;
        
        softDrop = SDL_SCANCODE_KP_5;
        hardDrop = SDL_SCANCODE_SPACE;
        sonicDrop = SDL_SCANCODE_LSHIFT;
        
        rcw = SDL_SCANCODE_KP_4;
        rccw = SDL_SCANCODE_KP_6;
        flip = SDL_SCANCODE_KP_8;
    }
    // Load existing profile from file
    InputProfile(const std::string& name){
        this->name = name;
        
        std::string path = kProfileDirectory + name + kFileExtension;

        std::ifstream input(path);
        input >> das;
        input >> arr;
        input >> dropArr;
        input.ignore();

        std::string keyname;
        for (int i=0; i<8; i++) {
            std::getline(input,keyname);
            keys[i] = SDL_GetScancodeFromName(keyname.c_str());
        }

        SDL_Log("Loaded profile \"%s\" from \"%s\"",name.c_str(), path.c_str()); 
    }

    void save() {
        std::string path = kProfileDirectory + name + kFileExtension;
        std::ofstream out(path, std::ios::trunc | std::ios::out);
        write(out);
        SDL_Log("Wrote profile \"%s\" to \"%s\"",name.c_str(), path.c_str()); 
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
            out << SDL_GetKeyName(SDL_GetKeyFromScancode(((SDL_Scancode)keys[i]))) << '\n';
    }
};