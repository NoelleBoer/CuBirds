#include "game.h"
#include <iostream>
#include <iomanip>
#include <array>

/**
 * @brief Test one Game instance nRepeats times
 * 
 * Prints the results in a table to the terminal
 */
void testPlayers(){
    const int nRepeats = 10000; // Number of times the game is played
    std::array<std::array<int, 4>, 3> scores{}; // Keeps the score of each game
    std::pair<int, int> winner; // Variable that has the index of the winner and the way that is won
    const char* rowLabels[] = {"Empty", "Seven", "2x3", "Total"};
    const char* columnLabels[] = {"Tie", "P1", "P2", "Total"};
    float totalTurns = 0; // Keeps track of the total amount of turns of each game
    std::chrono::duration<double> totalTimeP1 = std::chrono::duration<double>(0.0); // Keeps the time for player 1
    std::chrono::duration<double> totalTimeP2 = std::chrono::duration<double>(0.0); // Keeps the time for player 2

    // Run the games and collect results
    for (int i = 0; i < nRepeats; i++) {
        Game game(0,0,1,1,1,1,1);
        winner = game.play();
        totalTurns += game.getTurn();
        totalTimeP1 += game.getTimeP1(); 
        totalTimeP2 += game.getTimeP2();
        scores[winner.first][winner.second]++;
    }

    const int width = 6; // Width for alignment

    // Print column labels
    std::cout << std::setw(8) << " " << " | ";
    for (int j = 0; j < 3; j++) {
        std::cout << std::setw(width) << columnLabels[j];
    }
    std::cout << " | " << std::setw(width) << columnLabels[3] << std::endl;

    // Print a separator line
    std::cout << std::setw(8) << " " << " +";
    for (int j = 0; j < 4; j++) {
        std::cout << std::setw(width) << std::string(width - 1, '-');
    }
    std::cout << std::endl;

    // Print the array with the row labels
    for (int i = 0; i < 3; i++) {
        // Print row label
        std::cout << std::setw(8) << rowLabels[i] << " | ";

        // Print the array elements with alignment
        for (int j = 0; j < 3; j++) {
            std::cout << std::setw(width) << scores[i][j];
        }

        // Print the row total
        int rowSum = scores[i][0] + scores[i][1] + scores[i][2];
        std::cout << " | " << std::setw(width) << rowSum << std::endl;
    }

    // Print a separator line before the totals
    std::cout << std::setw(8) << " " << " +";
    for (int j = 0; j < 4; j++) {
        std::cout << std::setw(width) << std::string(width - 1, '-');
    }
    std::cout << std::endl;

    // Print the column totals
    std::cout << std::setw(8) << rowLabels[3] << " | ";
    for (int j = 0; j < 3; j++) {
        int columnSum = scores[0][j] + scores[1][j] + scores[2][j];
        std::cout << std::setw(width) << columnSum;
    }
    // Print the grand total
    int grandTotal = scores[0][0] + scores[0][1] + scores[0][2] +
                     scores[1][0] + scores[1][1] + scores[1][2] +
                     scores[2][0] + scores[2][1] + scores[2][2];
    std::cout << " | " << std::setw(width) << grandTotal << std::endl;

    // Print average turns per game
    std::cout << "\nAverage number of turns per game: " 
              << std::fixed << std::setprecision(2) 
              << (totalTurns / nRepeats) << std::endl;
    std::cout << "Average time taken by Player 1: "
              << std::fixed << std::setprecision(6)
              << (totalTimeP1 / nRepeats).count() << " seconds" << std::endl;

    std::cout << "Average time taken by Player 2: "
              << std::fixed << std::setprecision(6)
              << (totalTimeP2 / nRepeats).count() << " seconds" << std::endl;
}

int main() {
    testPlayers();
    return 0;
}