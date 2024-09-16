#include "table.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>
#include <ostream>

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

void Table::printTable(){
    for (const Card& card : firstRow) {
        std::cout << card.getBirdType() << " ";
    }
    std::cout << std::endl;
    for (const Card& card : secondRow) {
        std::cout << card.getBirdType() << " ";
    }
    std::cout << std::endl;
    for (const Card& card : thirdRow) {
        std::cout << card.getBirdType() << " ";
    }
    std::cout << std::endl;
    for (const Card& card : fourthRow) {
        std::cout << card.getBirdType() << " ";
    }
    std::cout << std::endl<<std::endl;
}

void Table::initializeDrawPile() {
    // Add all 110 standard cards
    for (int i = 0; i<7; i++) {
        drawPile.push_back(Card("Flamingo",2,3,7));
    }
    for (int i = 0; i<10; i++) {
        drawPile.push_back(Card("Owl", 3,4,10));
    }
    for (int i = 0; i<10; i++) {
        drawPile.push_back(Card("Toucan", 3,4,10));
    }
    for (int i = 0; i<13; i++) {
        drawPile.push_back(Card("Duck", 4,6,13));
    }
    for (int i = 0; i<13; i++) {
        drawPile.push_back(Card("Parrot", 4,6,13));
    }
    for (int i = 0; i<17; i++) {
        drawPile.push_back(Card("Magpie", 5,7,17));
    }
    for (int i = 0; i<20; i++) {
        drawPile.push_back(Card("Reed Warbler", 6,9,20));
    }
    for (int i = 0; i<20; i++) {
        drawPile.push_back(Card("Robin", 6,9,20));
    }
}

void Table::shuffleDrawPile() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(drawPile.begin(), drawPile.end(), std::default_random_engine(seed));
}

Card Table::drawCard() {
    if (drawPile.empty()){
        reshuffleFromDiscardPile();
    }
    if (!drawPile.empty()) {
        Card drawnCard = drawPile.back();
        drawPile.pop_back();
        return drawnCard;
    } else {
        return Card("Empty", 0,0,10);
    }
}

void Table::reshuffleFromDiscardPile() {
    std::vector<Card> discardPile = getDiscardPile();
    if (!discardPile.empty()) {
        drawPile.insert(drawPile.end(), discardPile.begin(), discardPile.end());  // Move discard pile into the deck
        shuffleDrawPile();
        clearDiscardPile();  // Clear discard pile after reshuffling
    }
}

int Table::getDiscardSize(){
    return discardPile.size();
}

int Table::getDrawSize(){
    return drawPile.size();
}

std::pair<std::vector<Card>, bool> Table::resolveRow(const Card& card, int rowNumber){
    std::vector<Card> collectedCards;
    std::vector<Card> row;
    bool deckEmpty = false;
    //get a subvector of the enclosed birds
    if (rowNumber == 1) {
        row = firstRow;
    } else if (rowNumber == 2) {
        row = secondRow;
    } else if (rowNumber == 3) {
        row = thirdRow;
    } else if (rowNumber == 4) {
        row = fourthRow;
    }
    for (size_t i = 0; i < row.size(); ++i) {
        for (size_t j = i + 1; j < row.size(); ++j) {
            if (row[i].getBirdType() == row[j].getBirdType()) {
                // Return the subvector between the two matching cards (exclusive)
                collectedCards = std::vector<Card>(row.begin() + i + 1, row.begin() + j);
                row.erase(row.begin() + i + 1, row.begin() + j);
            }
        }
    }

    //If only one kind of bird on a row add cards till there are 2 kinds of birds
    bool allCardsMatch = true;
    for (const Card& crd : row) {
        if (card.getBirdType()!=crd.getBirdType()) {
            allCardsMatch = false;
            break;
        }
    }

    if (allCardsMatch) {
        Card newCard = drawCard();
        if (newCard.getBirdType() == "Empty") {
            deckEmpty = true;
        }
        while ((newCard.getBirdType() == card.getBirdType()) && newCard.getBirdType() != "Empty"){
            row.push_back(newCard);
            newCard = drawCard();
        }
        if (newCard.getBirdType() == "Empty") {
            deckEmpty = true;
        } else {
            row.push_back(newCard);
        }
    }

    if (rowNumber == 1) {
        firstRow = row;
    } else if (rowNumber == 2) {
        secondRow = row;
    } else if (rowNumber == 3) {
        thirdRow = row;
    } else if (rowNumber == 4) {
        fourthRow = row;
    }
    return std::make_pair(collectedCards, deckEmpty);
}