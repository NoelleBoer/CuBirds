#include "game.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <unordered_map>
#include <random>
#include <chrono>
#include <cmath>
#include <array>

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
            // player.printHand();
            // player.printCollection();
            // table.printTable();
            // table.printDrawPile();
            if (table.drawPileEmpty()) return std::make_pair(0, endGame());
            
            auto start = std::chrono::high_resolution_clock::now();

            startTurn(player);

            // Check if a players' collection is complete, if it is that player wins and the game ends
            gameEnds = checkForWin(player);
            if (gameEnds!=0) return std::make_pair(gameEnds,player.getIndex());
            auto end = std::chrono::high_resolution_clock::now();
            if (player.getIndex() == 1) totalTimeP1 += end - start;
            else totalTimeP2 += end-start;
            // player.printHand();
            // player.printCollection();
            // table.printTable();
        }
    }
}

//Find the player with the largest collection and return the index of this player, if there is a tie return 0
int Game::endGame() {
    int numberInCollectionP1 = 0;
     int numberInCollectionP2 = 0;

    for (int i = 0; i < 8; i++) {
        numberInCollectionP1 += players[0].getCollection()[i];
        numberInCollectionP2 += players[1].getCollection()[i];
    }
    if (numberInCollectionP1 == numberInCollectionP2) return 0;
    else if (numberInCollectionP1 >= numberInCollectionP2) return players[0].getIndex();
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
    table.setDrawPile({7,10,10,13,13,17,20,20});
    std::vector<int> row;
    // start with 3 different cards in each row
    int firstCard, secondCard, thirdCard;
    for (int i = 0; i < 4; i++) {
        row = {};
        firstCard = table.drawCard();
        secondCard = table.drawCard();

        //make sure the second card is different from the first card
        while (firstCard == secondCard){
            table.addCardToDiscard(secondCard);
            secondCard = table.drawCard();
        }
        thirdCard = table.drawCard();

        //make sure the third card is different from first and second card
        while ((firstCard == thirdCard) || (secondCard == thirdCard)){
            table.addCardToDiscard(thirdCard);
            thirdCard = table.drawCard();
        }
        row.push_back(firstCard); 
        row.push_back(secondCard); 
        row.push_back(thirdCard); 
        //When all three cards are different add them to the row
        table.setRow(row, i);
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
    table.reshuffleFromDiscardPile(); // start with an empty discard pile
}

void Game::startTurn(Player& player) {
    playCards(player);
    if(!table.drawPileEmpty())playFamily(player);
    //Check if the hand of the player is empty
    if (player.getHandSize() == 0) {
        //the hand of every player goes to discard pile
        for (Player& player : players) {
            for (int card : player.getHand()){
                table.addCardToDiscard(card);
            }
            player.emptyHand();
        }
        
        //Every player gets 8 new cards
        for (Player& player : players) {
            for (int i = 0; i < 8; i++) {
                if (!table.drawPileEmpty()) player.drawCard(table.drawCard());
                else break;
            }
        }
        if (!table.drawPileEmpty()) startTurn(player);//current player gets another turn
    }
}

int Game::checkForWin(Player& player) {
    int numberSpecies = 0;
    int numberTriplets = 0;

    for (int i = 0; i < 8; i++) {
        if (player.getCollection()[i] > 2) numberTriplets++;
        if (player.getCollection()[i] > 0) numberSpecies++;
    }

    if (numberSpecies >= 7) return 1;
    if (numberTriplets >= 2) return 2;
    return 0;
}

bool Game::resolveTable(Player& player, int row, int id, int side, int numberCards) {
    //Give player the enclosed birds vector
    std::vector<int> result = table.resolveRow(id, row, side, numberCards);
    //When the deck is empty the game ends
    if (!result.empty()) {
        for (const int card : result) {
            if (!table.drawPileEmpty()) player.drawCard(card);
        }
        return true;
    }
    return false;
}

void Game::playCards(Player& player) {
    int type = player.getType();

    if (type == 0) playRandomCards(player);
    else if (type > 99) {
        if ((type-100)/2 <= turn) playScoredCards(player,false);
        else playRandomCards(player);
    } 
    else playScoredCards(player, false);
}

void Game::playFamily(Player& player) {
    int type = player.getType();

    if (type == 0 || type == 1) playGreedyFamily(player);
    else if (type > 99) {
        if ((type-100)/2 <= turn) {
            if (type % 2 == 0) playTwoThree(player);
            else playSeven(player);
        } else playGreedyFamily(player);
    }
}

void Game::playRandomCards(Player& player) {
    bool cardsCollected = false;
    int totalWeight = 0;
    int playingType = 0;
    for (int weight : player.getHand()) {
        totalWeight += weight;
    }

    // Choose a random number between 1 and totalWeight
    std::random_device rd;  // Seed
    std::mt19937 gen(rd()); // Mersenne Twister RNG
    std::uniform_int_distribution<int> dis(1, totalWeight);
    int randomCard = dis(gen);

    // Map the random number to a card ID based on weights
    int cumulativeSum = 0;
    for (size_t i = 0; i < 8; i++) {
        cumulativeSum += player.getHand()[i];
        if (randomCard <= cumulativeSum) {
            playingType = i; // Select the card ID
            break;
        }
    }

    // Generate a random row and side to play the cards
    std::uniform_int_distribution<int> distribution1(0, 3);
    std::uniform_int_distribution<int> distribution2(0, 1);
    int row = distribution1(gen);
    int side = distribution2(gen);
    table.addCards(playingType,row,side,player.getHand()[playingType]);
    if (resolveTable(player,row,playingType, side, player.getHand()[playingType])) cardsCollected = true;
    player.deleteType(playingType);
    //draw 2 cards if no cards collected 50% of the time
    if (!cardsCollected && std::rand() % 2 == 0) {
        if (!table.drawPileEmpty()) player.drawCard(table.drawCard());
        if (!table.drawPileEmpty()) player.drawCard(table.drawCard());
    }
}

float Game::scoreTwoThreeCards(Player& player, std::vector<int> enclosedBirds, bool test){
    float totalScore = 0;
    int numberCards = 0;
    int numberSpecies = 0;
    std::array<int, 8> currentBoard = {};

    for (int i = 0; i < 4; i++){
        std::vector<int> row = table.getRow(i);
        for (int card : row) {
            numberCards++;
            currentBoard[card]++;
        }
    }

    // Check if there is more than one species in the collection
    for (int i = 0; i < 8; i++){
        if (player.getCollection()[i] != 0) numberSpecies++;
    }

    for (const int card : enclosedBirds){
        if (player.getCollection()[card] == 0 && numberSpecies == 1) {
            // Scale the score of the card by how many cards are on the board
            totalScore += (5 + (10 * (currentBoard[card] / numberCards))); //score from 5-15
        } else if (player.getCollection()[card] == 1) totalScore += 15;
        else if (player.getCollection()[card] == 2) totalScore += 20;
    }
    return totalScore;
}

float Game::scoreSevenCards(Player& player, std::vector<int> enclosedBirds, bool test){
    float totalScore = 0;
    int numberCards = 0;
    std::array<int, 8> currentBoard = {0};

    for (int i = 0; i < 4; i++){
        std::vector<int> row = table.getRow(i);
        for (int card : row) {
            numberCards++;
            currentBoard[card]++;
        }
    }

    for (int card : enclosedBirds){
        if (player.getCollection()[card] == 0) {
            // Scale the score of the card by how many cards are on the board, more cards lesser score
            if (numberCards == 0) numberCards =1;
            totalScore += 20 - (10 * (currentBoard[card] / numberCards)); //score from 10-20
        }
    }
    return totalScore;
}

float Game::scoreGreedyCards(std::vector<int> enclosedBirds) {
    float totalScore = 0;
    const int k = 200; // Scaling factor

    for (const int card : enclosedBirds) {
        totalScore+= k/numberBirds[card];
    }
    return totalScore;
}

float Game::scoreCards(Player& player, std::vector<int> enclosedBirds, bool test) {
    if (enclosedBirds.empty()) return 0;
    int type = player.getType();
    if (type < 99 || (type-100)/2 > turn) return scoreGreedyCards(enclosedBirds);

    float totalScore = 0;
    float opponentScore = 0;
    int k = 1;
    int l = 1;
    int m = 0.1; //Scaling factor: opponent collection score

    // First score cards on own players collection
    if (type % 2 == 0) {
        totalScore += k*scoreTwoThreeCards(player, enclosedBirds, test);
    } else {
        totalScore += k*scoreSevenCards(player, enclosedBirds, test);
    }

    //Score on how much the player needs the new cards in the future
    if (!test){
        std::array<int, 8>  oldHand = player.getHand();
        std::array<int, 8>  newHand = oldHand;
        for (int card : enclosedBirds){
            newHand[card]++;
        }
        player.changeHand(newHand);
        totalScore += l * playScoredCards(player, true);
        player.changeHand(oldHand);
    }

    //Scale by the rarity of the cards
    for (const int card : enclosedBirds) {
        totalScore+= m/numberBirds[card];
    }

    //Secondly score cards on how much you think your opponent needs the cards
    for (Player& plr : players) {
        if (plr.getIndex() != player.getIndex()) {
            opponentScore += (scoreTwoThreeCards(plr, enclosedBirds, true) + scoreSevenCards(plr, enclosedBirds, true))/2;
        }
    }
    totalScore += m*opponentScore;

    return totalScore;
}

float Game::playScoredCards(Player& player, bool test){
    float maxScore = 0;
    int optimalRow = 0; // Row of the optimal play
    int optimalSide = 0; // Side of the optimal play
    int optimalType = 0; // Card from the family of the optimal play
    bool cardsCollected = false;
    float score = 0;
    std::vector<int> row;
    std::vector<int> enclosedBirds;
    
    for (int i = 0; i < 8; i++) {
        if (player.getHand()[i] != 0) {
            for (int j = 0; j < 4; j++) {
                // First score by inserting at front
                row = table.getRow(j);
                row.insert(row.begin(), i);
                enclosedBirds = birdsEnclosed(row, i);
                score = scoreCards(player, enclosedBirds, test);
                if (score > maxScore) {
                    maxScore = score;
                    optimalRow = j;
                    optimalSide = 0;
                    optimalType = i;
                }
                // Second score by inserting at the back
                row = table.getRow(j);
                row.push_back(i);
                enclosedBirds = birdsEnclosed(row, i);
                score = scoreCards(player, enclosedBirds, test);
                if (score > maxScore) {
                    maxScore = score;
                    optimalRow = j;
                    optimalSide = 1;
                    optimalType = i;
                }
            }
        }
    }

    if (test) return maxScore;

    // Do the maximal scored play
    if (maxScore == 0) {
        playRandomCards(player); //No birds can be enclosed
        return 0;
    }
    table.addCards(optimalType,optimalRow,optimalSide,player.getHand()[optimalType]);
    if (resolveTable(player,optimalRow,optimalType, optimalSide, player.getHand()[optimalType])) cardsCollected = true;
    player.deleteType(optimalType);

    //draw 2 cards if no cards collected and hand is not empty
    if (!cardsCollected && player.getHandSize() != 0) {
        if (!table.drawPileEmpty()) player.drawCard(table.drawCard());
        if (!table.drawPileEmpty()) player.drawCard(table.drawCard());
    }
    return 0;
}

void Game::playGreedyFamily(Player& player){
    //Check for every card in hand, check rarest card first
    for (int i = 0; i < 8; i++) {
        if (player.getHand()[i] >= bigFam[i]){
            player.collectBird(i);
            player.collectBird(i);
            for (int j=0; j < player.getHand()[i]-2; j++){
                table.addCardToDiscard(i);
            }
            player.deleteType(i);
            return;
        } else if (player.getHand()[i] >= smallFam[i]){
            player.collectBird(i);
            for (int j=0; j < player.getHand()[i]-1; j++){
                table.addCardToDiscard(i);
            }
            player.deleteType(i);
            return;
        }
    }
}

void Game::playTwoThree (Player& player){
    // First try to complete the collection by adding a big family
    for (int i = 0; i<8; i++){
        if (player.getCollection()[i] == 1){
            if (player.getHand()[i] >= bigFam[i]) {
                player.collectBird(i);
                player.collectBird(i);
                for (int j=0; j < player.getHand()[i]-2; j++){
                    table.addCardToDiscard(i);
                }
                player.deleteType(i);
                return;
            }
        }
    }

    // Then try to complete a collection by adding a small family
    for (int i = 0; i<8; i++){
        if (player.getCollection()[i] == 1){
            if (player.getHand()[i] >= smallFam[i]) {
                player.collectBird(i);
                for (int j=0; j < player.getHand()[i]-1; j++){
                    table.addCardToDiscard(i);
                }
                player.deleteType(i);
                return;
            }
        }
    }

    // Then try to add a card of a second kind to the collection if there is only 1
    int numberSpecies = 0;
    int speciesId = 0;
    // Check if there is more than one species in the collection
    for (int i = 0; i < 8; i++){
        if (player.getCollection()[i] != 0) {
            numberSpecies++;
            speciesId = i;
        }
    }
    if (numberSpecies == 1){
        for (int i = 0; i < 8; i++) {
            if (i!= speciesId){
                if (player.getHand()[i] >= bigFam[i]){
                    player.collectBird(i);
                    player.collectBird(i);
                    for (int j=0; j < player.getHand()[i]-2; j++){
                        table.addCardToDiscard(i);
                    }
                    player.deleteType(i);
                    return;
                } else if (player.getHand()[i] >= smallFam[i]){
                    player.collectBird(i);
                    for (int j=0; j < player.getHand()[i]-1; j++){
                        table.addCardToDiscard(i);
                    }
                    player.deleteType(i);
                    return;
                }
            }
        }
    }
}

void Game::playSeven (Player& player){
    // Try to add a bird to the collection that is not yet in the collection
    for (int i = 0; i < 8; i++){
        if (player.getCollection()[i] == 0){
            if (player.getHand()[i] >= bigFam[i]){
                player.collectBird(i);
                player.collectBird(i);
                for (int j=0; j < player.getHand()[i]-2; j++){
                    table.addCardToDiscard(i);
                }
                player.deleteType(i);
                return;
            } else if (player.getHand()[i] >= smallFam[i]){
                player.collectBird(i);
                for (int j=0; j < player.getHand()[i]-1; j++){
                    table.addCardToDiscard(i);
                }
                player.deleteType(i);
                return;
            }
        }
    }
}

std::vector<int> Game::birdsEnclosed(std::vector<int> row, int id) {
    std::vector<int> enclosedBirds;
    size_t first = std::string::npos;
    size_t second = std::string::npos;

    // Find the first and second occurrences of the id
    for (size_t i = 0; i < row.size(); i++) {
        if (row[i] == id) {
            if (first == std::string::npos) {
                first = i; 
            } else {
                second = i;
                break;
            }
        }
    }

    if (first != std::string::npos && second != std::string::npos) {
        for (size_t i = first + 1; i < second; i++) {
            if (row[i] != id) {
                enclosedBirds.push_back(row[i]);
            }
        }
    }

    return enclosedBirds;
}