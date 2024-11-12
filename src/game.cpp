#include "game.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <unordered_map>
#include <random>
#include <chrono>

Game::Game(int type1, int type2) {
    // Always initialize with 2 players
    players.push_back(Player(type1,1));
    players.push_back(Player(type2,2));
}

std::pair<int, int> Game::play() {
    int gameEnds;
    startGame();
    while (true) {
        turn++;
        for (Player& player : players) {
            auto start = std::chrono::high_resolution_clock::now();
            //Start the turn of a player, when this returns false the deck is empty and the game ends
            if (!startTurn(player)) return std::make_pair(0, endGame());
            // Check if a players' collection is complete, if it is that player wins and the game ends
            gameEnds = checkForWin(player);
            if (gameEnds!=0) return std::make_pair(gameEnds,player.getIndex());
            auto end = std::chrono::high_resolution_clock::now();
            if (player.getIndex() == 1) totalTimeP1 += end - start;
            else totalTimeP2 += end-start;
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

int Game::getTurn(){
    return turn;
}

std::chrono::duration<double> Game::getTimeP1(){
    return totalTimeP1;
}

std::chrono::duration<double> Game::getTimeP2(){
    return totalTimeP2;
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

int Game::checkForWin(Player& player) {
    std::vector<Card> collection;
    std::unordered_map<std::string, int> countMap;
    collection = player.getCollection();

    // Count occurrences of each item
    for (const auto& card: collection) {
        const std::string& birdType = card.getBirdType();
        countMap[birdType]++;
    }

    // Check for 7 different items
    if (countMap.size() >= 7) {
        return 1;  // 7 different things
    }

    // Check for two sets of 3 identical items
    int triplesCount = 0;
    for (const auto& pair : countMap) {
        if (pair.second >= 3) {
            triplesCount++;
        }
    }
    if (triplesCount == 2) {
        return 2; 
    }
    return 0;
}

bool Game::resolveTable(Player& player, int row, const Card& card) {
    //Give player the enclosed birds vector
    std::pair<std::vector<Card>, bool> result = table.resolveRow(card, row);
    //When the deck is empty the game ends
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

    if (type == 0) playRandomCards(player); 
    else playGreedyCards(player);
}

void Game::playFamily(Player& player) {
    int type = player.getType();

    if (type == 0) playRandomFamily(player);
    else if (type == 1) playGreedyFamily(player);
    else if (type == 2) playGreedyBigFamily(player);
    else if (type == 3) playTwoThree(player);
    else if (type == 4) playSeven(player);
}

void Game::playRandomFamily(Player& player) {
    int numberInHand = 0;
    bool familyPlayed = false; //only one family can be played in a turn, so stop turn when this variable is true
    player.shuffleHand(); //Make sure newly acquired cards have an equal chance of getting played
    std::vector<Card> hand = player.getHand();
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
            for (int j=0; j < numberInHand-2; j++){
                table.addCardToDiscard(card);
            }
            player.deleteType(card);
            familyPlayed = true;
        } else if (numberInHand >= card.getSmallFamily()){
            player.collectBird(card);
            for (int j=0; j < numberInHand-1; j++){
                table.addCardToDiscard(card);
            }
            player.deleteType(card);
            familyPlayed = true;
        }
        if (familyPlayed) break;
    }
}

void Game::playRandomCards(Player& player) {
    player.shuffleHand(); // Make sure the hand is shuffled to get a random card
    std::vector<Card> hand = player.getHand();
    bool cardsCollected = false;
    int numberOfType = 0;

    // Choose a card to play
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
        if (std::rand() % 2 == 0) {  // 50% chance to collect cards
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

void Game::playGreedyCards(Player& player){
    std::vector<Card> hand = player.getHand();
    int maxNumber = 0; // How many cards are maximally collected by playing a move
    int optimalRow = 0; // Row of the optimal play
    int optimalSide = 0; // Side of the optimal play
    int numberOfType = 0; // Number of cards of the family of the optimal play
    Card optimalType = Card("Empty",0,0,10);; // Card from the family of the optimal play
    bool cardsCollected = false;
    std::vector<Card> row;

    // Get a copy of every row of the board
    std::vector<Card> firstRow = table.getFirstRow();
    std::vector<Card> secondRow = table.getSecondRow();
    std::vector<Card> thirdRow = table.getThirdRow();
    std::vector<Card> fourthRow = table.getFourthRow();

    for (const Card& card : hand) {
        // First Row
        row = firstRow;
        row.insert(row.begin(), card);
        int enclosedBirds = birdsEnclosed(row, card);
        if (enclosedBirds > maxNumber) {
            maxNumber = enclosedBirds;
            optimalRow = 1;
            optimalSide = 0;  // Inserted at the front
            optimalType = card;
        }
        row = firstRow;
        row.push_back(card);
        enclosedBirds = birdsEnclosed(row, card);
        if (enclosedBirds > maxNumber) {
            maxNumber = enclosedBirds;
            optimalRow = 1;
            optimalSide = 1;  // Inserted at the back
            optimalType = card;
        }

        // Second Row
        row = secondRow;
        row.insert(row.begin(), card);
        enclosedBirds = birdsEnclosed(row, card);
        if (enclosedBirds > maxNumber) {
            maxNumber = enclosedBirds;
            optimalRow = 2;
            optimalSide = 0;  // Inserted at the front
            optimalType = card;
        }
        row = secondRow;
        row.push_back(card);
        enclosedBirds = birdsEnclosed(row, card);
        if (enclosedBirds > maxNumber) {
            maxNumber = enclosedBirds;
            optimalRow = 2;
            optimalSide = 1;  // Inserted at the back
            optimalType = card;
        }

        // Third Row
        row = thirdRow;
        row.insert(row.begin(), card);
        enclosedBirds = birdsEnclosed(row, card);
        if (enclosedBirds > maxNumber) {
            maxNumber = enclosedBirds;
            optimalRow = 3;
            optimalSide = 0;  // Inserted at the front
            optimalType = card;
        }
        row = thirdRow;
        row.push_back(card);
        enclosedBirds = birdsEnclosed(row, card);
        if (enclosedBirds > maxNumber) {
            maxNumber = enclosedBirds;
            optimalRow = 3;
            optimalSide = 1;  // Inserted at the back
            optimalType = card;
        }

        // Fourth Row
        row = fourthRow;
        row.insert(row.begin(), card);
        enclosedBirds = birdsEnclosed(row, card);
        if (enclosedBirds > maxNumber) {
            maxNumber = enclosedBirds;
            optimalRow = 4;
            optimalSide = 0;  // Inserted at the front
            optimalType = card;
        }
        row = fourthRow;
        row.push_back(card);
        enclosedBirds = birdsEnclosed(row, card);
        if (enclosedBirds > maxNumber) {
            maxNumber = enclosedBirds;
            optimalRow = 4;
            optimalSide = 1;  // Inserted at the back
            optimalType = card;
        }

    }
    if (maxNumber == 0) playRandomCards(player); //no birds can be enclosed
    else {
        for (const Card& card : hand) {
            if (card.getBirdType() == optimalType.getBirdType()) numberOfType++;
        }

        table.addCards(optimalType,optimalRow,optimalSide,numberOfType);
        if (resolveTable(player,optimalRow,optimalType)) cardsCollected = true;
        player.deleteType(optimalType);
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

void Game::playGreedyFamily(Player& player){
    int numberInHand = 0;
    bool familyPlayed = false; //only one family can be played in a turn, so stop turn when this variable is true
    std::vector<Card> hand = player.getHand();
    //Try to play the most valuable family first
    for (int i = 0; i<20; i++){
        //Check for every card in hand
        for (const Card& card : hand) {
            if (card.getNumberBirds() == i){
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
                    for (int j=0; j < numberInHand-2; j++){
                        table.addCardToDiscard(card);
                    }
                    player.deleteType(card);
                    familyPlayed = true;
                } else if (numberInHand >= card.getSmallFamily()){
                    player.collectBird(card);
                    for (int j=0; j < numberInHand-1; j++){
                        table.addCardToDiscard(card);
                    }
                    player.deleteType(card);
                    familyPlayed = true;
                }
            }
            if (familyPlayed) break;
        }
        if (familyPlayed) break;
    }
}

void Game::playGreedyBigFamily(Player& player){
    int numberInHand = 0;
    bool familyPlayed = false; //only one family can be played in a turn, so stop turn when this variable is true
    std::vector<Card> hand = player.getHand();
    //Try to play the most valuable family first
    for (int i = 20; i>0; i--){
        //Check for every card in hand
        for (const Card& card : hand) {
            if (card.getNumberBirds() == i){
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
                    for (int j=0; j < numberInHand-2; j++){
                        table.addCardToDiscard(card);
                    }
                    player.deleteType(card);
                    familyPlayed = true;
                } else if (numberInHand >= card.getSmallFamily()){
                    player.collectBird(card);
                    for (int j=0; j < numberInHand-1; j++){
                        table.addCardToDiscard(card);
                    }
                    player.deleteType(card);
                    familyPlayed = true;
                }
            }
            if (familyPlayed) break;
        }
        if (familyPlayed) break;
    }
}

void Game::playTwoThree (Player& player){
    int currentCollection[8] = {0};
    std::string currentBirdType;
    std::vector<Card> collection = player.getCollection();
    std::vector<Card> hand = player.getHand();
    int numberInHand = 0;
    bool familyPlayed = false;
    Card currentCard = Card("Empty",0,0,10);
    std::vector<std::string> birdTypes = {"Flamingo", "Owl", "Toucan", "Duck", 
                                      "Parrot", "Magpie", "Reed Warbler", "Robin"};
    // Get an overview of the current collection
    for (const Card& card : collection){
        if (card.getBirdType() == "Flamingo") currentCollection[0]++;
        else if (card.getBirdType() == "Owl") currentCollection[1]++;
        else if (card.getBirdType() == "Toucan") currentCollection[2]++;
        else if (card.getBirdType() == "Duck") currentCollection[3]++;
        else if (card.getBirdType() == "Parrot") currentCollection[4]++;
        else if (card.getBirdType() == "Magpie") currentCollection[5]++;
        else if (card.getBirdType() == "Reed Warbler") currentCollection[6]++;
        else if (card.getBirdType() == "Robin") currentCollection[7]++;
    }
    // First try to complete the collection by adding a big family
    for (int i = 0; i<8; i++){
        if (currentCollection[i] == 1){
            currentCard = Card("Empty",0,0,10);
            numberInHand = 0;
            currentBirdType = birdTypes[i];
            for (const Card& card : hand) {
                if (card.getBirdType() == currentBirdType) {
                    numberInHand++;
                    currentCard = card;
                }
            }
            if (numberInHand >= currentCard.getBigFamily() && currentCard.getBirdType()!= "Empty") {
                player.collectBird(currentCard);
                player.collectBird(currentCard);
                for (int j=0; j < numberInHand-2; j++){
                    table.addCardToDiscard(currentCard);
                }
                player.deleteType(currentCard);
                familyPlayed = true;
            }
            if (familyPlayed) break;
        }
    }

    // Then try to complete a collection by adding a small family
    if (!familyPlayed){
        for (int i = 0; i<8; i++){
            if (currentCollection[i] == 2) {
                currentCard = Card("Empty",0,0,10);
                numberInHand = 0;
                currentBirdType = birdTypes[i];
                for (const Card& card : hand) {
                    if (card.getBirdType() == currentBirdType) {
                        numberInHand++;
                        currentCard = card;
                    }
                }
                if (numberInHand >= currentCard.getSmallFamily() && currentCard.getBirdType()!= "Empty") {
                    player.collectBird(currentCard);
                    player.collectBird(currentCard);
                    for (int j=0; j < numberInHand-2; j++){
                        table.addCardToDiscard(currentCard);
                    }
                    player.deleteType(currentCard);
                    familyPlayed = true;
                }
            }
            if (familyPlayed) break;
        }
    }

    // Then try to add a card of a second kind to the collection if there is only 1
    if (!familyPlayed) {
        int numberSpecies = 0;
        // Check if there is more than one species in the collection
        for (int i = 0; i < 8; i++){
            if (currentCollection[i] != 0) numberSpecies++;
        }
        if (numberSpecies == 1){
            for (const Card& card : hand) {
                numberInHand = 0;
                if (card.getBirdType()!= collection.front().getBirdType()){
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
                        for (int j=0; j < numberInHand-2; j++){
                            table.addCardToDiscard(card);
                        }
                        player.deleteType(card);
                        familyPlayed = true;
                    } else if (numberInHand >= card.getSmallFamily()){
                        player.collectBird(card);
                        for (int j=0; j < numberInHand-1; j++){
                            table.addCardToDiscard(card);
                        }
                        player.deleteType(card);
                        familyPlayed = true;
                    }
                    if (familyPlayed) break;
                }
                if (familyPlayed) break;
            }
        }
    }
}

void Game::playSeven (Player& player){
    int currentCollection[8] = {0};
    std::string currentBirdType;
    std::vector<Card> collection = player.getCollection();
    std::vector<Card> hand = player.getHand();
    int numberInHand = 0;
    bool familyPlayed = false;
    Card currentCard = Card("Empty",0,0,10);
    std::vector<std::string> birdTypes = {"Flamingo", "Owl", "Toucan", "Duck", 
                                      "Parrot", "Magpie", "Reed Warbler", "Robin"};
    // Get an overview of the current collection
    for (const Card& card : collection){
        if (card.getBirdType() == "Flamingo") currentCollection[0]++;
        else if (card.getBirdType() == "Owl") currentCollection[1]++;
        else if (card.getBirdType() == "Toucan") currentCollection[2]++;
        else if (card.getBirdType() == "Duck") currentCollection[3]++;
        else if (card.getBirdType() == "Parrot") currentCollection[4]++;
        else if (card.getBirdType() == "Magpie") currentCollection[5]++;
        else if (card.getBirdType() == "Reed Warbler") currentCollection[6]++;
        else if (card.getBirdType() == "Robin") currentCollection[7]++;
    }
    // Try to add a bird to the collection that is not yet in the collection
    for (int i = 0; i<8; i++){
        if (currentCollection[i] == 0){
            currentCard = Card("Empty",0,0,10);
            numberInHand = 0;
            currentBirdType = birdTypes[i];
            for (const Card& card : hand) {
                if (card.getBirdType() == currentBirdType) {
                    numberInHand++;
                    currentCard = card;
                }
            }
            if (numberInHand >= currentCard.getBigFamily() && currentCard.getBirdType()!= "Empty"){
                player.collectBird(currentCard);
                player.collectBird(currentCard);
                for (int j=0; j < numberInHand-2; j++){
                    table.addCardToDiscard(currentCard);
                }
                player.deleteType(currentCard);
                familyPlayed = true;
            } else if (numberInHand >= currentCard.getSmallFamily()&& currentCard.getBirdType()!= "Empty"){
                player.collectBird(currentCard);
                for (int j=0; j < numberInHand-1; j++){
                    table.addCardToDiscard(currentCard);
                }
                player.deleteType(currentCard);
                familyPlayed = true;
            }
        }
        if (familyPlayed) break;
    }
}

int Game::birdsEnclosed(std::vector<Card> row, const Card& card){
    int enclosedBirds = 0;

    for (size_t i = 0; i < row.size(); ++i) {
        for (size_t j = i + 1; j < row.size(); ++j) {
            if (row[i].getBirdType() == row[j].getBirdType() && j > i + 1) {
                // Ensure that only cards of a different bird type are enclosed
                bool differentBirdsEnclosed = true;
                for (size_t k = i + 1; k < j; ++k) {
                    if (row[k].getBirdType() == row[i].getBirdType()) {
                        differentBirdsEnclosed = false;
                        break;
                    }
                }

                if (differentBirdsEnclosed) {
                    enclosedBirds = j - i - 1;
                    break;  // Exit the inner loop once matching cards are found and handled
                }
            }
        }
    }
    return enclosedBirds;
}