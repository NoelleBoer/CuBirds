#include "game.h"
#include <iostream>

int main() {
    int nRepeats = 60; // How many games are played
    int arr[3][3] = {0, 0, 0};
    std::pair<int, int> winner;
    std::string rowLabels[3] = {"Empty", "Seven", "2x3  "};
    for (int i = 0; i<nRepeats; i++){
        Game game(4,3);
        winner = game.play();
        arr[winner.first][winner.second]++;
    }

    std::cout << "      Tie  P1   P2" << std::endl;
    std::cout << "    -----------------" << std::endl;

    // Print the array with the row labels
    for (int i = 0; i < 3; i++) {
        // Print row label
        std::cout << rowLabels[i] << " | ";
        
        // Print the array elements
        for (int j = 0; j < 3; j++) {
            std::cout << arr[i][j] << "   ";
        }
        
        std::cout << std::endl;
    }
    return 0;
}