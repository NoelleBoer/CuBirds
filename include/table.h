#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <array>
#include <string>

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
     * This function initializes a draw pile with the 110 standard cards.
     */
    void setDrawPile(std::array<int, 8> drawVector);

    /**
     * @brief Adds card(s) to a row on the table
     * 
     * @param card The card to be added to the row
     * @param rowNumber Specifies the row where the card should be added
     * @param side Specifies which side the card should be added
     * Value of side can be 0 for left (front) or 1 for right (back)
     * @param numberOfCards Number of cards that should be added
     */
    void addCards(int id, int rowNumber, bool side, int numberOfCards);

    /**
     * @brief Resolves a row and returns the collected cards
     * 
     * When cards are enclosed between two cards of the same birdType these cards are returned
     * The function also returns whether the deck is empty or not when completing the row from deck
     * 
     * @param card The card that was added to the row that turn
     * @param rowNumber The number of the row that needs to be resolved
     * 
     * @return The cards that should be collected by the player and whether the deck is empty
     */
    std::vector<int> resolveRow(int id, int rowNumber, int side, int numberCards);

    /** 
     * @brief Adds card to discard pile
     * 
     * @param Card that needs to be added to the discard pile
     */
    void addCardToDiscard(int id);

    /**
     * @brief Get the discard pile
     * 
     * @return The discard pile vector
     */
    std::array<int, 8> getDiscardPile() const;

    /**
     * @brief Empty the discard pile
     */
    void clearDiscardPile(); 

    /**
     * @brief Reshuffle from the discard pile
     * 
     * Adds the cards from the discard pile to the draw pile
     * Then shuffles the draw pile
     * Then clears the discard pile
     */
    void reshuffleFromDiscardPile();

    /**
     * @brief Shuffles the drawpile
     */
    void shuffleDrawPile();

    /** 
     * @brief Get a card from the draw pile
     * 
     * If the drawpile is empty reshuffle from the discard pile
     * Get the back card of the drawpilevector
     * Delete this card
     * 
     * @return The last card in the drawpile vector
     * 
     * If the drawpile is empty after reshuffle it returns a card with birdType "Empty"
     */
    int drawCard();

    /**
     * @brief Gets the first row of the board
     * 
     * @return The first row of the board as a vector of the class Card
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
     */
    void printTable();
    bool drawPileEmpty();
    void printTableTypes();
    void setRow(std::vector<int> row, int rowNumber);
    void printDrawPile();


private:
    std::array<std::vector<int>, 4> table;
    std::array<int, 8> discardPile = {};
    std::array<int, 8> drawPile = {}; /**< The draw pile. */
};

#endif // TABLE_H