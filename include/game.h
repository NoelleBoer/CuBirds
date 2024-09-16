#ifndef GAME_H
#define GAME_H

#include <vector>
#include "player.h"
#include "table.h"
#include "card.h"

class Game {
public:
    Game();
    void play();

private:
    std::vector<Player> players;
    Table table;
    void startGame();
    bool startTurn(Player& player);
    bool checkForWin(Player& player);
    bool checkEmptyHand(Player& player);
    bool handleEmptyDeck(const Card& card);
    void resolveTable(Player& player);
};

#endif