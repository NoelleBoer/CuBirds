#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include "card.h"

class Table {
public:
    Table();
    void initializeDrawPile();
    void addCard(const Card& card, int rowNumber, bool side);
    std::pair<std::vector<Card>, bool> resolveRow(const Card& card, int rowNumber);

    void addCardToDiscard(const Card& card);
    std::vector<Card> getDiscardPile() const;
    void clearDiscardPile(); 
    void reshuffleFromDiscardPile();

    void shuffleDrawPile();
    Card drawCard();
    std::vector<Card> shuffleDrawPile() const;

    std::vector<Card> getFirstRow() const;
    std::vector<Card> getSecondRow() const;
    std::vector<Card> getThirdRow() const;
    std::vector<Card> getFourthRow() const;
    int getDiscardSize();
    int getDrawSize();

    void printTable();


private:
    std::vector<Card> firstRow;
    std::vector<Card> secondRow;
    std::vector<Card> thirdRow;
    std::vector<Card> fourthRow;
    std::vector<Card> discardPile; 
    std::vector<Card> drawPile;
};

#endif