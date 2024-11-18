#include "game.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <unordered_map>
#include <random>
#include <chrono>
#include <cmath>

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
    if (players[0].getCollection().size() == players[1].getCollection().size()) return 0;
    else if (players[0].getCollection().size() >= players[1].getCollection().size()) return players[0].getIndex();
    else return players[1].getIndex();
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
    else playScoredCards(player, false);
}

void Game::playFamily(Player& player) {
    int type = player.getType();

    if (type == 0) playRandomFamily(player);
    else if (type == 1) playGreedyFamily(player);
    else if (type == 2) playGreedyBigFamily(player);
    else if (type > 99) {
        if ((type-100)/2 <= turn) {
            if (type % 2 == 0) playTwoThree(player);
            else playSeven(player);
        } else playRandomFamily(player);
    }
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

float Game::scoreTwoThreeCards(Player& player, std::vector<Card> enclosedBirds, bool test){
    int k = 1;
    int l = 1;
    int m = 200;
    float totalScore = 0;
    int currentCollection[9] = {0};
    int numberSpecies = 0;
    std::vector<Card> collection = player.getCollection();
    for (const Card& card : collection){
            currentCollection[card.getIndex()]++;
    }

    // Check if there is more than one species in the collection
    for (int i = 0; i < 8; i++){
        if (currentCollection[i] != 0) numberSpecies++;
    }
    
    //Score on how much the cards are directly needed for the collection (0-20)
    int numberCards = 0;
    int currentBoard[9] = {0};
    if (numberSpecies == 1){
        for (const auto& vector : {table.getFirstRow(), table.getSecondRow(), table.getThirdRow(), table.getFourthRow()}) {
            for (const auto& card : vector) {
                numberCards++;
                currentBoard[card.getIndex()]++;
            }
        }
    }

    for (const Card& card : enclosedBirds){
        if (currentCollection[card.getIndex()] == 0 && numberSpecies == 1) {
            // Scale the score of the card by how many cards are on the board
            totalScore += k * (5 + (10 * (currentBoard[card.getIndex()] / numberCards))); //score from 5-15
        } else if (currentCollection[card.getIndex()] == 1) totalScore += k*15;
        else if (currentCollection[card.getIndex()] == 2) totalScore += k*20;
    }

    //Score on how much you may need the cards on the next turn
    if (!test){
        std::vector<Card> oldHand = player.getHand();
        std::vector<Card> newHand = oldHand;
        newHand.insert(newHand.end(), enclosedBirds.begin(), enclosedBirds.end());
        player.changeHand(newHand);
        totalScore += l * playScoredCards(player, true);
        player.changeHand(oldHand);
    }

    //Scale by the rarity of the cards (10-28)
    for (const Card& card : enclosedBirds) {
        totalScore+= m/card.getNumberBirds();
    }
    return totalScore;
}

float Game::scoreSevenCards(Player& player, std::vector<Card> enclosedBirds, bool test){
    int k = 1;
    int l = 1;
    int m = 200;
    float totalScore = 0;
    int currentCollection[9] = {0};
    std::vector<Card> collection = player.getCollection();
    for (const Card& card : collection){
            currentCollection[card.getIndex()]++;
    }

    //Score on how much the cards are directly needed for the collection
    int numberCards = 0;
    int currentBoard[9] = {0};
    for (const auto& vector : {table.getFirstRow(), table.getSecondRow(), table.getThirdRow(), table.getFourthRow()}) {
        for (const auto& card : vector) {
            numberCards++;
            currentBoard[card.getIndex()]++;
        }
    }

    for (const Card& card : enclosedBirds){
        if (currentCollection[card.getIndex()] == 0) {
            // Scale the score of the card by how many cards are on the board
            totalScore += k * (20 - (10 * (currentBoard[card.getIndex()] / numberCards))); //score from 10-20
        }
    }

    //Score on how much you may need the cards in the future
    if (!test){
        std::vector<Card> oldHand = player.getHand();
        std::vector<Card> newHand = oldHand;
        newHand.insert(newHand.end(), enclosedBirds.begin(), enclosedBirds.end());
        player.changeHand(newHand);
        totalScore += l * playScoredCards(player, true);
        player.changeHand(oldHand);
    }

    //Scale by the rarity of the cards
    for (const Card& card : enclosedBirds) {
        totalScore+= m/card.getNumberBirds();
    }
    return totalScore;
}

float Game::scoreGreedyCards(std::vector<Card> enclosedBirds) {
    float totalScore = 0;
    const int k = 200;

    for (const Card& card : enclosedBirds) {
        totalScore+= k/card.getNumberBirds();
    }
    return totalScore;
}

float Game::scoreCards(Player& player, std::vector<Card> enclosedBirds, bool test) {
    int type = player.getType();
    if (type < 99 || (type-100)/2 > turn) {
        return scoreGreedyCards(enclosedBirds);
    } else {
        int type = player.getType();
        float totalScore = 0;
        float opponentScore = 0;

        //Scaling factors
        int k = 1;
        int l = 0.1;

        // First score cards on how much you need them
        if (type % 2 == 0) {
            totalScore += k*scoreTwoThreeCards(player, enclosedBirds, test);
        } else {
            totalScore += k*scoreSevenCards(player, enclosedBirds, test);
        }

        // Secondly score cards on how much you think your opponent needs the cards
        for (Player& plr : players) {
            if (plr.getIndex() != player.getIndex()) {
                opponentScore += (scoreTwoThreeCards(plr, enclosedBirds, false) + scoreSevenCards(plr, enclosedBirds, false)/2);
            }
        }
        totalScore += l*opponentScore;

        return totalScore;
    }
}

float Game::playScoredCards(Player& player, bool test){
    std::vector<Card> hand = player.getHand();
    float maxScore = 0; // How many cards are maximally collected by playing a move
    int optimalRow = 0; // Row of the optimal play
    int optimalSide = 0; // Side of the optimal play
    int numberOfType = 0; // Number of cards of the family of the optimal play
    Card optimalType = Card("Empty",0,0,10,0);; // Card from the family of the optimal play
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
        float score = scoreCards(player, birdsEnclosed(row,card), test);
        if (score > maxScore) {
            maxScore = score;
            optimalRow = 1;
            optimalSide = 0;  // Inserted at the front
            optimalType = card;
        }
        row = firstRow;
        row.push_back(card);
        score = scoreCards(player, birdsEnclosed(row,card), test);
        if (score > maxScore) {
            maxScore = score;
            optimalRow = 1;
            optimalSide = 1;  // Inserted at the back
            optimalType = card;
        }

        // Second Row
        row = secondRow;
        row.insert(row.begin(), card);
        score = scoreCards(player, birdsEnclosed(row,card), test);
        if (score > maxScore) {
            maxScore = score;
            optimalRow = 2;
            optimalSide = 0;  // Inserted at the front
            optimalType = card;
        }
        row = secondRow;
        row.push_back(card);
        score = scoreCards(player, birdsEnclosed(row,card), test);
        if (score > maxScore) {
            maxScore = score;
            optimalRow = 2;
            optimalSide = 1;  // Inserted at the back
            optimalType = card;
        }

        // Third Row
        row = thirdRow;
        row.insert(row.begin(), card);
        score = scoreCards(player, birdsEnclosed(row,card), test);
        if (score > maxScore) {
            maxScore = score;
            optimalRow = 3;
            optimalSide = 0;  // Inserted at the front
            optimalType = card;
        }
        row = thirdRow;
        row.push_back(card);
        score = scoreCards(player, birdsEnclosed(row,card), test);
        if (score > maxScore) {
            maxScore = score;
            optimalRow = 3;
            optimalSide = 1;  // Inserted at the back
            optimalType = card;
        }

        // Fourth Row
        row = fourthRow;
        row.insert(row.begin(), card);
        score = scoreCards(player, birdsEnclosed(row,card), test);
        if (score > maxScore) {
            maxScore = score;
            optimalRow = 4;
            optimalSide = 0;  // Inserted at the front
            optimalType = card;
        }
        row = fourthRow;
        row.push_back(card);
        score = scoreCards(player, birdsEnclosed(row,card), test);
        if (score > maxScore) {
            maxScore = score;
            optimalRow = 4;
            optimalSide = 1;  // Inserted at the back
            optimalType = card;
        }

    }
    if (test) return maxScore;
    if (maxScore == 0) playRandomCards(player); //no birds can be enclosed
    else {
        for (const Card& card : hand) {
            if (card.getBirdType() == optimalType.getBirdType()) numberOfType++;
        }

        table.addCards(optimalType,optimalRow,optimalSide,numberOfType);
        if (resolveTable(player,optimalRow,optimalType)) cardsCollected = true;
        player.deleteType(optimalType);
        //draw 2 cards if no cards collected and hand is not empty
        if (!cardsCollected && player.getHand().size() != 0) {
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
    return 0;
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
    int currentCollection[9] = {0};
    std::string currentBirdType;
    std::vector<Card> collection = player.getCollection();
    std::vector<Card> hand = player.getHand();
    int numberInHand = 0;
    bool familyPlayed = false;
    Card currentCard = Card("Empty",0,0,10,0);
    std::vector<std::string> birdTypes = {"Flamingo", "Owl", "Toucan", "Duck", 
                                      "Parrot", "Magpie", "Reed Warbler", "Robin"};
    // Get an overview of the current collection
    // Get an overview of the current collection
    for (const Card& card : collection){
        currentCollection[card.getIndex()]++;
    }
    // First try to complete the collection by adding a big family
    for (int i = 0; i<8; i++){
        if (currentCollection[i] == 1){
            currentCard = Card("Empty",0,0,10,0);
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
                currentCard = Card("Empty",0,0,10,0);
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
    int currentCollection[9] = {0};
    std::string currentBirdType;
    std::vector<Card> collection = player.getCollection();
    std::vector<Card> hand = player.getHand();
    int numberInHand = 0;
    bool familyPlayed = false;
    Card currentCard = Card("Empty",0,0,10,0);
    std::vector<std::string> birdTypes = {"Flamingo", "Owl", "Toucan", "Duck", 
                                      "Parrot", "Magpie", "Reed Warbler", "Robin"};
    // Get an overview of the current collection
    for (const Card& card : collection){
        currentCollection[card.getIndex()]++;
    }

    // Try to add a bird to the collection that is not yet in the collection
    for (int i = 0; i<8; i++){
        if (currentCollection[i] == 0){
            currentCard = Card("Empty",0,0,10,0);
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

std::vector<Card> Game::birdsEnclosed(std::vector<Card> row, const Card& card) {
    std::vector<Card> enclosedBirds;

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
                    // Collect all cards enclosed between the matching bird cards
                    enclosedBirds.insert(enclosedBirds.end(), row.begin() + i + 1, row.begin() + j);
                    return enclosedBirds;  // Return immediately after finding the enclosed birds
                }
            }
        }
    }

    return enclosedBirds;  // Return an empty vector if no enclosed birds are found
}
