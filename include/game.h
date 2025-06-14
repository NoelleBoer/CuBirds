#ifndef GAME_H
#define GAME_H

#include "player.h"
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
    /* --- STANDARD GAME LOGIC --- */

    /**
     * @brief Constructor for the game class
     * 
     * - Turn is initalized at 0
     * - The drawpile is initialized
     * - Cards are placed in rows on the table
     * - Players get 8 cards in hand and 1 in collection
     * - The discardpile is added to the drawpile, to start with an empty discardpile
     */
    Game();

    /**
     * @brief Checks and handles when a player has an empty hand
     * 
     * 
     * @return Whether there was an empty hand
     */
    bool checkForEmptyHand();


    /**
     * @brief Ends the game when the deck is empty
     * 
     * When the deck is empty the player with the biggest collection is the winning player
     * 
     * @return The index of the winning player or 0 when there is a tie
     */
    int endGame();

    /**
     * @brief Checks if @player has a complete collection
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
     * @brief Resolves the table after cards are played
     * 
     * Resolves the row on the table and lets the player add the cards
     * to hand that are collected. Ends the game if necessary.
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



    /* --- STANDARD PLAY LOGIC --- */

    /**
     * @brief Plays the game until there is a winner
     * 
     * @returns winning player and the way this player won
     * 
     * This function also keeps track of the time each player takes to play the game and the turn
     * 
     * First int is the index of the winning player or 0 for a tie
     * Second int is 0 for win by empty deck, 1 for win by 7 different species and 2 for win by 2x3 birds
     * 
     */
    std::pair<int, int> play();

    /**
     * @brief Handles the turn of a player
     * 
     * First a player must plays cards
     * Then a player can play a family
     * Then there is checked whether the players' hand is empty
     * When the hand is empty everyones hand is put in discard 
     *      and everybody gets 8 new cards and the current player gets another turn
     *
     * @param player Player whose turn it is
     */
    void playTurn(Player& player);

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
     * Scored based on the variables of the @player
     * 
     * 
     * @param player Current player
     * @param enclosedBirds Birds that are enclosed by a certain move
     * @param test Is the function call a test
     * @param id Id of the card that was played
     * 
     * @return The maximum score that can be obtained by the move
     */
    float scoreCards(Player& player, std::vector<int> enclosedBirds, bool test, int id);



    /* --- MC GAME LOGIC--- */
    /**
     * @brief Plays a turn for a random player
     * 
     * - Chooses randomly (weighted) which birdtype to play
     * - Chooses randomly the row and side to play this type
     * - Chooses randomly whether to play a family
     * 
     * @param player Current player 
     * 
     */
    void playRandomTurn(Player& player);

    /**
     * @brief Plays cards at a specific location on the table
     * 
     * @param player Current player 
     * @param k Index of the birdtype to play
     * @param i Row to play
     * @param j Side to play
     * 
     * 
     */
    void playMCCards(Player& player, int k, int i, int j, int numberBirds);

    /**
     * @brief Plays a specific family
     * 
     * @param player Current player 
     * @param m Index of the birdtype [1,9]
     * 
     */
    void playMCFamily(Player& player, int m);

    /**
     * @brief Plays the game until there is a winner
     * 
     * @returns winning player and the way this player won
     * 
     * This function also keeps track of the time each player takes to play the game and the turn
     * 
     * First int is the index of the winning player or 0 for a tie
     * Second int is 0 for win by empty deck, 1 for win by 7 different species and 2 for win by 2x3 birds
     * 
     */
    std::pair<int, int> playMCGame ();

    /* --- RANDOM GAME LOGIC --- */

    /**
     * @brief Plays the game using random turns until there is a winner
     * 
     * @returns winning player and the way this player won
     * 
     * This function also keeps track of the time each player takes to play the game and the turn
     * 
     * First int is the index of the winning player or 0 for a tie
     * Second int is 0 for win by empty deck, 1 for win by 7 different species and 2 for win by 2x3 birds
     * 
     */
    std::pair<int, int> playRandomGame();
    

    /* --- GET FUNCTIONS --- */

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


    /* --- SET FUNCTIONS --- */
    /**
     * @brief Set the variables for player 1
     * 
     * @param player Player whose variables are set
     * @param k Variable k, see player class
     * @param l Variable l, see player class
     * @param m Variable m, see player class
     * @param n Variable n, see player class
     * @param o Variable o, see player class
     * @param p Variable p, see player class
     */
    void setPlayer1 (float k, float l, float m, float n, float o, float p);

    /**
     * @brief Set the variables for player 2
     * 
     * @param player Player whose variables are set
     * @param k Variable k, see player class
     * @param l Variable l, see player class
     * @param m Variable m, see player class
     * @param n Variable n, see player class
     * @param o Variable o, see player class
     * @param p Variable p, see player class
     */
    void setPlayer2 (float k, float l, float m, float n, float o, float p);

    /**
     * @brief Set a @collection for player 1
     * 
     * @param player Player whose collection is renewed
     * @param newCollection Array of the new collection
     */
    void setCollectionP1 (std::array<int, kindsOfBirds>  newCollection);

    /**
     * @brief Set a @collection for player 2
     * 
     * @param player Player whose collection is renewed
     * @param newCollection Array of the new collection
     */
    void setCollectionP2 (std::array<int, kindsOfBirds>  newCollection);

    /**
     * @brief Set a @hand for player 1
     * 
     * @param player Player whose collection is renewed
     * @param newCollection Array of the new hand
     */
    void setHandP1 (std::array<int, kindsOfBirds>  newHand);

    /**
     * @brief Set a @hand for player 2
     * 
     * @param player Player whose collection is renewed
     * @param newCollection Array of the new hand
     */
    void setHandP2 (std::array<int, kindsOfBirds>  newHand);

private:
    std::vector<Player> players; /**< Vector of players of the game*/
    Table table; /**< Table of the game */
    int turn; /**< Tracker of the turn*/
    std::chrono::duration<double> totalTimeP1 = std::chrono::duration<double>(0.0); /**< Total duration of player 1 turns*/
    std::chrono::duration<double> totalTimeP2 = std::chrono::duration<double>(0.0); /**< Total duration of player 2 turns*/
};

#endif // GAME_H