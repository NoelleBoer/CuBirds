#ifndef CARD_H
#define CARD_H

#include <string>

/**
 * @class Card
 * @brief Represents a bird card in the CuBirds game.
 *
 * The Card class holds information about a specific bird type,
 * including the number of birds needed to form small and big families.
 * It also tracks the total number of birdcards of this type in the game.
 */

class Card {
public:
    /**
     * @brief Constructs a Card with specified parameters.
     *
     * This constructor initializes a bird card with the provided bird type,
     * the number of birds required for a small family, a big family, and
     * the total number of birdcards in a standard deck for this bird type.
     * 
     * @param birdType The type of bird represented by the card.
     * @param smallFamily The number of birds required to form a small family.
     * @param bigFamily The number of birds required to form a big family.
     * @param numberBirds The total number of this bird available in a standard deck.
     */
    Card(std::string birdType, int smallFamily, int bigFamily, int numberBirds);

    /**
     * @brief Default constructor for Card.
     *
     * Initializes a card with default values: {"Unknown",0,0,0}.
     */
    Card();

    /**
     * @brief Gets the bird type of the card.
     * The possible values of birdType are:
     * - "Flamingo"
     * - "Owl"
     * - "Toucan"
     * - "Duck"
     * - "Parrot"
     * - "Magpie"
     * - "Reed Warbler"
     * - "Robin"
     * - "Unknown" initial value
     * - "Empty" to signal that the deck is empty
     * @return The bird type as a string.
     */
    std::string getBirdType() const;

     /**
     * @brief Gets the number of birds required for a small family.
     * A small family is a number between 0 and 6
     * @return The number of birds needed to form a small family.
     */
    int getSmallFamily() const;

    /**
     * @brief Gets the number of birds required for a big family.
     * A big family is a number between 0 and 9
     * @return The number of birds needed to form a big family.
     */
    int getBigFamily() const;

    /**
     * @brief Gets the total number of birds of this type available in the game.
     * The number of birds is a number between 0 and 20
     * @return The total number of birds for this type in a standard deck.
     */
    int getNumberBirds() const;

private:
    std::string birdType; /**< The type of bird represented by the card. */
    int smallFamily; /**< Number of birds required to form a small family. */
    int bigFamily; /**< Number of birds required to form a big family. */
    int numberBirds; /**< Total number of this bird type available in the game. */
};

#endif // CARD_H