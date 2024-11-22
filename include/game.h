#ifndef GAME_H
#define GAME_H

#include <vector>
#include "player.h"
#include "table.h"
#include <array>
#include <chrono>


/**
 * @class Game
 * @brief Represents one game of CuBirds
 *
 * The Game class regulates the game of CuBirds by letting players take turns,
 * ending the game if necesarry and deciding a winner of the game.
 */

class Game {
public:
    /**
     * @brief Constructor for the game class
     * 
     * In this constructer the players are made with a specified type and index
     */
    Game(int type1, int type2);

    /**
     * @brief Plays the game untill there is a winner
     * 
     * @returns winning player and the way this player won
     * 
     * First int is the index of the winning player or 0 for a tie
     * Second int is 0 for win by empty deck, 1 for win by 7 different species and 2 for win by 2x3 birds
     * 
     */
    std::pair<int, int> play();
    int getTurn();
    std::chrono::duration<double> getTimeP1();
    std::chrono::duration<double> getTimeP2();


private:
    std::vector<Player> players; /**< Vector of players of the game. */
    Table table; /**< Table of the game. */
    int turn = 0; /**< Which turn are the players on */
    std::chrono::duration<double> totalTimeP1 = std::chrono::duration<double>(0.0);
    std::chrono::duration<double> totalTimeP2 = std::chrono::duration<double>(0.0);

    //Card information:
    std::array<int, 8>  smallFam = {2,3,3,4,4,5,6,6};
    std::array<int, 8>  bigFam = {3,4,4,6,6,7,9,9};
    std::array<int, 8>  numberBirds = {7,10,10,13,13,17,20,20};


    /**
     * @brief Generates the beginning conditions of a game
     * 
     * Fills all four rows with three different cards
     * Gives all players eight cards
     * Gives all players a starting card for their collection
     */
    void startGame();

    /**
     * @brief Ends the game when the deck is empty
     * 
     * When the deck is empty the player with the biggest collection is the winning player
     * 
     * @return The index of the winning player or 0 when there is a tie
     */
    int endGame();

    /**
     * @brief Handles the turn of a player
     * 
     * First a player plays cards
     * Then a player can play a family
     * Then there is checked whether the players' hand is empty
     * When the hand is empty everybodies hand is put in discard and everybody gets 8 new cards
     *
     * @param player Player whose turn it is
     * 
     * @return Whether the deck is empty
     * It returns false when a player empties his hand and 
     * there are not enough cards to deal every player 8 new cards.
     * Otherwise it returns true.
     */
    void startTurn(Player& player);

    /**
     * @brief Checks if there is a winning player
     * 
     * If there is a player with seven different birds or two sets of three birds in his collection
     * this player wins.
     * 
     * @return whether and how a player wins
     * 
     * return 0 if the player did not win
     * return 1 if the player won by having 7 different species
     * return 2 if the player won by having 2 sets of at least 3 birds
     */
    int checkForWin(Player& player);

    /**
     * @brief Plays cards on the board
     * 
     * This function makes sure that a player plays cards according to the 
     * strategy of this type of player.
     * 
     * @param player Player whose turn it is
     */
    void playCards(Player& player);

    /**
     * @brief Plays a family
     * 
     * This function makes sure that a player plays a family according to the
     * strategy of this type of player. The cards are discarded from hand and
     * the right number of cards are added to the collection of the player.
     * 
     * @param player Player whose turn it is
     */
    void playFamily(Player& player);

    /**
     * @brief Resolves the table after cards are played
     * 
     * Resolves the row on the table and lets the player add the cards
     * to hand that are to be collected. Ends the game if necessary.
     * 
     * @param player Player whose turn it is
     * @param row Row where cards were played
     * @param card Card whose birdType is the type of cards that were played this turn
     * 
     * @return Whether the player collected any cards
     */
    bool resolveTable(Player& player, int row, int id, int side, int numberCards);

    /**
     * @brief Playes cards for playerType 0
     * 
     * - chooses a random card from hand and playes this type
     * - 50% chance to draw cards when no cards are collected
     * 
     * @param player Current player 
     * 
     */
    void playRandomCards(Player& player);

    /**
     * @brief Playes family for playerType 0
     * 
     * - always plays a family when able (the one he finds first in hand)
     * 
     * @param player Current player 
     * 
     */
    void playRandomFamily(Player& player);

    /**
     * @brief Playes cards for playerType 1
     * 
     * - chooses a random card from hand and playes this type
     * - 50% chance to draw cards when no cards are collected
     * 
     * @param player Current player 
     * 
     */
    void playGreedyCards(Player& player);

    /**
     * @brief Playes family for playerType 1
     * 
     * - always plays a family when able (the one he finds first in hand)
     * 
     * @param player Current player 
     * 
     */
    void playGreedyFamily(Player& player);

    /**
     * @brief Checks how many birds are enclosed on a row
     * Aanvullen
     */
    std::vector<int> birdsEnclosed(std::vector<int> row, int id);
    void playGreedyBigFamily(Player& player);
    void playSeven(Player& player);
    void playTwoThree(Player& player);
    void increaseTurn();
    float playScoredCards(Player& player, bool test);
    float scoreGreedyCards(std::vector<int> enclosedBirds);
    float scoreCards(Player& player, std::vector<int> enclosedBirds, bool test);
    float scoreTwoThreeCards(Player& player,std::vector<int> enclosedBirds, bool test);
    float scoreSevenCards(Player& player,std::vector<int> enclosedBirds, bool test);
    
};

#endif // GAME_H