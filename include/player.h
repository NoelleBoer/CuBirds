#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "card.h"
#include "table.h"

class Player {
public:
    Player(std::string name, int playerType);
    std::string getName();
    std::vector<Card> getHand();
    std::vector<Card> getCollection();
    int getType();

    void drawCard(const Card& card);
    void collectBird(const Card& card);
    void emptyHand();
    void deleteType(const Card& card);
    void discardCard(const Card& card);

    void printHand();
    void printCollection();

private:
    std::string name;
    std::vector<Card> hand;
    std::vector<Card> collection;
    int playerType;
};

#endif