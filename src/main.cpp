#include "game.h"
#include <iostream>

int main() {
    int nRepeats = 10; // How many games are played
    int arr[3] = {0, 0, 0};
    int winner;
    for (int i = 0; i<nRepeats; i++){
        Game game(0,1);
        winner = game.play();
        arr[winner]++;
    }
    std::cout << "Number of ties: " << arr[0] << std::endl;
    std::cout << "Number of wins for player 1: " << arr[1] << std::endl;
    std::cout << "Number of wins for player 2: " << arr[2] << std::endl;
    return 0;
}