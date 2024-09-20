#ifndef GAME_H
#define GAME_H

#include <vector>
#include "player.h"
#include "table.h"
#include "card.h"

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
     * @returns The index of the winning player or 0 for a tie
     */
    int play();

private:
    std::vector<Player> players; /**< Vector of players of the game. */
    Table table; /**< Table of the game. */

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
    bool startTurn(Player& player);

    /**
     * @brief Checks if there is a winning player
     * 
     * If there is a player with seven different birds or two sets of three birds in his collection
     * this player wins.
     * 
     * @return Whether this player wins the game with his current collection
     */
    bool checkForWin(Player& player);

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
    bool resolveTable(Player& player, int row, const Card& card);
};

#endif // GAME_H