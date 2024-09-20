#include "game.h"
#include <iostream>
#include <unordered_map>
#include <random>

Game::Game(int type1, int type2) {
    // Always initialize with 2 players
    players.push_back(Player(type1,1));
    players.push_back(Player(type2,2));
}

int Game::play() {
    startGame();
    while (true) {
        for (Player& player : players) {
            //Start the turn of a player, when this returns false the deck is empty and the game ends
            if (!startTurn(player)) return endGame();
            // Check if a players' collection is complete, if it is that player wins and the game ends
            if (checkForWin(player)) return player.getIndex();
        }
    }
}

//Find the player with the largest collection and return the index of this player, if there is a tie return 0
int Game::endGame() {
    // Assume there are at least two players
    Player* largestCollectionPlayer = &players.front();
    bool isTie = true;  // Variable to track if there's a tie

    for (Player& player : players) {
        if (player.getCollection().size() > largestCollectionPlayer->getCollection().size()) {
            largestCollectionPlayer = &player;
            isTie = false;  // Set to false if we find a larger collection
        } else if (player.getCollection().size() != largestCollectionPlayer->getCollection().size()) {
            isTie = false;  // If collections differ, it's not a tie
        }
    }

    // If it's a tie, return 0
    if (isTie) {
        return 0;
    }

    // Otherwise, return the index of the player with the largest collection
    return largestCollectionPlayer->getIndex();
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
        //When all three cards are different add them to the row
        table.addCards(firstCard, row,1,1);
        table.addCards(secondCard, row,1,1);
        table.addCards(thirdCard, row,1,1);
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
            // If the drawpile and discardpile combined have not enough cards the game ends
            if (table.getDrawSize() + table.getDiscardSize() < cardsRequired){
                return false;
            }
            // Else we can reshuffle and then deal new cards
            table.reshuffleFromDiscardPile();
        }
        //Every player gets 8 new cards
        for (Player& player : players) {
            for (int i = 0; i < 8; i++) {
                player.drawCard(table.drawCard());
            }
        }
        if (!startTurn(player)) return false; //current player gets another turn
    }
    // If the turn is completed and the deck is not empty return true
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
    //When the deck is empty the game ends
    if (result.second) {
        endGame();
        return false;
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
    int numberOfType = 0;

    if (type == 0){
        Card playingType = hand.front();
        for (const Card& card : hand) {
            if (card.getBirdType() == playingType.getBirdType()) numberOfType++;
        }
        // Generate a random row and side to play the cards
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<int> distribution1(1, 4);
        std::uniform_int_distribution<int> distribution2(0, 1);
        int row = distribution1(generator);
        int side = distribution2(generator);

        table.addCards(playingType,row,side,numberOfType);
        if (resolveTable(player,row,playingType)) cardsCollected = true;
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

void Game::playFamily(Player& player) {
    int type = player.getType();
    int numberInHand = 0;
    bool familyPlayed = false; //only one family can be played in a turn, so stop turn when this variable is true
    std::vector<Card> hand = player.getHand();

    if (type == 0){
        //Check for every card in hand
        for (const Card& card : hand) {
            numberInHand = 0;
            //How many of that card the player has
            for (const Card& crd : hand) {
                if (card.getBirdType() == crd.getBirdType()) numberInHand++;
            }

            //If that number is enough for a big family, play this family
            //Else if that number is enough for a small family, play this family
            //Else do nothing
            if (numberInHand >= card.getBigFamily()){
                player.collectBird(card);
                player.collectBird(card);
                for (int i=0; i < numberInHand-2; i++){
                    table.addCardToDiscard(card);
                }
                player.deleteType(card);
                familyPlayed = true;
            } else if (numberInHand >= card.getSmallFamily()){
                player.collectBird(card);
                for (int i=0; i < numberInHand-1; i++){
                    table.addCardToDiscard(card);
                }
                player.deleteType(card);
                familyPlayed = true;
            }
            if (familyPlayed) break;
        }
    }
}
