#include "game.h"
#include <iostream>
#include <iomanip>
#include <array>
#include <random>
#include <algorithm>

/**
 * @brief Test one Game instance nRepeats times
 * 
 * Prints the results in a table to the terminal
 */
void testPlayers(){
    const int nRepeats = 100; // Number of times the game is played
    std::array<std::array<int, 4>, 3> scores{}; // Keeps the score of each game
    std::pair<int, int> winner; // Variable that has the index of the winner and the way that is won
    const char* rowLabels[] = {"Empty", "Seven", "2x3", "Total"};
    const char* columnLabels[] = {"Tie", "P1", "P2", "Total"};
    float totalTurns = 0; // Keeps track of the total amount of turns of each game
    std::chrono::duration<double> totalTimeP1 = std::chrono::duration<double>(0.0); // Keeps the time for player 1
    std::chrono::duration<double> totalTimeP2 = std::chrono::duration<double>(0.0); // Keeps the time for player 2

    // Run the games and collect results
    for (int i = 0; i < nRepeats; i++) {
        Game game(0,0,0,5,0,1,1,1,1,1); //replace with the player stats you want to test
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

void tuneParameters (){
    const int populationSize = 50; // Number of solutions in the population
    const int generations = 100;  // Number of generations
    const float mutationRate = 0.1; // Mutation rate
    const int nRepeats = 10; // Games per fitness evaluation
    const int numberOpponents = 10;

    // struct to populate the population
    struct Player {
        float k, l, m, n, o; // Player parameters
    };

    //Every player gets a random initialazation with every variable [0,2]
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0, 2.0);
    std::uniform_real_distribution<float> dist1(0.0, 1.0);
    std::uniform_real_distribution<float> dist2(-1.0, 1.0); 
    std::uniform_int_distribution<int> distIndex(0, populationSize - 1);

    std::vector<Player> population(populationSize);
    for (Player &p : population) {
        p.k = dist(gen);
        p.l = dist(gen);
        p.m = dist(gen);
        p.n = dist(gen);
        p.o = dist(gen);
    }

    for (int g = 0; g < generations; g++) {
        std::vector<std::pair<Player, double>> scoredPopulation;
        for (const Player &p : population) {
            double wins = 0;
            for (int opp = 0; opp < numberOpponents; opp++){
                Player p2 = population[distIndex(gen)];
                for (int i = 0; i < nRepeats; i++){
                    Game game(p.k, p.l, p.m, p.n, p.o, p2.k, p2.l, p2.m, p2.n, p2.o);
                    std::pair<int, int> winner = game.play();
                    if (winner.first == 1) wins++;
                }
            }
            float score = wins/(nRepeats*numberOpponents);
            scoredPopulation.emplace_back(p, score);
        }

        // Sort population by fitness in descending order
        std::sort(scoredPopulation.begin(), scoredPopulation.end(),
                  [](const std::pair<Player, double>& a, const std::pair<Player, double>& b) {
                      return a.second > b.second;
                  });


        // Keep the top half as the next generation
        std::vector<Player> nextGeneration;
        for (size_t i = 0; i < populationSize / 2; i++) {
            nextGeneration.push_back(scoredPopulation[i].first);
        }

        //Perform crossover to create offspring
        while (nextGeneration.size() < populationSize) {
            const Player &parent1 = nextGeneration[rand() % (populationSize / 2)];
            const Player &parent2 = nextGeneration[rand() % (populationSize / 2)];

            Player child;
            child.k = (parent1.k + parent2.k) / 2;
            child.l = (parent1.l + parent2.l) / 2;
            child.m = (parent1.m + parent2.m) / 2;
            child.n = (parent1.n + parent2.n) / 2;
            child.o = (parent1.o + parent2.o) / 2;

            //Apply mutation
            if (dist1(gen) < mutationRate) child.k += dist2(gen);
            if (dist1(gen) < mutationRate) child.l += dist2(gen);
            if (dist1(gen) < mutationRate) child.m += dist2(gen);
            if (dist1(gen) < mutationRate) child.n += dist2(gen);
            if (dist1(gen) < mutationRate) child.o += dist2(gen);

            nextGeneration.push_back(child);
        }
        population = nextGeneration;

        // Print the best player's parameters at each generation
        const Player &bestPlayer = scoredPopulation[0].first;
        double bestFitness = scoredPopulation[0].second;
        std::cout << "Generation " << g + 1 << ": Best Fitness = " << bestFitness
                  << ", Parameters = (" << bestPlayer.k << ", " << bestPlayer.l << ", "
                  << bestPlayer.m << ", " << bestPlayer.n << ", " << bestPlayer.o << ")\n";
    }
     // Print the best player overall
    const Player &bestPlayer = population[0];
    std::cout << "\nOptimal Parameters: (" << bestPlayer.k << ", " << bestPlayer.l << ", "
              << bestPlayer.m << ", " << bestPlayer.n << ", " << bestPlayer.o << ")\n";
}

int main() {
    tuneParameters();
    return 0;
}