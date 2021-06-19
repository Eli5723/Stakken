#pragma once


#include "InputBuffer.h"
#include "../../Gameplay/Replay.h"

struct ReplayMapper {
    InputBuffer buffer;
    Replay& replay;

    int frame;
    int time;

    ReplayMapper(Replay& replay) : replay(replay) {
        frame = 0;
        time = 0;
        buffer.actions = &replay.actions[0];
        buffer.actionCount = 0;
    } 

    void reset(){
        time = 0;
        frame = 0;
        buffer.actions = &replay.actions[0];
        buffer.actionCount = 0;
    }

    void setReplay(Replay& replay){
        this->replay = replay;
        frame = 0;
        time = 0;
        buffer.actions = &replay.actions[0];
        buffer.actionCount = 0;
    }

    void update(int dt) {

        time += dt;
        // Queue actions until we're caught up with the replay
		while (frame < replay.frameCount && time >= replay.frames[frame].time){
            buffer.actionCount += replay.frames[frame].actionCount;
            frame++;
        }
    }

    void flush(){
        buffer.actions += buffer.actionCount;
        buffer.actionCount = 0;
    }
};