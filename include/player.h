#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "card.h"
#include "table.h"

class Player {
public:
    Player(std::string name, int playerType);
    void drawCard(const Card& card);
    void playCard();
    void playFamily();
    void collectBird(const Card& card);
    void printHand();
    void emptyHand();
    std::vector<Card> getCollection();
    std::vector<Card> getHand();
    std::string getName();

private:
    std::string name;
    std::vector<Card> hand;
    std::vector<Card> collection;
    int playerType;
};

#endif