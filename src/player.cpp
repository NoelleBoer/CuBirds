#include "player.h"

#include <iostream>

Player::Player(int index, float k, float l, float m, float n, float o) 
    : hand{}, collection{}, index(index), k(k),l(l),m(m),n(n),o(o){}


void Player::drawCard(int id) {
    hand[id]++;
}

void Player::collectBird(int id) {
    collection[id]++;
}

void Player::deleteType(int id) {
    hand[id] = 0;
}

void Player::emptyHand() {
    for (int i = 0; i < kindsOfBirds; i++){
        hand[i] = 0;
    }
}

//Print functions
void Player::printHand() {
    std::array<std::string, 8>  birdTypes = {"Flamingo    ", "Owl         ", "Toucan      ", "Duck        ", 
                                      "Parrot      ", "Magpie      ", "Reed Warbler", "Robin       "};
    std::cout << "Hand of player " << index << ": "<< std::endl;
    for (int i = 0; i < kindsOfBirds; i++){
        std::cout << i << ". " << birdTypes[i] << ": " << hand[i] << std::endl;
    }
    std::cout << std::endl;
}

void Player::printCollection() {
    std::array<std::string, 8>  birdTypes = {"Flamingo    ", "Owl         ", "Toucan      ", "Duck        ", 
                                      "Parrot      ", "Magpie      ", "Reed Warbler", "Robin       "};
    std::cout << "Collection of player " << index << ": " << std::endl;
    for (int i = 0; i < kindsOfBirds; i++){
        std::cout << i << ". " << birdTypes[i] << ": " << collection[i] << std::endl;
    }
    std::cout << std::endl;
}

//Get functions
std::array<int, 8>  Player::getCollection() {
    return collection;
}

std::array<int, 8>  Player::getHand() {
    return hand;
}

int Player::getIndex() {
    return index;
}

int Player::getHandSize(){
    int handSize = 0;
    for (int i = 0; i < kindsOfBirds; i++){
        handSize += hand[i];
    }
    return handSize;
}

float Player::getK() {
    return k;
}

float Player::getL() {
    return l;
}

float Player::getM() {
    return m;
}

float Player::getN() {
    return n;
}

float Player::getO() {
    return o;
}


//Set functions
void Player::setHand(std::array<int, 8>  newHand){
    hand = newHand;
}