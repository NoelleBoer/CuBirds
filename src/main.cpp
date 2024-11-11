#include "game.h"
#include <iostream>
#include <bits/stdc++.h>

int main() {
    int nRepeats = 30; // How many games are played
    int arr[3][3] = {0, 0, 0};
    std::pair<int, int> winner;
    std::string rowLabels[3] = {"Empty", "Seven", "2x3  "};
    float numberTurns;
    for (int i = 0; i<nRepeats; i++){
        Game game(3,4);
        winner = game.play();
        numberTurns+=game.getTurn();
        arr[winner.first][winner.second]++;
    }
    std::cout << "Average number of turns: " << std::round(numberTurns/nRepeats) << std::endl;
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