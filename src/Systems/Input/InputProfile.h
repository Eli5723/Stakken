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

    // Initial Piece Rotation
    union {
        int rotation[7];
        struct {
            int T_rotation;
            int O_rotation;
            int L_rotation;
            int J_rotation;
            int Z_rotation;
            int S_rotation;
            int I_rotation;
        };
    };

    // Default Profile
    InputProfile(){
        name = "Default";
    
        das = 110;
        arr = 0;
        dropArr = 100;

        // Piece Movement settings
        left = SDL_SCANCODE_A;
        right = SDL_SCANCODE_D;
        
        softDrop = SDL_SCANCODE_KP_5;
        hardDrop = SDL_SCANCODE_SPACE;
        sonicDrop = SDL_SCANCODE_LSHIFT;
        
        rcw = SDL_SCANCODE_KP_6;
        rccw = SDL_SCANCODE_KP_4;
        flip = SDL_SCANCODE_KP_8;

        for (int i = 0; i < 7; i++)
            rotation[i] = 0;
    }

    // Load existing profile from file
    InputProfile(const std::string& name){
        this->name = name;
        
        std::string path = kProfileDirectory + name + kFileExtension;

        std::ifstream input(path);
        input >> das;
        input >> arr;
        input >> dropArr;
       
        input >>  T_rotation;
        input >>  O_rotation;
        input >>  L_rotation;
        input >>  J_rotation;
        input >>  Z_rotation;
        input >>  S_rotation;
        input >>  I_rotation;

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

        out <<  T_rotation << '\n';
        out <<  O_rotation << '\n';
        out <<  L_rotation << '\n';
        out <<  J_rotation << '\n';
        out <<  Z_rotation << '\n';
        out <<  S_rotation << '\n';
        out <<  I_rotation << '\n';

        for (int i=0; i<8; i++)
            out << SDL_GetKeyName(SDL_GetKeyFromScancode(((SDL_Scancode)keys[i]))) << '\n';
    }
};