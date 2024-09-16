#include "game.h"
#include <iostream>
#include <unordered_map>
#include <random>

bool gameEnded = false;

Game::Game() {
    // Always initialize with 2 players
    players.push_back(Player("Player 1",0));
    players.push_back(Player("Player 2",0));
}

void Game::play() {
    startGame();
    while (!gameEnded) {
        table.printTable();
        for (Player& player : players) {
            if (gameEnded) break;
            player.printHand();
            player.printCollection();
            std::cout << std::endl;
            if (!startTurn(player)){
                //winner by empty deck
                endGame();
                break;
            }
            if (checkForWin(player)) {
                //winner by complete collection
                std::cout <<player.getName() << " wins!" << std::endl;
                gameEnded = true;  
                break;
            }
        }
    }
}

void Game::endGame() {
    gameEnded = true;
    Player* largestCollectionPlayer = &players.front();
    if (largestCollectionPlayer) {
        for (Player& player : players) {
            if (player.getCollection().size() > largestCollectionPlayer->getCollection().size()) {
                largestCollectionPlayer = &player;
            }
        }
        std::cout << "The deck is empty, " << largestCollectionPlayer->getName() << " wins!" << std::endl;
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
    playCards(player);
    if (!gameEnded){
        playFamily(player);

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
                for (int i = 0; i < 8; i++) {
                    player.drawCard(table.drawCard());
                }
            }
            if (!startTurn(player)){ //current player gets another turn
                return false;
            } 
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

bool Game::resolveTable(Player& player, int row, const Card& card) {
    //Give player the enclosed birds vector
    std::pair<std::vector<Card>, bool> result = table.resolveRow(card, row);
    if (result.second) {
        endGame();
        return true;
    }
    if (!result.first.empty()) {
        for (const Card& card : result.first) {
            player.drawCard(card);
        }
        return true;
    }
    return false;
}

void Game::playCards(Player& player) {
    int type = player.getType();
    std::vector<Card> hand = player.getHand();
    bool cardsCollected = false;
    if (type == 0){ //random player
        Card playingType = hand.front();
        for (const Card& card : hand) {
            if (card.getBirdType() == playingType.getBirdType()){
                std::random_device rd;
                std::mt19937 generator(rd());
                std::uniform_int_distribution<int> distribution1(1, 4);
                std::uniform_int_distribution<int> distribution2(0, 1);
                int row = distribution1(generator);
                int side = distribution2(generator);
                table.addCard(card,row,side);
                if (resolveTable(player,row,card)){
                    cardsCollected = true;
                }
            }
            if (gameEnded) break;
        }
        if (!gameEnded){
            player.deleteType(playingType);
            //draw 2 cards if no cards collected
            if (!cardsCollected) {
                Card newCard = table.drawCard();
                if (newCard.getBirdType() == "Empty") {
                    endGame();
                } else {
                    player.drawCard(newCard);
                    newCard = table.drawCard();
                    if (newCard.getBirdType() == "Empty") {
                        endGame();
                    } else {
                        player.drawCard(newCard);
                    } 
                }
            }
        }
    }
}

void Game::playFamily(Player& player) {
    int type = player.getType();
    int numberInHand = 0;
    bool familyPlayed = false;
    std::vector<Card> hand = player.getHand();
    if (type == 0){
        for (const Card& card : hand) {
            for (const Card& crd : hand) {
                if (card.getBirdType() == crd.getBirdType()) numberInHand++;
            }
            if (numberInHand >= card.getBigFamily()){
                player.collectBird(card);
                player.collectBird(card);
                for (int i = card.getBigFamily(); i > 0; i--){
                    player.discardCard(card);
                }
                familyPlayed = true;
            } else if (numberInHand >= card.getSmallFamily()){
                player.collectBird(card);
                for (int i = card.getSmallFamily(); i > 0; i--){
                    player.discardCard(card);
                }
                familyPlayed = true;
            }
            if (familyPlayed) break;
            numberInHand = 0;
        }
    }
}
