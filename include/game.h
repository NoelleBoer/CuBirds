#ifndef GAME_H
#define GAME_H

#include <vector>
#include "player.h"
#include "deck.h"
#include "table.h"

class Game {
public:
    Game();
    void play();

private:
    std::vector<Player> players;
    Deck deck;
    Table table;

    void startTurn(Player& player);
    void checkForEndGame();
    void handleEmptyDeck();
};

#endif