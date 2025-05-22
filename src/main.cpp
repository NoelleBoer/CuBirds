#include "game.h"
#include <iomanip>
#include <algorithm>
#include <fstream> 
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>

/**
 * @brief Prints the results of an experiment to the terminal
 * 
 * @param scores Has information about the amount of wins per player and the collection of that player
 * @param totalTurns The amount of turns that were played
 * @param nRepeats Number of games that were played
 * @param totalTimeP1 The total amount of time player 1 spend playing
 * @param totalTimeP2 The total amount of time player 2 spend playing
 * 
 */
void printResultsShell(std::array<std::array<int, 3>, 3> scores, float totalTurns, int nRepeats, 
    std::chrono::duration<double> totalTimeP1, std::chrono::duration<double> totalTimeP2){
    const char* rowLabels[] = {"Empty", "Seven", "2x3", "Total"};
    const char* columnLabels[] = {"Tie", "P1", "P2", "Total"};
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

/**
 * @brief Prints the results of an experiment to the terminal
 * 
 * @param scores Has information about the amount of wins per player and the collection of that player
 * @param totalTurns The amount of turns that were played
 * @param nRepeats Number of games that were played
 * @param totalTimeP1 The total amount of time player 1 spend playing
 * @param totalTimeP2 The total amount of time player 2 spend playing
 * @param filename File where the JSON will be saved
 * 
 */
void printResultsJSONFile(std::array<std::array<int, 3>, 3> scores,
                      float totalTurns,
                      int nRepeats,
                      std::chrono::duration<double> totalTimeP1,
                      std::chrono::duration<double> totalTimeP2,
                      const std::string& filename) {

    namespace fs = std::filesystem;
    using json = nlohmann::json;

    fs::create_directories("results");

    json result;

    const std::array<std::string, 3> rowLabels = {"Empty", "Seven", "2x3"};
    const std::array<std::string, 3> columnLabels = {"Tie", "P1", "P2"};

    int grandTotal = 0;
    json table;

    for (int i = 0; i < 3; ++i) {
        int rowTotal = 0;
        json row;
        for (int j = 0; j < 3; ++j) {
            row[columnLabels[j]] = scores[i][j];
            rowTotal += scores[i][j];
            grandTotal += scores[i][j];
        }
        row["Total"] = rowTotal;
        table[rowLabels[i]] = row;
    }

    json totals;
    for (int j = 0; j < 3; ++j) {
        int columnSum = scores[0][j] + scores[1][j] + scores[2][j];
        totals[columnLabels[j]] = columnSum;
    }
    totals["Total"] = grandTotal;

    result["Scores"] = table;
    result["ColumnTotals"] = totals;
    result["AverageTurnsPerGame"] = totalTurns / nRepeats;
    result["AverageTimePlayer1"] = (totalTimeP1 / nRepeats).count();
    result["AverageTimePlayer2"] = (totalTimeP2 / nRepeats).count();

    // Write to file using the given filename
    std::ofstream outFile("results/" + filename);
    outFile << std::setw(4) << result << std::endl;
}

/**
 * @brief Test one starting setting on standard play @nRepeats times
 * 
 * @param MCGame Determines whether to play in standard mode or in MC mode
 * @param shell Determines whether to print the results to the shell or to a json file
 * @param nRepeats The number of times a game is played
 * 
 */
void testPlayers(bool MCGame, bool shell, int nRepeats){
    std::array<std::array<int, 3>, 3> scores{}; // Keeps the score of each game
    std::pair<int, int> winner; // Variable that has the index of the winner and the way that is won
    float totalTurns = 0; // Keeps track of the total amount of turns of each game
    std::chrono::duration<double> totalTimeP1 = std::chrono::duration<double>(0.0); // Keeps the time for player 1
    std::chrono::duration<double> totalTimeP2 = std::chrono::duration<double>(0.0); // Keeps the time for player 2

    // Run the games and collect results
    for (int i = 0; i < nRepeats; i++) {
        std::cout << "Running Game " << i << " ..." << std::endl;
        Game game;
        // Enter game settings below. For example:
        // game.setPlayer1(0.5,0.5,0.3,0.5,1.8,0.5);
        if (MCGame) winner = game.playMCGame();
        else winner = game.play();

        totalTurns += game.getTurn();
        totalTimeP1 += game.getTimeP1(); 
        totalTimeP2 += game.getTimeP2();
        scores[winner.first][winner.second]++;
    }
    if (shell) printResultsShell(scores, totalTurns, nRepeats, totalTimeP1, totalTimeP2);
    else printResultsJSONFile(scores, totalTurns, nRepeats, totalTimeP1, totalTimeP2, "results_testPlayers.json");
}

/**
 * @brief Uses a genetic Algorithm to find the best parameters for k,l,m,n,o,p
 * 
 * @param shell Determines whether to print the results to the shell or to a json file
 * @param nRepeats The number of times a game is played
 */
void geneticAlgorithm(bool shell, int nRepeats) {
    const int populationSize = 200;
    const int generations = 100;
    const float initialMutationRate = 0.2f;
    const int numberOpponents = 10;

    struct Player {
        float k, l, m, n, o, p;
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    std::uniform_real_distribution<float> distO(0.0f, 2.0f);
    std::uniform_real_distribution<float> distM(-1.0f, 1.0f);
    std::uniform_real_distribution<float> mutationDist(-0.05f, 0.05f);
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

    nlohmann::json outputJson;
    std::ofstream outFile;
    if (!shell) {
        std::filesystem::create_directories("results");
        outFile.open("results/genetic_algorithm.json");
    }

    for (int g = 0; g < generations; g++) {
        std::cout << "Running Generation " << g << " ..." << std::endl;
        float mutationRate = initialMutationRate * (1.0f - g / float(generations));
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
                  [](const auto &a, const auto &b) {
                      return a.second > b.second;
                  });

        const Player &best = scoredPopulation[0].first;
        double bestFitness = scoredPopulation[0].second;

        if (shell) {
            std::cout << "Generation " << g + 1 << ": Best Fitness = " << bestFitness << std::endl;
            std::cout << "  k=" << best.k << ", l=" << best.l
                      << ", m=" << best.m << ", n=" << best.n
                      << ", o=" << best.o << ", p=" << best.p << std::endl;
        } else {
            nlohmann::json genJson;
            genJson["generation"] = g + 1;
            genJson["fitness"] = bestFitness;
            genJson["parameters"] = {
                {"k", best.k}, {"l", best.l}, {"m", best.m},
                {"n", best.n}, {"o", best.o}, {"p", best.p}
            };
            outputJson["generations"].push_back(genJson);
        }

        // Generate new population
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

    if (!shell && outFile.is_open()) {
        outFile << std::setw(4) << outputJson << std::endl;
        outFile.close();
    }
}

/**
 * @brief Counts the wins for specific beginning cards in the collection
 * 
 * @param MCGame Determines whether to play in standard mode or in MC mode
 * @param shell Determines whether to print the results to the shell or to a json file
 * @param nRepeats The number of times a game is played
 */
void testStartingSetups(bool MCGame, bool shell, int nRepeats) {
    std::array<int, kindsOfBirds> collectionP1{};
    std::array<int, kindsOfBirds> collectionP2{};
    std::array<std::array<std::array<int, 2>, kindsOfBirds>, kindsOfBirds> winners{}; // [i][j][0] = P1 wins, [i][j][1] = P2 wins

    for (int i = 0; i < kindsOfBirds; i++) {
        for (int j = 0; j < kindsOfBirds; j++) {
            std::cout << "Running Game for player 1:" << i << " and player 2:"<< j << " ..." << std::endl;
            for (int k = 0; k < nRepeats; k++) {
                collectionP1.fill(0);
                collectionP2.fill(0);
                collectionP1[i]++;
                collectionP2[j]++;

                Game game;
                game.setCollectionP1(collectionP1);
                game.setCollectionP2(collectionP2);

                std::pair<int, int> winner = MCGame ? game.playMCGame() : game.play();

                if (winner.first == 1) winners[i][j][0]++;
                else if (winner.first == 2) winners[i][j][1]++;
            }
        }
    }

    if (shell) {
        std::cout << "Starting Setups Test (" << (MCGame ? "MCGame" : "Standard") << "):\n";
        for (int i = 0; i < kindsOfBirds; i++) {
            for (int j = 0; j < kindsOfBirds; j++) {
                std::cout << "P1[" << i << "] vs P2[" << j << "]: "
                          << "P1 wins = " << winners[i][j][0]
                          << ", P2 wins = " << winners[i][j][1] << '\n';
            }
        }
    } else {
        // Build JSON structure
        nlohmann::json jsonOutput;
        for (int i = 0; i < kindsOfBirds; i++) {
            for (int j = 0; j < kindsOfBirds; j++) {
                jsonOutput["results"][std::to_string(i)][std::to_string(j)] = {
                    {"P1_wins", winners[i][j][0]},
                    {"P2_wins", winners[i][j][1]}
                };
            }
        }

        std::filesystem::create_directories("results");
        std::string filename = MCGame ? "results/starting_setups_mc.json"
                                      : "results/starting_setups_standard.json";
        std::ofstream outFile(filename);
        outFile << std::setw(4) << jsonOutput << std::endl;
        std::cout << "Results saved to " << filename << std::endl;
    }
}

/**
 * @brief Uses brute monte carlo simulation to determine turns and play the game
 * 
 */
void monteCarlo(bool shell, int nRepeats) {
    std::array<std::array<int, 3>, 3> scores{}; // Keeps the score of each game
    std::pair<int, int> winner; // Variable that has the index of the winner and the way that is won
    float totalTurns = 0; // Keeps track of the total amount of turns of each game
    std::chrono::duration<double> totalTimeP1 = std::chrono::duration<double>(0.0); // Keeps the time for player 1
    std::chrono::duration<double> totalTimeP2 = std::chrono::duration<double>(0.0); // Keeps the time for player 2

    // Run the games and collect results
    for (int i = 0; i < nRepeats; i++) {
        std::cout << "Running Game " << i << " ..." << std::endl;
        Game game;
        winner = game.playMCGame();
        totalTurns += game.getTurn();
        totalTimeP1 += game.getTimeP1(); 
        totalTimeP2 += game.getTimeP2();
        scores[winner.first][winner.second]++;
    }
    if (shell) printResultsShell(scores, totalTurns, nRepeats, totalTimeP1, totalTimeP2);
    else printResultsJSONFile(scores, totalTurns, nRepeats, totalTimeP1, totalTimeP2,"results_mc.json");
}

int main() {
    bool MCGame = true;
    bool shell = false;
    int nRepeats = 1000;

    std::cout << "Settings: " << std::endl;
    if (MCGame) std::cout << "Monte Carlo: On" << std::endl;
    else std::cout << "Monte Carlo: Off" << std::endl;
    if (shell) std::cout << "Results are printed to the shell" << std::endl;
    else std::cout << "Results are stored in JSON files in /results" << std::endl;
    std::cout << "For every test " << nRepeats << " games are ran" << std::endl << std::endl;

    std::cout << "Started Testing Players" << std::endl;
    testPlayers(MCGame, shell, nRepeats);
    std::cout << "Finished Testing Players" << std::endl;

    std::cout << "Started Testing Starting Setups" << std::endl;
    testStartingSetups(MCGame, shell, nRepeats);
    std::cout << "Finished Testing Starting Setups" << std::endl;

    std::cout << "Started Testing Genetic Algorithm" << std::endl;
    geneticAlgorithm(shell, nRepeats);
    std::cout << "Finished Testing Genetic Algorithm" << std::endl;

    std::cout << "Started Testing Monte Carlo" << std::endl;
    monteCarlo(shell, nRepeats);
    std::cout << "Finished Testing Monte Carlo" << std::endl;
    return 0;
}