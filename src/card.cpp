#include "card.h"

Card::Card(std::string birdType, int smallFamily, int bigFamily, int numberBirds)
    : birdType(birdType), smallFamily(smallFamily), bigFamily(bigFamily), numberBirds(numberBirds) {}

std::string Card::getBirdType() const {
    return birdType;
}

int Card::getSmallFamily() const {
    return smallFamily;
}

int Card::getBigFamily() const {
    return bigFamily;
}

int Card::getNumberBirds() const {
    return numberBirds;
}