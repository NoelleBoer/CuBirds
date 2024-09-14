#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "card.h"
#include "table.h"

class Player {
public:
    Player(std::string name);
    void drawCard(const Card& card);
    void playCard(const Card& card);
    void collectBird(const std::string& birdType);

private:
    std::string name;
    std::vector<Card> hand;
    std::vector<Card> collections;
};

#endif