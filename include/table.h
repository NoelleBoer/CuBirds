#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <string>
#include <array>
#include <random>
#include <iostream>

/* --- CONSTANTS USED IN CUBIRDS --- */
constexpr int kindsOfBirds = 8; /**< Kinds of unique birds in the game */
constexpr int numberOfRows = 4; /**< Amount of rows on the table */
constexpr int numberSides = 2; /**< Amount of sides per row */
constexpr std::array<std::string_view, 8> birdTypes = {
    "Flamingo", "Owl", "Toucan", "Duck", "Parrot", "Magpie", "Reed Warbler", "Robin"}; /**< Birdname per index*/
constexpr std::array<int, kindsOfBirds>  smallFam = {2,3,3,4,4,5,6,6}; /**< Amount of birds needed for a small family*/
constexpr std::array<int, kindsOfBirds>  bigFam = {3,4,4,6,6,7,9,9}; /**<  Amount of birds needed for a big family*/
constexpr std::array<int, kindsOfBirds>  numberBirds = {7,10,10,13,13,17,20,20}; /**<  Amount of birds in the game*/
constexpr std::array<float, kindsOfBirds>  rarityScore = {1,0.538,0.538,0.290,0.290,0.095,0,0}; /**<  Score based on rarity in the game*/

/**
 * @class Table
 * @brief Represents the table in the CuBirds game.
 *
 * The table consists of the rows of the board, a drawpile and a discardpile
 */

class Table {
public:
    /* --- STANDARD GAME LOGIC --- */

    /**
     * @brief Constructs a table
     * 
     * This constructor initializes a table 
     */
    Table();

    /**
     * @brief Adds card(s) to a specific place on the table
     * 
     * @param id Id of the card to be added to the row
     * @param rowNumber Specifies the row where the card should be added
     * @param side Specifies which side the card should be added
     * @param numberOfCards Specefies the number of cards to be added
     * 
     * Value of @side can be 0 for left (front) or 1 for right (back)
     * 
     */
    void addCards(int id, int rowNumber, bool side, int numberOfCards);

    /** 
     * @brief Adds card to discard pile
     * 
     * @param id Id of the card that needs to be added to the discard pile
     * 
     * The card has to be removed from elsewhere in the game after adding 
     *      the card to the discard pile
     */
    void addCardToDiscard(int id);

    /**
     * @brief Empty the discard pile
     * 
     * by setting every index to zero
     */
    void clearDiscardPile();

    /**
     * @brief Returns whether the @drawPile is empty
     * 
     * When the pile is empty it first tries to reshuffle from the discard pile
     * 
     * @return True if the drawPile is empty, false if the drawpile is not empty
     */
    bool drawPileEmpty();

    /** 
     * @brief Get a card from the draw pile
     * 
     * If the drawPile is not empty it makes a weighted random choice between
     * the different kinds of cards
     * 
     * @return index of a card from the drawpile
     * 
     * @throw logic_error If the mapping does not resolve to an index
     */
    int drawCard();

    /**
     * @brief Reshuffle from the discard pile
     * 
     * Adds the cards from the discard pile to the draw pile
     * Then clears the discard pile
     */
    void reshuffleFromDiscardPile();

    /**
     * @brief Resolves a row where cards are added and returns the collected cards
     * 
     * When cards are enclosed between two cards with the same id these cards are returned
     * 
     * @param id Id of the card that was added to the row that turn
     * @param rowNumber The number of the row that needs to be resolved
     * @param side The side where the cards were added
     * @param numberCards The number of cards that were added to the row
     * 
     * The birds that are enclosed are stored in @collectedCards
     * Cards are added to the row if needed
     * 
     * @return The cards that should be collected by the player after the row resolves
     */
    std::vector<int> resolveRow(int id, int rowNumber, int side, int numberCards);

    /* --- PRINT FUNCTIONS --- */

    /**
     * @brief prints the drawPile array
     */
    void printDrawPile();

    /**
     * @brief prints the discardPile array
     */
    void printDiscardPile();

    /**
     * @brief Prints all rows on the table to the terminal
     * 
     * Cards are printed as their index
     */
    void printTable();

    /**
     * @brief Prints all rows on the table to the terminal
     * 
     * Cards are printed as their birdname
     */
    void printTableTypes();

    /*GET FUNCTIONS*/

    /**
     * @brief Gets the number of cards in the discard pile
     * 
     * @return The number of cards in the discard pile
     */
    int getDiscardSize();

    /**
     * @brief Gets the discard pile
     * 
     * @return The array @discardPile
     */
    std::array<int, kindsOfBirds> getDiscardPile();

    /** 
     * @brief Gets the drawpile array
     * @return The drawpile as an array
     */
    std::array<int, kindsOfBirds> getDrawPile();

    /**
     * @brief Gets the a row of the board vector
     * 
     * @param rowNumber Number of a row on the table (0-3)
     * 
     * @return The vector of a row of the board
     */
    std::vector<int> getRow(int rowNumber) const;

    /**
     * @brief Gets the number of cards in the draw pile
     * 
     * @return The number of cards in the draw pile
     */
    int getDrawSize();

    /**
     * @brief Gets the random machine 
     * 
     * @return The rng
     */
    std::mt19937& getRNG() {
        return rng;
    }

    /* --- SET FUNCTIONS --- */

    /**
     * @brief Initializes a draw pile
     * 
     * This function initializes a draw pile as an array of cards
     */
    void setDrawPile(std::array<int, kindsOfBirds> drawVector);

    /**
     * @brief Sets the row to a particular vector
     * 
     * @param row New value of the row
     * @param rowNumber Number of the row to be replaced
     */
    void setRow(std::vector<int> row, int rowNumber);

    /**
     * @brief Sets the discard pile
     * 
     * @param row New value of the discarpile
     */
    void setDiscardPile(std::array<int, kindsOfBirds> discardVector);

    std::array<int, kindsOfBirds> discardPile = {}; /**< The discard pile. */
    std::array<int, kindsOfBirds> drawPile = {}; /**< The draw pile. */

private:
    std::array<std::vector<int>, numberOfRows> table = {}; /**< The table with 4 rows */
    std::mt19937 rng; /**< Seed for the random machine */
};

#endif // TABLE_H