#include "table.h"
#include <random>
#include <iostream>

Table::Table() {}

void Table::addCards(int id, int rowNumber, bool side, int numberOfCards) {
    for (int i = 0; i < numberOfCards; i++){
        if (!side) table[rowNumber].insert(table[rowNumber].begin(), id);
        else table[rowNumber].push_back(id);
    }
}

void Table::addCardToDiscard(int id) {
    discardPile[id]++;
}

void Table::clearDiscardPile() {
    for (int i = 0; i < kindsOfBirds; i++) {
        discardPile[i] = 0;
    }
}

bool Table::drawPileEmpty(){
    for (int i = 0; i < kindsOfBirds; i++){
        if (drawPile[i] != 0) return false;
    }
    reshuffleFromDiscardPile();
    for (int i = 0; i < kindsOfBirds; i++){
        if (drawPile[i] != 0) return false;
    }
    return true;
}

int Table::drawCard() {
    if (!drawPileEmpty()){
        int totalCards = getDrawSize();
        std::random_device rd;  // Seed
        std::mt19937 gen(rd()); // Mersenne Twister RNG
        std::uniform_int_distribution<int> dis(1, totalCards);
        int randomCard = dis(gen);

        // Map the random number to a card ID based on weights
        int cumulativeSum = 0;
        for (int i = 0; i < kindsOfBirds; i++) {
            cumulativeSum += drawPile[i];
            if (randomCard <= cumulativeSum) {
                drawPile[i]--;
                return i; // Return the card ID
            }
        }
    }
    throw std::logic_error("Card draw failed: randomCard mapping did not resolve.");
}

void Table::reshuffleFromDiscardPile() {
    for (int i = 0; i < kindsOfBirds; i++){
        drawPile[i] += discardPile[i];
        discardPile[i] = 0;
    }
}

std::vector<int> Table::resolveRow(int id, int rowNumber, int side, int numberCards){
    if (numberCards == 0) return {};
    std::vector<int> collectedCards; //Keeps track of which cards are to be collected
    int indexBreak = -1; // To mark where the enclosing id is found
    std::vector<int> row = getRow(rowNumber); // Copy of the row that is to be resolved

    if (side == 0) { // Cards were added to the left
        for (size_t i = numberCards; i < row.size(); ++i) {
            if (row[i] == id) { // Next card of id is found
                indexBreak = i; // Remember where this card is found
                break;
            }
            collectedCards.push_back(row[i]); // Push every card until a card of id is found
        }
        if (indexBreak == -1) { // No cards are enclosed
            collectedCards.clear();
        } else {
            if (numberCards >= 0 && indexBreak < static_cast<int>(row.size())) {
                row.erase(row.begin() + numberCards, row.begin() + indexBreak); // Erase the enclosed cards
            }
        }
    } else if (side == 1) { // Cards were added to the right
        for (int i = row.size() - numberCards - 1; i >= 0; --i) {
            if (row[i] == id) { // Next card of id is found
                indexBreak = i; // Remember where this card is found
                break;
            }
            collectedCards.push_back(row[i]);  // Push every card until card of id is found
        }
        if (indexBreak == -1) { // No cards are enclosed
            collectedCards.clear();
        } else {
            if (indexBreak + 1 < static_cast<int>(row.size()) && row.size() - numberCards >= 0) {
                row.erase(row.begin() + indexBreak + 1, row.end() - numberCards); // Erase enclosed cards
            }
        }
    }

    //Check if all cards remaining on the row have the same birdtype
    bool allCardsMatch = true;
    for (const int crd : row) {
        if (id!=crd) {
            allCardsMatch = false;
            break;
        }
    }

    //If there is only one kind on a row, add cards until there are two types
    if (allCardsMatch) {
        if (!drawPileEmpty()) {
            int newCard = drawCard();
            while (newCard == id && !drawPileEmpty()) {
                row.push_back(newCard);
                newCard = drawCard();
            }
            row.push_back(newCard);
        }
    }

    table[rowNumber] = row;

    return collectedCards;
}

//Print functions
void Table::printDrawPile() {
    std::array<std::string, 8>  birdTypes = {"Flamingo", "Owl", "Toucan", "Duck", 
                                      "Parrot", "Magpie", "Reed Warbler", "Robin"};
    std::cout << "Drawpile: " << std::endl;
    for (int i = 0; i < kindsOfBirds; i++){
        std::cout << birdTypes[i] << ": " << drawPile[i] << std::endl;
    }
    std::cout << std::endl;
}

void Table::printDiscardPile() {
    std::array<std::string, 8>  birdTypes = {"Flamingo", "Owl", "Toucan", "Duck", 
                                      "Parrot", "Magpie", "Reed Warbler", "Robin"};
    std::cout << "Discardpile: " << std::endl;
    for (int i = 0; i < kindsOfBirds; i++){
        std::cout << birdTypes[i] << ": " << discardPile[i] << std::endl;
    }
    std::cout << std::endl;
}

void Table::printTable(){
    for (int i = 0; i < numberOfRows; i++){
        for (int card : table[i]) {
            std::cout << card << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Table::printTableTypes(){
    std::array<std::string, 8>  birdTypes = {"Flamingo", "Owl", "Toucan", "Duck", 
                                      "Parrot", "Magpie", "Reed Warbler", "Robin"};
    for (int i = 0; i < numberOfRows; i++){
        for (int card : table[i]) {
            std::cout << birdTypes[card] << " ";
        }
        std::cout << std::endl;
    }
}

//Get functions
int Table::getDiscardSize(){
    int totalSize = 0;
    for (int i = 0; i < kindsOfBirds; i++){
        totalSize += discardPile[i];
    }
    return totalSize;
}

int Table::getDrawSize(){
    int totalSize = 0;
    for (int i = 0; i < kindsOfBirds; i++){
        totalSize += drawPile[i];
    }
    return totalSize;
}

std::array<int, 8> Table::getDrawPile(){
    return drawPile;
}

std::vector<int> Table::getRow(int rowNumber) const {
    return table[rowNumber];
}

//Set functions
void Table::setDrawPile(std::array<int, 8> drawVector) {
    drawPile = drawVector;
}

void Table::setRow(std::vector<int> row, int rowNumber){
    table[rowNumber] = row;
}

void Table::setDiscardPile(std::array<int, 8> discardVector) {
    discardPile = discardVector;
}