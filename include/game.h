#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "table.h"
#include <vector>
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
    Game(float k1, float l1, float m1, float n1, float o1, 
        float k2, float l2, float m2, float n2, float o2);

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

    /**
     * @brief Get the current turn the game is at
     * 
     * @return The current turn the game is at
     */
    int getTurn();

    /**
     * @brief Get the total amount of time for player 1
     * @return The total time player 1 played
     */
    std::chrono::duration<double> getTimeP1();

    /**
     * @brief Get the total amount of time for player 2
     * @return The total time player 2 played
     */
    std::chrono::duration<double> getTimeP2();


private:
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
     * First a player must plays cards
     * Then a player can play a family
     * Then there is checked whether the players' hand is empty
     * When the hand is empty everybodies hand is put in discard and everybody gets 8 new cards
     * The current player gets another turn
     *
     * @param player Player whose turn it is
     */
    void playTurn(Player& player);

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
     * @param id Id of the card whose birdType is the type of cards that were played this turn
     * @param side On which side the cards were played
     * @param numberCards The number of cards that were played of birdtype id
     * 
     * @return Whether the player collected any cards
     */
    bool resolveTable(Player& player, int row, int id, int side, int numberCards);

    /**
     * @brief Playes cards for playerType 0
     * 
     * - chooses a weighted random card from hand and playes this type
     * - 50% chance to draw cards when no cards are collected
     * 
     * @param player Current player 
     * 
     */
    void playRandomCards(Player& player);

    /**
     * @brief Playes a greedy family
     * 
     * First tries to play a big family (rarest bird first)
     * Then tries to play a small family (rarest bird first)
     * 
     * @param player Current player 
     * 
     */
    void playGreedyFamily(Player& player);

    /**
     * @brief Checks which birds would be enclosed in a row
     * 
     * @param row Which row the function should look at
     * @param id Id of the bird that can enclose other birds
     * 
     * @return A vector of the enclosed birds
     */
    std::vector<int> birdsEnclosed(std::vector<int> row, int id);

    /**
     * @brief Plays a family for a collection of seven different birds
     * 
     * Plays a bird that is not yet in the collection
     * 
     * @param player Current player
     */
    void playSeven(Player& player);

    /**
     * @brief Plays a family for a collection of two sets of three birds
     * 
     * First tries to complete a solo set by collecting a big family
     * Then tries to complete a duo set by adding a small family
     * If the risk of the opponent clearing his hand is high, the player also plays a small family
     * If there is only one species of birds in the collection or the current sets cannot be completed 
     *  the player tries to add a new big or small family
     */
    void playTwoThree(Player& player);

    /**
     * @brief Plays cards based on a maximum score
     * 
     * @param player Current player
     * @param test Is the call a test
     * 
     * Test prevents an infinite loop
     * 
     * @return returns the maximum score that can be obtained for a player
     */
    float playScoredCards(Player& player, bool test);

    /**
     * @brief Score cards based on a number of factors
     * If the type = 0 or the type < turn we play greedy
     * 
     * Scored based on:
     * - How much needed for own collection (scaled by k)
     * - How much the player needs the cards on the next turn (scaled by l)
     * - Rarity of the cards (scaled by m)
     * - How much the opponent needs the cards (scaled by n)
     * 
     * The total score = 
     * [0,10]*rarityScore*k*numberOfCollectedCards (How much the cards is needed for own collection)
     * [0,10]*rarityScore*l*numberOfCollectedCards (How much the cards are worth in the future)
     * [0,10]*rarityScore*m*numberOfCollectedCards (How much the cards are scored for the opponent)
     * -10*rarityScore*numberOfCardsSpent (How much the cards we spend are worth)
     * 
     * @param player Current player
     * @param enclosedBirds Birds that are enclosed by a certain move
     * @param test Is the function call a test
     * @param id Id of the card that was played
     * 
     * @return The maximum score that can be obtained by the move
     */
    float scoreCards(Player& player, std::vector<int> enclosedBirds, bool test, int id);

    /**
     * @brief Calculates a greedy score for a vector of cards
     * 
     * @param enclosedBirds Vector of birds to be scored
     * 
     * @return Obtained score (10-28 per card)
     */
    float scoreGreedyCards(std::vector<int> enclosedBirds);

    /**
     * @brief Calculates score for a vector of cards for 2x3 collection
     * 
     * @param enclosedBirds Vector of cards to be scored
     * 
     * Scored based on:
     * - If the player has only 1 species a card is scored based on how many cards of this species are on the board
     * - If the player needs this card to complete a collection the score is 15
     * 
     * @return Obtained score (5-15 per card)
     */
    float scoreTwoThreeCards(Player& player,std::vector<int> enclosedBirds);

    /**
     * @brief Calculates score for a vector of cards for 7 collection
     * 
     * @param EnclosedBirds Vector of cards to be scored
     * 
     * @return Obtained score (10-20 per card)
     */
    float scoreSevenCards(Player& player,std::vector<int> enclosedBirds);

    std::vector<Player> players; /**< Vector of players of the game. */
    Table table; /**< Table of the game. */
    int turn; /**< Which turn are the players on */
    int kindsOfBirds = 8; /**< Kinds of unique birds in the game */
    int amountOfRows = 4; /**< Amount of rows on the table */

    std::chrono::duration<double> totalTimeP1 = std::chrono::duration<double>(0.0); /**< Total duration of player 1 turns*/
    std::chrono::duration<double> totalTimeP2 = std::chrono::duration<double>(0.0); /**< Total duration of player 2 turns*/

    //Card information:
    std::array<int, 8>  smallFam = {2,3,3,4,4,5,6,6}; /**< Amount of birds needed for a small family*/
    std::array<int, 8>  bigFam = {3,4,4,6,6,7,9,9}; /**<  Amount of birds needed for a big family*/
    std::array<int, 8>  numberBirds = {7,10,10,13,13,17,20,20}; /**<  Amount of birds in the game*/
    std::array<float, 8>  rarityScore = {28,20,20,15,15,11,10,10}; /**<  Score based on rarity in the game*/
};

#endif // GAME_H