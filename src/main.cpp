#include "game.h"
#include <iostream>

int main() {
    int nRepeats = 1000;
    int arr[2] = {0, 0};
    int winner;
    for (int i = 0; i<nRepeats; i++){
        Game game(0,0);
        winner = game.play();
        arr[winner]++;
    }
    std::cout << "Array elements: " << arr[0] << ", " << arr[1] << std::endl;
    return 0;
}