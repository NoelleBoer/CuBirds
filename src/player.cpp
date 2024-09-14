#include "player.h"
#include <algorithm>

Player::Player(std::string name) : name(name) {}

void Player::drawCard(const Card& card) {
    hand.push_back(card);
}

void Player::playCard(const Card& card) {
    // TODO: Logic to play a card from the player's hand
}

void Player::collectBird(const std::string& birdType) {
    // TODO: Logic to collect bird cards
    // dont forget: table.addCardToDiscard(card);
}