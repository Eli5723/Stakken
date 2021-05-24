#include <iostream>
#include <stdio.h>

#include "App.h"

int main(int argc, const char** argv){
    Stakken game = Stakken(argc,argv);
    return game.OnExecute();
}