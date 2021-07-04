#pragma once

#include "Game.h"
#include "Replay.h"
#include "../Systems/Input/ReplayMapper.h"

struct ReplayViewer {
    int time;

    Replay& replay;
    ReplayMapper& mapper;
    Game& game;

    ReplayViewer(Replay& replay , ReplayMapper& mapper, Game& game) : replay(replay), mapper(mapper), game(game) {
        time = 0;
        game.Reset(replay.seed);
        mapper.reset();
    }

    void reset(){
        time = 0;
        game.Reset(replay.seed);
        mapper.reset();
    }

    void rewind(int dt){
        time = std::max(time - dt,0);

        game.Reset(replay.seed);
        game.state = Game::GameState::Playing;
        mapper.reset();
        
        mapper.update(time);
        game.ApplyInput(mapper.buffer);
        mapper.flush();


        game.UpdateReplay(time);
    }

    void advance(int dt){
        if (game.state == Game::GameState::Playing){
            mapper.update(dt);
            game.ApplyInput(mapper.buffer);
            mapper.flush();
            game.UpdateReplay(dt);

            time += dt;
        }
    }  
};