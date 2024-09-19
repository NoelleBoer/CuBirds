#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "card.h"
#include "table.h"

/**
 * @class Player
 * @brief Represents a player in the CuBirds game.
 *
 * A player has a name, a type and a unique index.
 */

class Player {
public:
    /**
     * @brief Constructs a player with the specified parameters.
     * 
     * This constructor initializes a player with the provided name,
     * playerType and index.
     * @param name The name of the player
     * @param playerType The type of player
     * @param index The index of the player
     */
    Player(std::string name, int playerType, int index);

    /**
     * @brief Returns the name of the player.
     * 
     * @return The name of a player as a string.
     */
    std::string getName();

    /**
     * @brief Gets the cards in the hand of a player.
     * 
     * @return The cards in the hand of a player as a vector of the class Card.
     */
    std::vector<Card> getHand();

    /**
     * @brief Gets the cards in the collection of a player
     * 
     * @return The cards in the collection of a player as a vector of the class Card.
     */
    std::vector<Card> getCollection();

    /**
     * @brief Gets the type of a player
     * ---player 0---
     * - plays the first card in hand
     * - always plays a family when able
     * - draws 2 cards if no cards are collected
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
     * @brief Adds a card to the hand vector
     * 
     * @param card The card to be added to the hand
     */
    void drawCard(const Card& card);

    /**
     * @brief Adds a card to the collection vector
     * 
     * @param card The card to be added to the collection
     */
    void collectBird(const Card& card);

    /**
     * @brief Empties the vector hand
     */
    void emptyHand();

    /**
     * @brief Deletes all cards that share a birdType with card from hand
     * 
     * @param card The card that decides the birdType that is to be deleted from hand
     */
    void deleteType(const Card& card);

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

private:
    std::string name; /**< The name of the player. */
    std::vector<Card> hand; /**< The cards in the hand of the player. */
    std::vector<Card> collection; /**< The cards in the collection of the player. */
    int playerType; /**< The type of player. */
    int index; /**< The index of the player. */
};

#endif // PLAYER_H