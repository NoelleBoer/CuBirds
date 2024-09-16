#include "game.h"
#include <iostream>
#include <unordered_map>

Game::Game() {
    // Always initialize with 2 players
    players.push_back(Player("Player 1",0));
    players.push_back(Player("Player 2",0));
}

void Game::play() {
    startGame();
    bool gameEnded = false;
    Player* largestCollectionPlayer = nullptr;
    while (!gameEnded) {
        for (Player& player : players) {
            if (!startTurn(player)){
                //winner by empty deck
                gameEnded = true;
                largestCollectionPlayer = &player;
                break;
            }
            if (checkForWin(player)) {
                //winner by complete collection
                gameEnded = true;  
                std::cout <<player.getName() << "won!";
                break;
            }
        }
    }
    if (largestCollectionPlayer) {
        for (Player& player : players) {
            if (player.getCollection().size() > largestCollectionPlayer->getCollection().size()) {
                largestCollectionPlayer = &player;
            }
        }
        std::cout << largestCollectionPlayer->getName() << " won!" << std::endl;
    } 
}

void Game::startGame() {
    table.initializeDrawPile();
    // start with 3 different cards in each row
    Card firstCard, secondCard, thirdCard;
    for (int row = 1; row < 5; row++) {
        firstCard = table.drawCard();
        secondCard = table.drawCard();

        //make sure the second card is different from the first card
        while (firstCard.getBirdType() == secondCard.getBirdType()){
            table.addCardToDiscard(secondCard);
            secondCard = table.drawCard();
        }
        thirdCard = table.drawCard();
        //make sure the third card is different from first and second card
        while ((firstCard.getBirdType() == thirdCard.getBirdType()) || (secondCard.getBirdType() == thirdCard.getBirdType())){
            table.addCardToDiscard(thirdCard);
            thirdCard = table.drawCard();
        }
        table.addCard(firstCard, row,1);
        table.addCard(secondCard, row,1);
        table.addCard(thirdCard, row,1);
    }
    //every player draws 8 cards
    for (Player& player : players) {
            for (int i = 0; i < 8; i++){
                player.drawCard(table.drawCard());
        }
    }
    //every player gets a starting card in their collection
    for (Player& player : players) {
        player.collectBird(table.drawCard());
    }
}

bool Game::startTurn(Player& player) {
    player.playCard();
    player.playFamily();

    //Check if the hand of the player is empty
    if (player.getHand().size() == 0) {
        //the hand of every player goes to discard pile
        for (Player& player : players) {
            for (const Card& card : player.getHand()) {
                table.addCardToDiscard(card);
            }
            player.emptyHand();
        }
        //Check if there are enough cards to give players a new hand
        int cardsRequired = players.size() * 8; 
        if (table.getDrawSize() < cardsRequired) {
            if (table.getDrawSize() + table.getDiscardSize() < cardsRequired){
                return false;
            }
            table.reshuffleFromDiscardPile();
        }
        //Every player gets 8 new cards
        for (Player& player : players) {
            for (int i = 0; i <8; i++) {
                player.drawCard(table.drawCard());
            }
        }
        if (!startTurn(player)){ //current player gets another turn
            return false;
        } 
    }
    return true;
}

bool Game::checkForWin(Player& player) {
    std::vector<Card> collection;
    std::unordered_map<std::string, int> countMap;
    collection = player.getCollection();
    // Count occurrences of each item
    for (const auto& card: collection) {
        const std::string& birdType = card.getBirdType();
        countMap[birdType]++;
    }
    // Check for 7 different items
    if (countMap.size() == 7) {
        return true;  // 7 different things
    }
    // Check for two sets of 3 identical items
    int triplesCount = 0;
    for (const auto& pair : countMap) {
        if (pair.second == 3) {
            triplesCount++;
        }
    }
    if (triplesCount == 2) {
        return true; 
    }
    return false;
}

bool Game::handleEmptyDeck(const Card& card) {
    if (card.getBirdType() == "Empty") {
        table.reshuffleFromDiscardPile();
        return true;
    } 
    return false;
}

void Game::resolveTable(Player& player) {
    //Give player the collected cards
    //If no cards collected player may draw 2
    //If only one kind of bird on a row add cards till there are 2 kinds of birds
}
    