#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include "card.h"

class Table {
public:
    Table();
    void addCard(const Card& card, int rowNumber, bool side);
    void addCardToDiscard(const Card& card);
    std::vector<Card> getDiscardPile() const;
    void clearDiscardPile(); 
    void printTable();
    void reshuffleFromDiscardPile();
    void shuffleDrawPile();
    Card drawCard();
    void initializeDrawPile();
    std::vector<Card> getFirstRow() const;
    std::vector<Card> getSecondRow() const;
    std::vector<Card> getThirdRow() const;
    std::vector<Card> getFourthRow() const;
    std::vector<Card> initializeDrawPile() const;
    std::vector<Card> shuffleDrawPile() const;
    int getDiscardSize();
    int getDrawSize();

private:
    std::vector<Card> firstRow;
    std::vector<Card> secondRow;
    std::vector<Card> thirdRow;
    std::vector<Card> fourthRow;
    std::vector<Card> discardPile; 
    std::vector<Card> drawPile;
    void initializeDeck();
};

#endif