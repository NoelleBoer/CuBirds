#include "player.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <ctime>
#include <array>

Player::Player(int playerType, int index) 
    : hand{}, collection{}, playerType(playerType), index(index) {}


void Player::drawCard(int id) {
    hand[id]++;
}

void Player::collectBird(int id) {
    collection[id]++;
}

void Player::printHand() {
    std::array<std::string, 8>  birdTypes = {"Flamingo    ", "Owl         ", "Toucan      ", "Duck        ", 
                                      "Parrot      ", "Magpie      ", "Reed Warbler", "Robin       "};
    std::cout << "Hand of player " << index << ": "<< std::endl;
    for (int i = 0; i <8; i++){
        std::cout << i << ". " << birdTypes[i] << ": " << hand[i] << std::endl;
    }
    std::cout << std::endl;
}

void Player::printCollection() {
    std::array<std::string, 8>  birdTypes = {"Flamingo    ", "Owl         ", "Toucan      ", "Duck        ", 
                                      "Parrot      ", "Magpie      ", "Reed Warbler", "Robin       "};
    std::cout << "Collection of player " << index << ": " << std::endl;
    for (int i = 0; i <8; i++){
        std::cout << i << ". " << birdTypes[i] << ": " << collection[i] << std::endl;
    }
    std::cout << std::endl;
}

std::array<int, 8>  Player::getCollection() {
    return collection;
}

std::array<int, 8>  Player::getHand() {
    return hand;
}

int Player::getType() {
    return playerType;
}

int Player::getIndex() {
    return index;
}

void Player::emptyHand() {
    for (int i = 0; i<8; i++){
        hand[i] = 0;
    }
}

int Player::getHandSize(){
    int handSize = 0;
    for (int i = 0; i<8; i++){
        handSize += hand[i];
    }
    return handSize;
}

void Player::deleteType(int id) {
    hand[id] = 0;
}

void Player::changeHand(std::array<int, 8>  newHand){
    hand = newHand;
}