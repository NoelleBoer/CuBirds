#include "game.h"
#include <iostream>
#include <iomanip>
#include <array>
#include <random>
#include <algorithm>
#include <fstream> 
#include <vector>

/**
 * @brief Test one Game instance nRepeats times
 * 
 * Prints the results in a table to the terminal
 */
void testPlayers(){
    const int nRepeats = 100; // Number of times the game is played
    std::array<std::array<int, 3>, 3> scores{}; // Keeps the score of each game
    std::pair<int, int> winner; // Variable that has the index of the winner and the way that is won
    const char* rowLabels[] = {"Empty", "Seven", "2x3", "Total"};
    const char* columnLabels[] = {"Tie", "P1", "P2", "Total"};
    float totalTurns = 0; // Keeps track of the total amount of turns of each game
    std::chrono::duration<double> totalTimeP1 = std::chrono::duration<double>(0.0); // Keeps the time for player 1
    std::chrono::duration<double> totalTimeP2 = std::chrono::duration<double>(0.0); // Keeps the time for player 2

    // Run the games and collect results
    for (int i = 0; i < nRepeats; i++) {
        Game game;
        game.setPlayer1(1,1,1,1,1,1);
        game.setPlayer2(1,1,1,1,1,1);
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

void tuneParameters() {
    const int populationSize = 400; // Number of solutions in the population (has to be even)
    const int generations = 100;  // Number of generations
    const float initialMutationRate = 0.1f;
    float mutationRate;
    const int nRepeats = 10; // Games per fitness evaluation
    const int numberOpponents = 40;

    struct Player {
        float k, l, m, n, o,p;
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0, 1.0);
    std::uniform_real_distribution<float> distO(0.0, 2.0);
    std::uniform_real_distribution<float> distM(-1.0, 1.0);
    std::uniform_real_distribution<float> mutationDist(-0.05, 0.05);
    std::uniform_int_distribution<int> distIndex(0, populationSize - 1);

    std::vector<Player> population(populationSize);
    for (Player &p : population) {
        p.k = dist(gen);
        p.l = dist(gen);
        p.m = distM(gen);
        p.n = dist(gen);
        p.o = distO(gen);
        p.p = dist(gen);
    }

    std::ofstream outputFile("genetic_algorithm_results.csv");
    outputFile << "Generation,BestFitness,AvgFitness,WorstFitness,Top5_k,Top5_l,Top5_m,Top5_n,Top5_o\n";

    for (int g = 0; g < generations; g++) {
        mutationRate = initialMutationRate * (1.0f - g / float(generations));
        std::vector<std::pair<Player, double>> scoredPopulation;
        for (const Player &p : population) {
            double wins = 0;
            for (int opp = 0; opp < numberOpponents; opp++) {
                Player p2 = population[distIndex(gen)];
                for (int i = 0; i < nRepeats; i++) {
                    Game game;
                    game.setPlayer1(p.k, p.l, p.m, p.n, p.o, p.p);
                    game.setPlayer2(p2.k, p2.l, p2.m, p2.n, p2.o, p2.p);
                    std::pair<int, int> winner = game.play();
                    if (winner.first == 1) wins++;
                }
            }
            double score = wins / (nRepeats * numberOpponents);
            scoredPopulation.emplace_back(p, score);
        }

        std::sort(scoredPopulation.begin(), scoredPopulation.end(),
                  [](const std::pair<Player, double> &a, const std::pair<Player, double> &b) {
                      return a.second > b.second;
                  });

        double bestFitness = scoredPopulation[0].second;
        double worstFitness = scoredPopulation.back().second;
        double avgFitness = 0;
        for (const auto &entry : scoredPopulation) {
            avgFitness += entry.second;
        }
        avgFitness /= scoredPopulation.size();

        outputFile << g + 1 << "," << bestFitness << "," << avgFitness << "," << worstFitness;
        for (size_t i = 0; i < std::min<size_t>(5, scoredPopulation.size()); i++) {
            const Player &p = scoredPopulation[i].first;
            outputFile << "," << p.k << "," << p.l << "," << p.m << "," << p.n << "," << p.o << "," << p.p;
        }
        outputFile << "\n";

        std::cout << "Generation " << g + 1 << ": Best Fitness = " << bestFitness
                  << ", Avg Fitness = " << avgFitness
                  << ", Worst Fitness = " << worstFitness << std::endl;
        for (size_t i = 0; i < std::min<size_t>(5, scoredPopulation.size()); i++) {
            const Player &p = scoredPopulation[i].first;
            std::cout << "  Top " << i + 1 << ": (" << p.k << ", " << p.l << ", "
                      << p.m << ", " << p.n << ", " << p.o << ", " << p.p <<")" << std::endl;
        }

        std::vector<Player> nextGeneration;
        for (size_t i = 0; i < populationSize / 2; i++) {
            nextGeneration.push_back(scoredPopulation[i].first);
        }

        while (nextGeneration.size() < populationSize) {
            const Player &parent1 = nextGeneration[distIndex(gen) % (populationSize / 2)];
            const Player &parent2 = nextGeneration[distIndex(gen) % (populationSize / 2)];

            Player child;
            child.k = std::clamp((parent1.k + parent2.k) / 2 + (dist(gen) < mutationRate ? mutationDist(gen) : 0.0f), 0.0f, 1.0f);
            child.l = std::clamp((parent1.l + parent2.l) / 2 + (dist(gen) < mutationRate ? mutationDist(gen) : 0.0f), 0.0f, 1.0f);
            child.m = std::clamp((parent1.m + parent2.m) / 2 + (dist(gen) < mutationRate ? mutationDist(gen) : 0.0f), -1.0f, 1.0f);
            child.n = std::clamp((parent1.n + parent2.n) / 2 + (dist(gen) < mutationRate ? mutationDist(gen) : 0.0f), 0.0f, 1.0f);
            child.o = std::clamp((parent1.o + parent2.o) / 2 + (dist(gen) < mutationRate ? mutationDist(gen) : 0.0f), 0.0f, 2.0f);
            child.p = std::clamp((parent1.p + parent2.p) / 2 + (dist(gen) < mutationRate ? mutationDist(gen) : 0.0f), 0.0f, 1.0f);

            nextGeneration.push_back(child);
        }
        population = nextGeneration;
    }
    outputFile.close();
}

void testStartingSetups () {
    int nRepeats = 1000;
    std::array<int,8> collectionP1 = {};
    std::array<int,8> collectionP2 = {};
    std::array<std::array<std::array<int, 2>, 8>, 8> winners{};
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            for (int k = 0; k < nRepeats; k++){
                collectionP1.fill(0);
                collectionP2.fill(0); 
                Game game;
                collectionP1[i]++;
                game.setCollectionP1(collectionP1);
                collectionP2[j]++;
                game.setCollectionP2(collectionP2);
                std::pair<int, int> winner = game.play();
                if (winner.first == 1) winners[i][j][0]++;
                else if (winner.first == 2) winners[i][j][1]++;
            }
        }
    }
    // Print the winners array
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::cout << "Starting setup P1[" << i << "] vs P2[" << j << "]: ";
            std::cout << "Player 1 Wins: " << winners[i][j][0] << ", ";
            std::cout << "Player 2 Wins: " << winners[i][j][1] << std::endl;
        }
    }
}

int main() {
    tuneParameters();
    return 0;
}