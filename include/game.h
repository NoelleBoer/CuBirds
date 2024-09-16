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
    void endGame();
    bool startTurn(Player& player);
    bool checkForWin(Player& player);
    bool checkEmptyHand(Player& player);
    void playCards(Player& player);
    void playFamily(Player& player);
    bool resolveTable(Player& player, int row, const Card& card);
};

#endif