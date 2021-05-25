#pragma once

#include "../TetrisAction.h"

// Inputs to be consumed by games
struct InputBuffer {
    TetrisAction* actions;
    int actionCount = 0;

    InputBuffer(int size){
        actions = new TetrisAction[size];
    }
    ~InputBuffer(){
        delete [] actions;
    }

    inline void queue(TetrisAction action) {
		actions[actionCount++] = action;
	}

	virtual void flush() {
		actionCount = 0;
	}
};