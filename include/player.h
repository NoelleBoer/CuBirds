#ifndef PLAYER_H
#define PLAYER_H

#include "table.h"

/**
 * @class Player
 * @brief Represents a player in the CuBirds game.
 *
 * A player has a type and a unique index.
 */

class Player {
public:
    /* --- STANDARD GAME LOGIC --- */

    /**
     * @brief Constructs a player with the specified parameters.
     * 
     * This constructor initializes a player with the provided index.
     * It also sets the default variables to the optimal values
     * 
     * @param index The index of the player
     * The index should start at 1
     */
    Player(int index);

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
     * @brief Deletes one type from player hand
     * 
     * Sets the index of id to zero
     * 
     * @param id The id of the card to be removed from hand
     */
    void deleteType(int id);

    /**
     * @brief Empties the hand
     * 
     * Sets all of the array indexes to zero
     */
    void emptyHand();

    /*PRINT FUNCTIONS*/

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

    /* --- GET FUNCTIONS --- */
    /**
     * @brief Gets the cards in the hand of a player
     * 
     * @return The cards in the hand of a player as an array
     * Every index is a type of card, with the rarest card first
     */
    std::array<int, kindsOfBirds> getHand();

    /**
     * @brief Gets the cards in the collection of a player
     * 
     * @return The cards in the collection of a player as an array
     * Every index is a type of card, with the rarest card first
     */
    std::array<int, kindsOfBirds> getCollection();

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
     * @brief Get the number of cards in a collection
     * 
     * @return the number of cards that are in the collection
     * 
     */
    int getCollectionSize();

    /**
     * @brief Return K value of this player
     * 
     * k represents the scaler of the own collection
     * 
     * @return k
     * 
     */
    float getK();

    /**
     * @brief Return L value of this player
     * 
     * l represents the scaler for looking 1 turn forward
     * 
     * @return l
     * 
     */
    float getL();

    /**
     * @brief Return M value of this player
     * 
     * m represents the scaler for the rarity of the card
     * 
     * @return m
     * 
     */
    float getM();

    /**
     * @brief Return N value of this player
     * 
     * n represents the scaler of looking at opponent
     * 
     * @return n
     * 
     */
    float getN();

    /**
     * @brief Return O value of this player
     * 
     * o represents the scaler of favoring seven over two sets of three
     * 
     * @return o
     * 
     */
    float getO();

    /**
     * @brief Return P value of this player
     * 
     * p represents the scaler the amount of cards
     * 
     * @return p
     * 
     */
    float getP();

    /*--- SET FUNCTIONS --- */

    /**
     * @brief Sets a hand with a specific array of cards
     * 
     * @param newHand The array that contains the new cards of the hand
     * 
     */
    void setHand(std::array<int, kindsOfBirds>  newHand);

    /**
     * @brief Sets a collection with a specific array of cards
     * 
     * @param newCollection The array that contains the new cards of the hand
     * 
     */
    void setCollection (std::array<int,kindsOfBirds> newCollection);

    /**
     * @brief Sets the variables of a player
     * 
     * @param vark New variable k for player
     * @param varl New variable l for player
     * @param varm New variable m for player
     * @param varn New variable n for player
     * @param varo New variable o for player
     * @param varp New variable p for player
     * 
     */
    void setVariables(float vark, float varl, float varm, float varn, float varo, float varp);

private:
    std::array<int, kindsOfBirds> hand; /**< The cards in the hand of the player. */
    std::array<int, kindsOfBirds> collection;/**< The cards in the collection of the player. */
    int index; /**< The index of the player. */
    float k; /**<  Scaler own collection [0,1]*/
    float l; /**<  Scaler for looking 1 turn forward [0,1]*/
    float m; /**<  Scaler for rarity of the card [-1,1]*/
    float n; /**<  Scaler for looking at opponent [0,1]*/
    float o; /**<  Scaler for favoring seven over two sets of three [0,2]*/
    float p; /**<  Scaler for the amount of cards [0,1]*/
};

#endif // PLAYER_H