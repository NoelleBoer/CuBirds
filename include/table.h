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
    std::vector<Card> getFirstRow() const;
    std::vector<Card> getSecondRow() const;
    std::vector<Card> getThirdRow() const;
    std::vector<Card> getFourthRow() const;

private:
    std::vector<Card> firstRow;
    std::vector<Card> secondRow;
    std::vector<Card> thirdRow;
    std::vector<Card> fourthRow;
    std::vector<Card> discardPile; 
};

#endif