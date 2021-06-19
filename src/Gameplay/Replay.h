#pragma once
#include "../Systems/TetrisAction.h"
#include "../Systems/Input/InputBuffer.h"

#include "Board.h"

#include <cstdlib>
#include <memory>
#include <vector>
#include <cstdio>
#include <cstring>

using std::vector;

// A frame of a replay
struct ReplayFrame {
    int actionCount;
    int time;
};


struct Recorder {
    vector<TetrisAction> actions;
    vector<ReplayFrame> frames;

    int frameActions = 0;

    int time;
    int seed;

    Recorder() : time(0), seed(0), actions(), frames(){
        actions = vector<TetrisAction>();
        frames = vector<ReplayFrame>();
    }

    void Reset(int seed){
        actions.clear();
        frames.clear();

        this->seed = seed;
        time = 0;
        frameActions = 0;
    }

    // Records an action
    inline void record(TetrisAction action){
        actions.push_back(action);
        frameActions++;
    }

    // Submits a full frame if it contains actions
    void saveFrame(){
        if (frameActions > 0) {
            frames.push_back({frameActions,time});
            frameActions = 0;
        }
    }

    void advance (int dt){
        time+= dt;
    }

    // -- Replay Format --
    // seed, number of frames
    // 
    // frames
    //  action count, time
    //  actions

    void save(char* name) {
        char* path = new char[255];
        strcpy(path,"./Replays/");
        strcat(path,name);
        strcat(path,".rep");

        FILE * file = fopen(path,"wb");

        std::fprintf(file, "%u %u\n", seed, (int)frames.size());
        
        int pos = 0;
        for (int i = 0; i < frames.size(); i++){
            ReplayFrame& frame = frames[i];

            // Print the action count and time of the frame
            std::fprintf(file,"%i %i\n" ,frame.actionCount, frame.time);

                // Print out the actions as characters on  a line
            for (int action = 0; action < frame.actionCount; action++) {
                std::fprintf(file, "%c", (char)actions[pos++] + 'a');
            }
            std::fprintf(file, "\n");
        }

        fclose(file);

        printf("Saved replay to: %s\n", path);
        delete[] path;
    }
};


struct Replay {
    
    vector<TetrisAction> actions;
    vector<ReplayFrame> frames;

    int frameCount;
    int time;
    int seed;

    Replay(char* path) {
        actions = vector<TetrisAction>();
        frames = vector<ReplayFrame>();

        FILE* file = fopen(path, "rb");

        // Read metadata
        fscanf(file, "%u %u\n", &seed, &frameCount);

        int actionCount, time;
        char actionCharacter;
        // Read Frames
        for (int i = 0; i < frameCount; i++){
            fscanf(file, "%i %i\n", &actionCount, &time);
            frames.push_back({actionCount, time});

            for (int action = 0; action < actionCount; action++) {
                fscanf(file, "%c", &actionCharacter);
                actions.push_back((TetrisAction)(actionCharacter - 'a'));
            }
            fscanf(file, "\n");
        }
    }
};