#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <array>

/**
 * @class Table
 * @brief Represents the table in the CuBirds game.
 *
 * The table consists of the rows of the board, a drawpile and a discardpile
 */

class Table {
public:
    /**
     * @brief Constructs a table
     * 
     * This constructor initializes a table 
     */
    Table();

    /**
     * @brief Initializes a draw pile
     * 
     * This function initializes a draw pile as an array of cards
     */
    void setDrawPile(std::array<int, 8> drawVector);

    /**
     * @brief Adds card(s) to a specific place on the table
     * 
     * @param id Id of the card to be added to the row
     * @param rowNumber Specifies the row where the card should be added
     * @param side Specifies which side the card should be added
     * 
     * Value of side can be 0 for left (front) or 1 for right (back)
     * 
     * @param numberOfCards Number of cards that should be added
     */
    void addCards(int id, int rowNumber, bool side, int numberOfCards);

    /**
     * @brief Resolves a row and returns the collected cards
     * 
     * When cards are enclosed between two cards of the same birdType these cards are returned
     * 
     * @param id Id of the card that was added to the row that turn
     * @param rowNumber The number of the row that needs to be resolved
     * @param side The side where the cards were added
     * @param numberCards The number of cards that were added to the row
     * 
     * @return The cards that should be collected by the player after the row resolves
     */
    std::vector<int> resolveRow(int id, int rowNumber, int side, int numberCards);

    /** 
     * @brief Adds card to discard pile
     * 
     * @param id Id of the card that needs to be added to the discard pile
     */
    void addCardToDiscard(int id);

    /**
     * @brief Empty the discard pile
     * 
     * by setting every index to zero
     */
    void clearDiscardPile(); 

    /**
     * @brief Reshuffle from the discard pile
     * 
     * Adds the cards from the discard pile to the draw pile
     * Then clears the discard pile
     */
    void reshuffleFromDiscardPile();

    /** 
     * @brief Get a card from the draw pile
     * 
     * If the drawPile is not empty it makes a weighted random choice between
     * the different kinds of cards
     * 
     * @return index of a card from the drawpile
     * 
     * If the drawpile is empty after reshuffle it returns a card with birdType "Empty"
     * 
     * @throw logic_error If the mapping does not resolve to an index
     */
    int drawCard();

    /**
     * @brief Gets the a row of the board vector
     * 
     * @param rowNumber Number of a row on the table (0-3)
     * 
     * @return The vector of a row of the board
     */
    std::vector<int> getRow(int rowNumber) const;

    /**
     * @brief Gets the number of cards in the discard pile
     * 
     * @return The number of cards in the discard pile
     */
    int getDiscardSize();

    /**
     * @brief Gets the number of cards in the draw pile
     * 
     * @return The number of cards in the draw pile
     */
    int getDrawSize();

    /**
     * @brief Prints all rows on the table to the terminal
     * 
     * Cards are printed as their index
     */
    void printTable();

    /**
     * @brief Returns whether the drawPile is empty
     * 
     * When the pile is empty it first tries to reshuffle from the discard pile
     * 
     * @return True if the drawPile is empty, false if the drawpile is not empty
     */
    bool drawPileEmpty();

    /**
     * @brief Prints all rows on the table to the terminal
     * 
     * Cards are printed as their birdname
     */
    void printTableTypes();

    /**
     * @brief Sets the row to a particular vector
     * 
     * @param row New value of the row
     * @param rowNumber Number of the row to be replaced
     */
    void setRow(std::vector<int> row, int rowNumber);

    /**
     * @brief prints the drawPile array
     */
    void printDrawPile();

    /** 
     * @brief Gets the drawpile array
     * @return The drawpile as an array
     */
    std::array<int, 8> getDrawPile();


private:
    std::array<std::vector<int>, 4> table; /**< The table with 4 rows */
    std::array<int, 8> discardPile = {}; /**< The discard pile. */
    std::array<int, 8> drawPile = {}; /**< The draw pile. */
    int kindsOfBirds = 8; /**< Kinds of unique birds in the game */
    int numberOfRows = 4; /**< Amount of rows on the table */
};

#endif // TABLE_H