#include "game.h"
#include <iostream>
#include <iomanip>
#include <array>

int main() {
    const int nRepeats = 1000; // Number of times the game is played
    std::array<std::array<int, 4>, 3> arr{};
    std::pair<int, int> winner;
    const char* rowLabels[] = {"Empty", "Seven", "2x3", "Total"};
    const char* columnLabels[] = {"Tie", "P1", "P2", "Total"};
    float totalTurns = 0.0f;
    std::chrono::duration<double> totalTimeP1 = std::chrono::duration<double>(0.0);
    std::chrono::duration<double> totalTimeP2 = std::chrono::duration<double>(0.0);

    // Run the games and collect results
    for (int i = 0; i < nRepeats; i++) {
        Game game(3,4);
        winner = game.play();
        totalTurns += game.getTurn();
        totalTimeP1 += game.getTimeP1(); 
        totalTimeP2 += game.getTimeP2();
        arr[winner.first][winner.second]++;
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
            std::cout << std::setw(width) << arr[i][j];
        }

        // Print the row total
        int rowSum = arr[i][0] + arr[i][1] + arr[i][2];
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
        int columnSum = arr[0][j] + arr[1][j] + arr[2][j];
        std::cout << std::setw(width) << columnSum;
    }
    // Print the grand total
    int grandTotal = arr[0][0] + arr[0][1] + arr[0][2] +
                     arr[1][0] + arr[1][1] + arr[1][2] +
                     arr[2][0] + arr[2][1] + arr[2][2];
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

    return 0;
}
