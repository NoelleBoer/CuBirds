//player 0 plays random cards and always plays a family when able

#include "player.h"
#include <algorithm>
#include <iostream>

Player::Player(std::string name, int playerType) : name(name), playerType(playerType) {}

void Player::drawCard(const Card& card) {
    hand.push_back(card);
}

void Player::playCard() {
    // kies een random type uit hand
    // speel alle kaarten van deze soort op een random rij aan een random kant
    // resolveTable na elke kaart
}

void Player::playFamily() {
    //check of er een familie uit de hand gespeeld kan worden
    //addcollection & adddiscard
}

void Player::collectBird(const Card& card) {
    collection.push_back(card);
}
void Player::printHand() {
    for (const Card& card : hand) {
        std::cout << card.getBirdType() << " ";
    }
}

std::vector<Card> Player::getCollection() {
    return collection;
}

std::vector<Card> Player::getHand() {
    return hand;
}


std::string Player::getName() {
    return name;
}

void Player::emptyHand() {
    hand.clear();
}