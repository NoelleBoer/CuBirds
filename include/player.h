#ifndef PLAYER_H
#define PLAYER_H

#include <array>

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
     * @brief Gets the cards in the hand of a player
     * 
     * @return The cards in the hand of a player as an array
     * Every index is a type of card, with the rarest card first
     */
    std::array<int, 8> getHand();

    /**
     * @brief Gets the cards in the collection of a player
     * 
     * @return The cards in the collection of a player as an array
     * Every index is a type of card, with the rarest card first
     */
    std::array<int, 8> getCollection();

    /**
     * @brief Gets the type of a player
     * 
     * Currently the type represents the turn at which a player chooses a goal
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
     * @brief Adds a card to the hand array
     * 
     * @param id Id of the card that has to be added
     */
    void drawCard(int id);

    /**
     * @brief Adds a card to the collection array
     * 
     * @param id Id of the card to be added to the collection
     */
    void collectBird(int id);

    /**
     * @brief Empties the hand
     * 
     * Sets all of the array indexes to zero
     */
    void emptyHand();

    /**
     * @brief Deletes one type from player hand
     * 
     * Sets the index of id to zero
     * 
     * @param id The id of the card to be removed from hand
     */
    void deleteType(int id);

    /**
     * @brief prints the hand array to the terminal
     * 
     */
    void printHand();

    /**
     * @brief prints the collection array to the terminal
     * 
     */
    void printCollection();

    /**
     * @brief Sets a hand with a specific array of cards
     * 
     * @param newHand The array that contains the new cards of the hand
     * 
     */
    void setHand(std::array<int, 8>  newHand);

    /**
     * @brief Get the number of cards in a collection
     * 
     * @return the number of cards that are in the collection
     * 
     */
    int getCollectionSize();

private:
    std::array<int, 8> hand; /**< The cards in the hand of the player. */
    std::array<int, 8> collection;/**< The cards in the collection of the player. */
    int playerType; /**< The type of player. */
    int index; /**< The index of the player. */
    int kindsOfBirds = 8; /**< The amount of unique birds in the game */
};

#endif // PLAYER_H