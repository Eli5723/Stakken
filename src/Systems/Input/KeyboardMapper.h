#include "./InputProfile.h"
#include "./InputBuffer.h"

class KeyboardMapper {
    public:
    InputProfile& profile;
    InputBuffer buffer;

    TetrisAction currentAction = TetrisAction::None;
    Uint8 repeatKey;

    int timeToRepeat;

    bool SoftDropping;

    KeyboardMapper(InputProfile& inputProfile) : profile(inputProfile), buffer(20) {
        currentAction = TetrisAction::None;
        SoftDropping = false;
    }


    // Handle key presses, instantaenous actions
    void keyEvents(SDL_Scancode input){
        if (input==profile.rcw) { 
            buffer.queue(TetrisAction::RCW);
            return;
        }

        if (input==profile.rccw) {
            buffer.queue(TetrisAction::RCCW);
            return;
        }

        if (input==profile.flip) {
            buffer.queue(TetrisAction::Flip);
            return;
        }

        if (input==profile.hardDrop) {
            buffer.queue(TetrisAction::HardDrop);
            return;
        }
        
        if (input==profile.sonicDrop) {
            buffer.queue(TetrisAction::SonicDrop);
            return;
        }

        // Repeatable actions, set the repeatable values
        if (input==profile.left) {
            repeatKey = profile.left;
            buffer.queue(TetrisAction::Left);
            currentAction = TetrisAction::Left;

            timeToRepeat = profile.das;
            return;
        }
        
        if (input==profile.right) {
            repeatKey = profile.right;
            buffer.queue(TetrisAction::Right);
            currentAction = TetrisAction::Right;

            timeToRepeat = profile.das;
            return;
        }

        if (input==profile.softDrop) {
            repeatKey = profile.softDrop;
            buffer.queue(TetrisAction::Right);
            currentAction = TetrisAction::Right;

            timeToRepeat = profile.das;
            return;
        }
    }

    // Poll Keyboard to check for repeats
    void update(int timeElapsed){
        const Uint8* keys = SDL_GetKeyboardState(NULL);

        if (currentAction != TetrisAction::None && keys[repeatKey]) {
            timeToRepeat-=timeElapsed;
            int maxActions = 9;

            // Handle Soft Drop auto repeat
            if (currentAction == TetrisAction::SoftDrop){
                timeToRepeat += profile.dropArr;
                buffer.queue(currentAction);
                return;
            }
            // Handle Side auto repeat
            while (timeToRepeat < 0 && maxActions--){
                timeToRepeat += profile.arr;
                buffer.queue(currentAction);
            }
        }
    }
};