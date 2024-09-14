#ifndef CARD_H
#define CARD_H

#include <string>

class Card {
public:
    Card(std::string birdType, int smallFamily, int bigFamily, int numberBirds);
    std::string getBirdType() const;
    int getSmallFamily() const;
    int getBigFamily() const;
    int getNumberBirds() const;

private:
    std::string birdType;
    int smallFamily;
    int bigFamily;
    int numberBirds;
};

#endif