#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <array>
#include "table.h"

/**
 * @class Player
 * @brief Represents a player in the CuBirds game.
 *
 * A player has a type and a unique index.
 */

class Player {
public:
    /**
     * @brief Constructs a player with the specified parameters.
     * 
     * This constructor initializes a player with the provided playerType and index.
     * 
     * @param playerType The type of player
     * @param index The index of the player
     * The index should start at 1
     */
    Player(int playerType, int index);

    /**
     * @brief Gets the cards in the hand of a player.
     * 
     * @return The cards in the hand of a player as a vector of the class Card.
     */
    std::array<int, 8> getHand();

    /**
     * @brief Gets the cards in the collection of a player
     * 
     * @return The cards in the collection of a player as a vector of the class Card.
     */
    std::array<int, 8> getCollection();

    /**
     * @brief Gets the type of a player
     * 
     * ---player 0---
     * - chooses a random card from hand and playes this type
     * - chooses first card in a random hand which is able to form a family, tries big family first
     * - 50% chance to draw cards when no cards are collected
     * 
     * ---player 1---
     * - Plays birds to get most cards from the board
     * - Tries to play most valuable family (type with least cards in the game) first
     * - Always draws cards when no cards are collected
     * 
     * @return The type of a player
     */
    int getType();

    /**
     * @brief Gets the index of a player
     * 
     * @return The index of a player
     */
    int getIndex();

    /**
     * @brief Gets the number of cards in the hand of the player
     * 
     * @return Number of cards in hand of player
     */
    int getHandSize();

    /**
     * @brief Adds a card to the hand vector
     * 
     * @param card The card to be added to the hand
     */
    void drawCard(int id);

    /**
     * @brief Adds a card to the collection vector
     * 
     * @param card The card to be added to the collection
     */
    void collectBird(int id);

    /**
     * @brief Empties the vector hand
     */
    void emptyHand();

    /**
     * @brief Deletes all cards that share a birdType with card from hand
     * 
     * @param card The card that decides the birdType that is to be deleted from hand
     */
    void deleteType(int id);

    /**
     * @brief Shuffles the hand
     * 
     * Used to get a random front() card 
     * 
     */
    void shuffleHand();

    /**
     * @brief prints the hand vector to the terminal
     * 
     */
    void printHand();

    /**
     * @brief prints the collection vector to the terminal
     * 
     */
    void printCollection();
    void changeHand(std::array<int, 8>  newHand);
    int getCollectionSize();

private:
    std::array<int, 8> hand; /**< The cards in the hand of the player. */
    std::array<int, 8> collection;/**< The cards in the collection of the player. */
    int playerType; /**< The type of player. */
    int index; /**< The index of the player. */
    
};

#endif // PLAYER_H