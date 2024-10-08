#include "player.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <ctime>

Player::Player(int playerType, int index) : playerType(playerType), index(index){}

void Player::drawCard(const Card& card) {
    hand.push_back(card);
}

void Player::collectBird(const Card& card) {
    collection.push_back(card);
}

void Player::printHand() {
    std::cout << "Hand of " << index << ": ";
    for (const Card& card : hand) {
        std::cout << card.getBirdType() << " ";
    }
    std::cout << std::endl;
}

void Player::printCollection() {
    std::cout << "Collection of " << index << ": ";
    for (const Card& card : collection) {
        std::cout << card.getBirdType() << " ";
    }
    std::cout << std::endl;
}

void Player::shuffleHand() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(hand.begin(), hand.end(), g);
}

std::vector<Card> Player::getCollection() {
    return collection;
}

std::vector<Card> Player::getHand() {
    return hand;
}

int Player::getType() {
    return playerType;
}

int Player::getIndex() {
    return index;
}

void Player::emptyHand() {
    hand.clear();
}

int Player::getHandSize(){
    return hand.size();
}

//uses a lambda function to compare each card with the birdType of the cardToRemove
void Player::deleteType(const Card& cardToRemove) {
    std::string birdTypeToRemove = cardToRemove.getBirdType();
    hand.erase(std::remove_if(hand.begin(), hand.end(), 
        [&birdTypeToRemove](const Card& card) {
            return card.getBirdType() == birdTypeToRemove;
        }), 
        hand.end());
}