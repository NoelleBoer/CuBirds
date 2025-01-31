#include "game.h"
#include <random>
#include <iostream> // to be removed

Game::Game() {
    // Always initialize with 2 players
    players.push_back(Player(1));
    players.push_back(Player(2));

    table.setDrawPile({7,10,10,13,13,17,20,20});// Standard 110 cards of CuBirds
    std::vector<int> row; // Row to be added to the table
    // start with 3 different cards in each row
    int firstCard, secondCard, thirdCard; // Cards of the row
    for (int i = 0; i < numberOfRows; i++) {
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

std::pair<int, int> Game::play() {
    int gameEnds; // Keeps track of whether a collections is complete
    while (true) {
        turn++; // Increment the turn for every round
        for (Player& player : players) { 
            if (table.drawPileEmpty()) return std::make_pair(0, endGame()); // End the game if the drawpile is empty
            if (table.getDrawSize() <= 15 && (players[0].getHandSize() == 0 ||players[1].getHandSize() == 0)) return std::make_pair(0, endGame());
            auto start = std::chrono::high_resolution_clock::now(); //Start the time
            playTurn(player); // Play the turn of a current player
            gameEnds = checkForWin(player); // End the game if the collection is complete
            if (gameEnds!=0) return std::make_pair(gameEnds,player.getIndex());
            auto end = std::chrono::high_resolution_clock::now();// Stop the time
            //Add the time of the turn to the right counter
            if (player.getIndex() == 1) totalTimeP1 += end - start;
            else totalTimeP2 += end-start;
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

void Game::playTurn(Player& player) {
    
    if(!table.drawPileEmpty()) playScoredCards(player, false);
    if(!table.drawPileEmpty()) playFamily(player);
    //Check if the hand of the player is empty
    if (player.getHandSize() == 0 && table.getDrawSize() > 15) {
        //The hand of every player goes to discard pile
        for (Player& plr : players) {
            for (int i = 0; i < kindsOfBirds; i++){
                for (int j = 0; j < plr.getHand()[i]; j++) table.addCardToDiscard(i);
            }
            // std::cout << "4" << std::endl;
            // table.printTable();
            plr.emptyHand();
        }
        //Every player gets 8 new cards (if there are enough in the drawpile)
        for (Player& plr : players) {
            for (int i = 0; i < 8; i++) {
                if (!table.drawPileEmpty()) plr.drawCard(table.drawCard());
                else break;
            }
        }
        if (table.getDrawSize() > 15 && !(players[0].getHandSize() == 0 || players[1].getHandSize() == 0)) playTurn(player);
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
    // Add the cards to the players hand
    if (!result.empty()) {
        for (const int card : result) {
            if (!table.drawPileEmpty()) player.drawCard(card);
        }
        return true;
    }
    return false;
}

void Game::playFamily(Player& player) {
    int awayFromGoalSeven = -1; // 8 unique birds, only 7 needed
    int awayFromGoalTwoThree = 5; // Maximum of 5 cards away from goal
    int solos = 0; // Number of species with 1 collected bird
    int duos = 0; // Number of species with 2 collected birds
    int triples = 0; // Number of species with 3 collected birds
    for (int i = 0; i < kindsOfBirds; i++){
        if (player.getCollection()[i] == 0) {
            awayFromGoalSeven++;
        } else if (player.getCollection()[i] == 1) solos++;
        else if (player.getCollection()[i] == 2) duos++;
        else if (player.getCollection()[i] == 3) triples++;
    }
    if (triples == 1 && duos == 1) awayFromGoalTwoThree = 1;
    else if ((triples == 1 && solos == 1 )|| duos == 2) awayFromGoalTwoThree = 2;
    else if ((duos == 1 && solos == 1) ||triples == 1) awayFromGoalTwoThree = 3;
    else if (solos == 2) awayFromGoalTwoThree = 4;
    //Choose a goal based on how far away from both goals the player is
    //Scaled by o (the favor two seven different birds in a collection)
    if (player.getO()*awayFromGoalSeven > awayFromGoalTwoThree) playTwoThree(player);
    else playSeven(player);
}

void Game::playRandomCards(Player& player) {
    bool cardsCollected = false; // Keeps track whether any cards were collected
    int totalWeight = 0; //Total amount of cards in players hand
    int playingType = 0; //Birdtype that is played
    for (int i = 0; i < kindsOfBirds; i++) {
        totalWeight += player.getHand()[i];
    }
    // Choose a random number between 1 and totalWeight
    std::random_device rd;  // Seed
    std::mt19937 gen(rd()); // Mersenne Twister RNG
    std::uniform_int_distribution<int> dis(1, totalWeight);
    int randomCard = dis(gen);

    // Map the random number to a card ID based on weights
    int cumulativeSum = 0;
    for (int i = 0; i < kindsOfBirds; i++) {
        cumulativeSum += player.getHand()[i];
        if (randomCard <= cumulativeSum) {
            playingType = i; // Select the card ID
            break;
        }
    }
    // Generate a random row and side to play the cards
    std::uniform_int_distribution<int> distribution1(0, 3); // Random row
    std::uniform_int_distribution<int> distribution2(0, 1); // Random side
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

float Game::scoreTwoThreeCards(Player& player, std::vector<int> enclosedBirds){
    float totalScore = 0; // Total obtained score
    float numberCards = 0; // Total number of cards on the board
    int numberSpecies = 0; // Total number of species in a players collection
    std::array<int, 8> currentBoard = {}; //Species on the current board

    // Get an overview of the current board
    for (int i = 0; i < numberOfRows; i++){
        std::vector<int> row = table.getRow(i);
        for (int card : row) {
            numberCards++;
            currentBoard[card]++;
        }
    }

    // Check if there is more than one species in the collection
    for (int i = 0; i < kindsOfBirds; i++){
        if (player.getCollection()[i] != 0) numberSpecies++;
    }

    // Score every card in enclosedBirds and scale by rarity
    // Result = [0,10]*rarityScore per card
    for (const int card : enclosedBirds){
        if (player.getCollection()[card] == 0 && numberSpecies == 1) {
            // Scale the score of the card by how many cards are on the board
            if (numberCards == 0) numberCards = 1;
            totalScore += (10 * (currentBoard[card] / numberCards))*player.getM()*rarityScore[card]; 
        } else if (player.getCollection()[card] == 1) totalScore += 10*player.getM()*rarityScore[card];
        else if (player.getCollection()[card] == 2) totalScore += 10*player.getM()*rarityScore[card];
    }
    return totalScore;
}

float Game::scoreSevenCards(Player& player, std::vector<int> enclosedBirds){
    float totalScore = 0; // Obtained score
    int numberCards = 0; // Number of cards on the boar
    std::array<int, 8> currentBoard = {}; // Species on the board

    //Get an overview of the board
    for (int i = 0; i < numberOfRows; i++){
        std::vector<int> row = table.getRow(i);
        for (int card : row) {
            numberCards++;
            currentBoard[card]++;
        }
    }

    // Score every card in enclosedBirds and scale by rarity
    // Result = [0,10]*rarityScore per card
    for (int card : enclosedBirds){
        if (player.getCollection()[card] == 0) {
            // Scale the score of the card by how many cards are on the board, more cards lesser score
            if (numberCards == 0) numberCards = 1;
            totalScore += (10 * (1 - (currentBoard[card] / numberCards)))*player.getM()*rarityScore[card];
        }
    }
    return totalScore;
}

float Game::scoreGreedyCards(std::vector<int> enclosedBirds) {
    float totalScore = 0;

    for (const int card : enclosedBirds) {
        totalScore += 10*rarityScore[card];
    }
    return totalScore;
}

float Game::scoreCards(Player& player, std::vector<int> enclosedBirds, bool test, int id) {
    int triples = 0; // Number of triplets in collection
    int duos = 0; // Number of duos in collection
    int solos = 0; // Number of solos in collection
    int awayFromGoalTwoThree = 5; // How many cards away from 2x3
    int awayFromGoalSeven = -1; // How many cards away from 7 different birds
    float totalScore = 0; // Total score for all cards

    // Check the current collection and set a score based on how many cards away from goal TwoThree
    for (int i = 0; i < kindsOfBirds; i++){
        if (player.getCollection()[i] == 0) {
            awayFromGoalSeven++;
        } else if (player.getCollection()[i] == 1) solos++;
        else if (player.getCollection()[i] == 2) duos++;
        else if (player.getCollection()[i] == 3) triples++;
    }
    if (triples == 1 && duos == 1) awayFromGoalTwoThree = 1;
    else if ((triples == 1 && solos == 1 )|| duos == 2) awayFromGoalTwoThree = 2;
    else if ((duos == 1 && solos == 1) || triples == 1) awayFromGoalTwoThree = 3;
    else if (solos == 2 || duos == 1) awayFromGoalTwoThree = 4;

    // First score cards on own players collection scaled on rarity score
    // - Added to score: How much the cards that are collected are needed for the collection
    // - Subtracted from the score: how much we need the cards that are played and thus removed from hand for the collection
    // TODO invent a logical way to keep in mind the cards that are layed away and how much we need those
    if (player.getO()*awayFromGoalTwoThree > awayFromGoalSeven) {
        totalScore += scoreSevenCards(player, enclosedBirds);
    } else {
        totalScore += scoreTwoThreeCards(player, enclosedBirds);
    }
    totalScore *= player.getK(); // Scale the score for the own collection by k

    //Score on how much the player needs the new cards in the future
    if (!test){
        std::array<int, 8>  oldHand = player.getHand(); // Store the current hand
        std::array<int, 8>  newHand = oldHand; // Change the hand to check future score
        for (int card : enclosedBirds){
            newHand[card]++;
        }
        player.setHand(newHand);
        totalScore += player.getL() * playScoredCards(player, true);
        player.setHand(oldHand); // Change back to original hand
    }

    //Score cards on how much your opponent needs the cards
    if (!test){
        std::array<int, 8> oldHand = players[(player.getIndex() == 1) ? 1 : 0].getHand();
        std::array<int, 8>  newHand = oldHand; // Change the hand to check future score
        for (int card : enclosedBirds){
            newHand[card]++;
        }
        players[(player.getIndex() == 1) ? 1 : 0].setHand(newHand);
        totalScore += players[(player.getIndex() == 1) ? 1 : 0].getN() * playScoredCards(players[(player.getIndex() == 1) ? 1 : 0], true);
        players[(player.getIndex() == 1) ? 1 : 0].setHand(oldHand); // Change back to original hand
    }
    return totalScore;
}

float Game::playScoredCards(Player& player, bool test){
    float maxScore = 0; // Maximum score that can be obtained
    int optimalRow = 0; // Row of the optimal play
    int optimalSide = 0; // Side of the optimal play
    int optimalType = 0; // Card from the family of the optimal play
    bool cardsCollected = false; // Keeps track whether any cards can be collected
    float score = 0; // Current score
    std::vector<int> row; // Copy of a row on the board
    std::vector<int> enclosedBirds; // Birds that can be collected
    
    for (int i = 0; i < kindsOfBirds; i++) {
        if (player.getHand()[i] != 0) {
            for (int j = 0; j < numberOfRows; j++) {
                // First score by inserting at front
                row = table.getRow(j);
                row.insert(row.begin(), i);
                enclosedBirds = birdsEnclosed(row, i);
                score = scoreCards(player, enclosedBirds, test, i);
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
                score = scoreCards(player, enclosedBirds, test, i);
                if (score > maxScore) {
                    maxScore = score;
                    optimalRow = j;
                    optimalSide = 1;
                    optimalType = i;
                }
            }
        }
    }

    if (test) return maxScore; // If it is a test, dont make the play, but return the score

    // Do the maximal scored play
    table.addCards(optimalType,optimalRow,optimalSide,player.getHand()[optimalType]);
    if (resolveTable(player,optimalRow,optimalType, optimalSide, player.getHand()[optimalType])) cardsCollected = true;
    player.deleteType(optimalType);

    //draw 2 cards if no cards collected and hand is not empty
    if (!cardsCollected && player.getHandSize() != 0) {
        if (!table.drawPileEmpty()) player.drawCard(table.drawCard());
        if (!table.drawPileEmpty()) player.drawCard(table.drawCard());
    }
    return maxScore;
}

void Game::playTwoThree (Player& player){
    // First try to complete the collection by adding a big family
    for (int i = 0; i<kindsOfBirds; i++){
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

    //Check if there is a risk of the opponent clearing his hand
    //High risk if it is later in the game and the opponent has less than 3 cards
    bool highRisk = false;
    if (players[2-player.getIndex()].getHandSize() < 3) highRisk = true;
    else if (players[2-player.getIndex()].getHandSize() < 6 && turn > 10) highRisk = true;

    // Then try to complete a collection by adding a small family
    for (int i = 0; i<kindsOfBirds; i++){
        if (player.getCollection()[i] == 2 || highRisk){
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

    // Check if there is more than one species in the collection
    int numberSpecies = 0;
    for (int i = 0; i < kindsOfBirds; i++){
        if (player.getCollection()[i] != 0) {
            numberSpecies++;
        }
    }

    //Check if current collections can be completed
    int canBeCompleted = 0;
    for (int i = 0; i < kindsOfBirds; i++){
        if (player.getCollection()[i] == 1) {
            if (table.getDrawPile()[i] >= bigFam[i]) canBeCompleted++;
        } else if (player.getCollection()[i] == 2) {
            if (table.getDrawPile()[i] >= smallFam[i]) canBeCompleted++;
        } else if (table.getDrawPile()[i] > 2) canBeCompleted++;
    }


    if (numberSpecies == 1 || canBeCompleted < 2){
        for (int i = 0; i < kindsOfBirds; i++) {
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
}

void Game::playSeven (Player& player){
    // Try to add a bird to the collection that is not yet in the collection
    for (int i = 0; i < kindsOfBirds; i++){
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

void Game::playGreedyFamily(Player& player){
    for (int i = 0; i < kindsOfBirds; i++) {
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

std::vector<int> Game::birdsEnclosed(std::vector<int> row, int id) {
    std::vector<int> enclosedBirds; // Birds that are enclosed
    size_t first = std::vector<int>::size_type(-1); // Index of one of the enclosing birds
    size_t second = std::vector<int>::size_type(-1); // Index of the other enclosing bird

    // Find the first and second occurrences of the id
    for (size_t i = 0; i < row.size(); i++) {
        if (row[i] == id) {
            if (first == std::vector<int>::size_type(-1)) {
                first = i; 
            } else {
                second = i;
                break;
            }
        }
    }

    if (first != std::vector<int>::size_type(-1) && second != std::vector<int>::size_type(-1)) {
        for (size_t i = first + 1; i < second; i++) {
            if (row[i] != id) {
                enclosedBirds.push_back(row[i]);
            }
        }
    }
    return enclosedBirds;
}

//Get functions
int Game::getTurn(){
    return turn;
}

std::chrono::duration<double> Game::getTimeP1(){
    return totalTimeP1;
}

std::chrono::duration<double> Game::getTimeP2(){
    return totalTimeP2;
}

//Set functions
void Game::setPlayer1 (float k, float l, float m, float n, float o) {
    players[0].setVariables(k,l,m,n,o);
}

void Game::setPlayer2 (float k, float l, float m, float n, float o) {
    players[1].setVariables(k,l,m,n,o);
}

void Game::setCollectionP1 (std::array<int, 8>  newCollection) {
    std::array<int,8> oldCollection = players[0].getCollection();
    for (int i = 0; i < kindsOfBirds; i++){
        table.discardPile[i]+=oldCollection[i];
    }
    players[0].setCollection(newCollection);
}

void Game::setCollectionP2 (std::array<int, 8>  newCollection) {
    std::array<int,8> oldCollection = players[1].getCollection();
    for (int i = 0; i < kindsOfBirds; i++){
        table.discardPile[i]+=oldCollection[i];
    }
    players[1].setCollection(newCollection);
}

void Game::setHandP1 (std::array<int, 8>  newHand) {
    std::array<int,8> oldHand = players[0].getHand();
    for (int i = 0; i < kindsOfBirds; i++){
        table.discardPile[i]+=oldHand[i];
    }
    players[0].setHand(newHand);
}

void Game::setHandP2 (std::array<int, 8>  newHand) {
    std::array<int,8> oldHand = players[1].getHand();
    for (int i = 0; i < kindsOfBirds; i++){
        table.discardPile[i]+=oldHand[i];
    }
    players[1].setHand(newHand);
}
