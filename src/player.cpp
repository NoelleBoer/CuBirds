#include "player.h"

Player::Player(int index) 
    : hand{}, collection{}, index(index), k(0.5),l(0.5),m(0.3),n(0.5),o(1.8), p(0.5){}


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

void Player::printHand() {
    std::cout << "Hand of player " << index << ": "<< std::endl;
    for (int i = 0; i < kindsOfBirds; i++){
        std::cout << i << ". " << birdTypes[i] << ": " << hand[i] << std::endl;
    }
    std::cout << std::endl;
}

void Player::printCollection() {
    std::cout << "Collection of player " << index << ": " << std::endl;
    for (int i = 0; i < kindsOfBirds; i++){
        std::cout << i << ". " << birdTypes[i] << ": " << collection[i] << std::endl;
    }
    std::cout << std::endl;
}

std::array<int, kindsOfBirds>  Player::getCollection() {
    return collection;
}

std::array<int, kindsOfBirds>  Player::getHand() {
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

float Player::getP() {
    return o;
}

void Player::setHand(std::array<int, kindsOfBirds>  newHand){
    hand = newHand;
}

void Player::setVariables(float vark, float varl, float varm, float varn, float varo, float varp){
    k = vark;
    l = varl;
    m = varm;
    n = varn;
    o = varo;
    p = varp;
}

void Player::setCollection(std::array<int,kindsOfBirds> newCollection) {
    collection = newCollection;
}
