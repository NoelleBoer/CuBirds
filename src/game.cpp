#include "game.h"

Game::Game() {
    // Always initialize with 2 players
    players.push_back(Player("Player 1"));
    players.push_back(Player("Player 2"));
}

void Game::play() {
    // Game loop logic
    while (true) {
        for (Player& player : players) {
            startTurn(player);
            checkForEndGame();
        }
    }
}

void Game::startTurn(Player& player) {
    // TODO: Logic for each player's turn
}

void Game::checkForEndGame() {
    // TODO: Check if any player has won or if game should end
}

void Game::handleEmptyDeck() {
    if (deck.drawCard(table).getBirdType() == "Empty") {
        deck.reshuffleFromDiscardPile(table);  // Reshuffle discard pile into the deck
    }
}