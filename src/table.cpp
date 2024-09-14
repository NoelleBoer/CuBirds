#include "table.h"

Table::Table() {}

void Table::addCard(const Card& card, int rowNumber, bool side) {
    if (rowNumber == 1) {
        if (!side) {
            firstRow.insert(firstRow.begin(),card);
        } else {
            firstRow.push_back(card);
        } 
    } else if (rowNumber == 2) {
        if (!side) {
            secondRow.insert(secondRow.begin(),card);
        } else {
            secondRow.push_back(card);
        } 
    } else if (rowNumber == 3) {
        if (!side) {
            thirdRow.insert(thirdRow.begin(),card);
        } else {
            thirdRow.push_back(card);
        } 
    } else if (rowNumber == 4) {
        if (!side) {
            fourthRow.insert(fourthRow.begin(),card);
        } else {
            fourthRow.push_back(card);
        } 
    }
}

std::vector<Card> Table::getFirstRow() const {
    return firstRow;
}
std::vector<Card> Table::getSecondRow() const {
    return secondRow;
}
std::vector<Card> Table::getThirdRow() const {
    return thirdRow;
}
std::vector<Card> Table::getFourthRow() const {
    return fourthRow;
}

void Table::addCardToDiscard(const Card& card) {
    discardPile.push_back(card);
}

std::vector<Card> Table::getDiscardPile() const {
    return discardPile;
}

void Table::clearDiscardPile() {
    discardPile.clear();
}