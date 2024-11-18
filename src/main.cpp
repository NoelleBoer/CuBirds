#include "game.h"
#include <iostream>
#include <iomanip>
#include <array>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void testTurns(){
    int nRepeats = 5000;
    int numberP1 = 0;
    int numberP2 = 0;
    int numberTies = 0;
    std::pair<int, int> winner;
    // Define the JSON structure
    json stackedBarData;
    
    // Add categories
    stackedBarData["categories"] = { "Player 1 wins (2x3)", "Player 2 wins (7)", "Tie" };

    // Add series data
    for (int i = 100; i < 250; i+=2){
        numberP1 = 0;
        numberP2 = 0;
        numberTies = 0;
        for (int j = 0; j < nRepeats; j++) {
            Game game(i,i+1);
            winner = game.play();
            if (winner.second == 0) numberTies++;
            else if (winner.second == 1) numberP1++;
            else if (winner.second == 2) numberP2++;
        }
        for (int j = 0; j < nRepeats; j++) {
            Game game(i+1,i);
            winner = game.play();
            if (winner.second == 0) numberTies++;
            else if (winner.second == 1) numberP2++;
            else if (winner.second == 2) numberP1++;
        }
        json series;
        series["name"] = "Turn " + std::to_string((i-100)/2);
        series["data"] = {numberP1, numberP2, numberTies};
        stackedBarData["series"].push_back(series);
    }

    // Write JSON to a file
    std::ofstream file("turn_data.json");
    if (file.is_open()) {
        file << stackedBarData.dump(4); // Pretty-print with 4 spaces of indentation
        file.close();
        std::cout << "JSON file created: turn_data.json" << std::endl;
    } else {
        std::cerr << "Error: Unable to open file for writing" << std::endl;
    }
}

void testPlayerTwoThree() {
    int nRepeats = 50;
    int numberP1 = 0;
    int numberP2 = 0;
    int numberTies = 0;
    std::pair<int, int> winner;
    // Define the JSON structure
    json stackedBarData;
    
    // Add categories
    stackedBarData["categories"] = { "Player 1 wins (2x3)", "Player 2 wins (random)", "Tie" };

    // Add series data
    for (int i = 100; i < 250; i+=2){
        numberP1 = 0;
        numberP2 = 0;
        numberTies = 0;
        for (int j = 0; j < nRepeats; j++) {
            Game game(i,0);
            winner = game.play();
            if (winner.second == 0) numberTies++;
            else if (winner.second == 1) numberP1++;
            else if (winner.second == 2) numberP2++;
        }
        for (int j = 0; j < nRepeats; j++) {
            Game game(0,i);
            winner = game.play();
            if (winner.second == 0) numberTies++;
            else if (winner.second == 1) numberP2++;
            else if (winner.second == 2) numberP1++;
        }
        std::cout << "Wins: " << numberTies << " " << numberP1 << " " << numberP2 << std::endl;
        json series;
        series["data"]["0"] = numberP1;
        series["data"]["1"] = numberP2;
        series["data"]["2"] = numberTies;

        stackedBarData["series"].push_back(series);
        series["name"] = "Turn " + std::to_string((i-100)/2);
        stackedBarData["series"].push_back(series);
    }

    // Write JSON to a file
    std::ofstream file("two_three_data.json");
    if (file.is_open()) {
        file << stackedBarData.dump(4); // Pretty-print with 4 spaces of indentation
        file.close();
        std::cout << "JSON file created: two_three_data.json" << std::endl;
    } else {
        std::cerr << "Error: Unable to open file for writing" << std::endl;
    }
}

void testPlayerSeven() {
    int nRepeats = 50;
    int numberP1 = 0;
    int numberP2 = 0;
    int numberTies = 0;
    std::pair<int, int> winner;
    // Define the JSON structure
    json stackedBarData;
    
    // Add categories
    stackedBarData["categories"] = { "Player 1 wins (seven)", "Player 2 wins (greedy)", "Tie" };

    // Add series data
    for (int i = 100; i < 250; i+=2){
        numberP1 = 0;
        numberP2 = 0;
        numberTies = 0;
        for (int j = 0; j < nRepeats; j++) {
            Game game(i+1,0);
            winner = game.play();
            if (winner.second == 0) numberTies++;
            else if (winner.second == 1) numberP1++;
            else if (winner.second == 2) numberP2++;
        }
        for (int j = 0; j < nRepeats; j++) {
            Game game(0,i+1);
            winner = game.play();
            if (winner.second == 0) numberTies++;
            else if (winner.second == 1) numberP2++;
            else if (winner.second == 2) numberP1++;
        }
        json series;
        std::cout << "Wins: " << numberTies << " " << numberP1 << " " << numberP2 << std::endl;
        series["data"]["0"] = numberP1;
        series["data"]["1"] = numberP2;
        series["data"]["2"] = numberTies;
        series["name"] = "Turn " + std::to_string((i-100)/2);
        stackedBarData["series"].push_back(series);
    }

    // Write JSON to a file

    std::ofstream file("seven_data.json");
    if (file.is_open()) {
        file << stackedBarData.dump(4); // Pretty-print with 4 spaces of indentation
        file.close();
        std::cout << "JSON file created: seven_data.json" << std::endl;
    } else {
        std::cerr << "Error: Unable to open file for writing" << std::endl;
    }
}

void testPlayers(){
    const int nRepeats = 1; // Number of times the game is played
    std::array<std::array<int, 4>, 3> arr{};
    std::pair<int, int> winner;
    const char* rowLabels[] = {"Empty", "Seven", "2x3", "Total"};
    const char* columnLabels[] = {"Tie", "P1", "P2", "Total"};
    float totalTurns = 0.0f;
    std::chrono::duration<double> totalTimeP1 = std::chrono::duration<double>(0.0);
    std::chrono::duration<double> totalTimeP2 = std::chrono::duration<double>(0.0);

    // Run the games and collect results
    for (int i = 0; i < nRepeats; i++) {
        Game game(101,0);
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
}

int main() {
    //testTurns();
    testPlayerSeven();
    //testPlayerTwoThree();
    //testPlayers();
    return 0;
}