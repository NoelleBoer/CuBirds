//---player 0---//
//plays the first card in hand
//always plays a family when able
//draws 2 cards if no cards are collected

#include "player.h"
#include <algorithm>
#include <iostream>

Player::Player(std::string name, int playerType) : name(name), playerType(playerType) {}

void Player::drawCard(const Card& card) {
    hand.push_back(card);
}

void Player::collectBird(const Card& card) {
    collection.push_back(card);
}

void Player::printHand() {
    std::cout << "Hand " << name << ": ";
    for (const Card& card : hand) {
        std::cout << card.getBirdType() << " ";
    }
    std::cout << std::endl;
}

void Player::printCollection() {
    std::cout << "Collection " << name << ": ";
    for (const Card& card : collection) {
        std::cout << card.getBirdType() << " ";
    }
    std::cout << std::endl;
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

int Player::getType() {
    return playerType;
}

void Player::emptyHand() {
    hand.clear();
}

void Player::deleteType(const Card& cardToRemove) {
    std::string birdTypeToRemove = cardToRemove.getBirdType();
    hand.erase(std::remove_if(hand.begin(), hand.end(), 
        [&birdTypeToRemove](const Card& card) {
            return card.getBirdType() == birdTypeToRemove;
        }), 
        hand.end());
}

void Player::discardCard(const Card& cardToRemove) {
    std::string birdTypeToRemove = cardToRemove.getBirdType();
    auto it = std::find_if(hand.begin(), hand.end(),
        [&birdTypeToRemove](const Card& card) {
            return card.getBirdType() == birdTypeToRemove;
        });

    // Erase the card
    hand.erase(it);
}