#include "game.h"

/* --- STANDARD GAME LOGIC --- */

Game::Game() {
    //Initialize turn at 0
    turn = 0;

    // Always initialize with 2 players
    players.push_back(Player(1)); //@index is 1
    players.push_back(Player(2)); //@index is 2

    // Initialize @drawPile with the standard 110 cards of CuBirds
    table.setDrawPile({7,10,10,13,13,17,20,20});

    //Create the table row by row
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
        //When all three cards are different add them to the row
        row.push_back(firstCard); 
        row.push_back(secondCard); 
        row.push_back(thirdCard); 
        table.setRow(row, i);
    }

    //every player draws 8 cards
    for (Player& player : players) {
            for (int i = 0; i < kindsOfBirds; i++){
                player.drawCard(table.drawCard());
        }
    }

    //every player gets a starting card in their collection
    for (Player& player : players) {
        player.collectBird(table.drawCard());
    }

    // Start with an empty discard pile
    table.reshuffleFromDiscardPile();
}

int Game::endGame() {
    int numberInCollectionP1 = 0;
    int numberInCollectionP2 = 0;

    for (int i = 0; i < kindsOfBirds; i++) {
        numberInCollectionP1 += players[0].getCollection()[i];
        numberInCollectionP2 += players[1].getCollection()[i];
    }
    if (numberInCollectionP1 == numberInCollectionP2) return 0;
    else if (numberInCollectionP1 >= numberInCollectionP2) return players[0].getIndex();
    else return players[1].getIndex();
}

bool Game::checkForEmptyHand () {
    bool emptyHand = false;
    for (Player& player : players) {
        if (player.getHandSize() == 0) {
            emptyHand = true;
            if (!table.drawPileEmpty() && table.getDrawSize() > 15) {
                //The hand of every player goes to discard pile
                for (Player& plr : players) {
                    for (int i = 0; i < kindsOfBirds; i++){
                        for (int j = 0; j < plr.getHand()[i]; j++) table.addCardToDiscard(i);
                    }
                    plr.emptyHand();
                }
                //Every player gets 8 new cards (if there are enough in the drawpile)
                for (Player& plr : players) {
                    for (int i = 0; i < kindsOfBirds; i++) {
                        if (!table.drawPileEmpty()) plr.drawCard(table.drawCard());
                        else break;
                    }
                }
            }
        }
    }
    return emptyHand;
}

int Game::checkForWin(Player& player) {
    int numberSpecies = 0;
    int numberTriplets = 0;

    for (int i = 0; i < kindsOfBirds; i++) {
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

/* --- STANDARD PLAY LOGIC --- */
std::pair<int, int> Game::play() {
    int gameEnds; // Keeps track of whether a collections is complete
    while (true) {
        turn++; // Increment the turn for every round
        for (Player& player : players) { 

            // If the drawpile is empty the game ends and the player with the biggest collection wins
            if (table.drawPileEmpty()) return std::make_pair(0, endGame());
            //If one of the players has an empty hand and the drawpile does 
            //not have enough cards to give players a new hand, the game ends
            if (table.getDrawSize() <= 15 
                && (players[0].getHandSize() == 0 ||players[1].getHandSize() == 0)) 
                return std::make_pair(0, endGame());

            //Start the time for the current player
            auto start = std::chrono::high_resolution_clock::now();
            //Play the turn for the current player
            playTurn(player);

            // End the game if the collection of this player is complete
            gameEnds = checkForWin(player); 
            if (gameEnds!=0) return std::make_pair(gameEnds,player.getIndex());
            //Stop the time for the current player
            auto end = std::chrono::high_resolution_clock::now();
            //Add the time of the turn to the right counter
            if (player.getIndex() == 1) totalTimeP1 += end - start;
            else totalTimeP2 += end-start;
        }
    }
}

void Game::playTurn(Player& player) { 
    // Lay cards on a side of a row in table
    if(!table.drawPileEmpty()) playScoredCards(player, false);
    // Collect cards by playing family
    if(!table.drawPileEmpty()) playFamily(player);

    //Check if the hand of the player is empty, extra turn if it is
    if (checkForEmptyHand() && player.getHandSize() != 0) playTurn(player);
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

    if (first != std::vector<int>::size_type(-1) 
        && second != std::vector<int>::size_type(-1)) {
        for (size_t i = first + 1; i < second; i++) {
            if (row[i] != id) {
                enclosedBirds.push_back(row[i]);
            }
        }
    }
    return enclosedBirds;
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
    if (triples == 1 && duos >= 1) awayFromGoalTwoThree = 1;
    else if ((triples == 1 && solos >= 1 )|| duos >= 2) awayFromGoalTwoThree = 2;
    else if ((duos == 1 && solos >= 1) || triples == 1) awayFromGoalTwoThree = 3;
    else if (solos >= 2 || duos == 1) awayFromGoalTwoThree = 4;
    //Choose a goal based on how far away from both goals the player is
    //Scaled by o (the favor two seven different birds in a collection)
    if(player.getO()*awayFromGoalTwoThree > awayFromGoalSeven) playSeven(player);
    else playTwoThree(player);
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
    if (players[(player.getIndex() == 1) ? 0 : 1].getHandSize() < 3) highRisk = true;
    else if (players[(player.getIndex() == 1) ? 0 : 1].getHandSize() < 6 && turn > 10) highRisk = true;

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

float Game::scoreCards(Player& player, std::vector<int> enclosedBirds, bool test, int id) {
    int triples = 0; // Number of triplets in collection
    int duos = 0; // Number of duos in collection
    int solos = 0; // Number of solos in collection
    int awayFromGoalTwoThree = 5; // How many cards away from 2x3
    int awayFromGoalSeven = -1; // How many cards away from 7 different birds
    int numberSpecies = 0;
    float alpha = 0;
    float beta = 0;
    float totalScore = 0; // Total score for all cards

    // Check the current collection and set a score based on how many cards away from goal TwoThree
    for (int i = 0; i < kindsOfBirds; i++){
        if (player.getCollection()[i] == 0) {
            awayFromGoalSeven++;
        } else if (player.getCollection()[i] == 1) solos++;
        else if (player.getCollection()[i] == 2) duos++;
        else if (player.getCollection()[i] == 3) triples++;
    }
    if (triples >= 2) awayFromGoalTwoThree = 0;
    else if (triples == 1 && duos >= 1) awayFromGoalTwoThree = 1;
    else if ((triples == 1 && solos >= 1 )|| duos >= 2) awayFromGoalTwoThree = 2;
    else if ((duos == 1 && solos >= 1) || triples == 1) awayFromGoalTwoThree = 3;
    else if (solos >= 2 || duos == 1) awayFromGoalTwoThree = 4;

    // Rate cards on how much they are needed to complete the collection 
    if (player.getO()*awayFromGoalTwoThree > awayFromGoalSeven) {
        for (int card : enclosedBirds){
            if (player.getCollection()[card] == 0) {
                alpha += 1;
            }
        }
    } else {
        // Count how many species are in the collection
        for (int i = 0; i < kindsOfBirds; i++){
            if (player.getCollection()[i] != 0) numberSpecies++;
        }
        for (const int card : enclosedBirds){
            if (player.getCollection()[card] == 0 && numberSpecies == 1) alpha += 1;
            else if (player.getCollection()[card] != 0) alpha += 1;
        }
    }
    if (enclosedBirds.size() != 0) totalScore += player.getK()*(alpha/enclosedBirds.size());

    //Add score based on rarity
    for (const int card : enclosedBirds){
        beta += rarityScore[card];
    }
    if (enclosedBirds.size() != 0) totalScore += player.getM()*(beta/enclosedBirds.size());

    //Add score based on the amount of cards
    constexpr int maxPossibleCards = 110;
    if (enclosedBirds.size() != 0) totalScore += player.getP()*std::log(1 + enclosedBirds.size()) / std::log(maxPossibleCards+1);

    //Score on how much the player needs the new cards in the future (1 if we leave with 1 species)
    if (!test){
        std::array<int, kindsOfBirds>  oldHand = player.getHand(); // Store the current hand
        std::array<int, kindsOfBirds>  newHand = oldHand; // Change the hand to check future score
        for (int card : enclosedBirds){
            newHand[card]++;
        }
        player.setHand(newHand);
        // Count how many species are in the collection
        numberSpecies = 0;
        for (int i = 0; i < kindsOfBirds; i++){
            if (player.getHand()[i] != 0) numberSpecies++;
        }
        if (numberSpecies <= 1) totalScore += player.getL()*1;
        else totalScore += player.getL() * playScoredCards(player, true)/3;
        player.setHand(oldHand); // Change back to original hand
    }

    //Score cards on how much your opponent needs the cards
    if (!test){
        std::array<int, kindsOfBirds> oldHand = players[(player.getIndex() == 1) ? 0 : 1].getHand();
        std::array<int, kindsOfBirds>  newHand = oldHand; // Change the hand to check future score
        for (int card : enclosedBirds){
            newHand[card]++;
        }
        players[(player.getIndex() == 1) ? 0 : 1].setHand(newHand);
        totalScore += player.getN() * playScoredCards(players[(player.getIndex() == 1) ? 0 : 1], true)/3;
        players[(player.getIndex() == 1) ? 0 : 1].setHand(oldHand); // Change back to original hand
    }
    return totalScore;
}

/* --- MC GAME LOGIC--- */
void Game::playRandomTurn(Player& player) {
    if (table.drawPileEmpty() || (table.getDrawSize() <= 15 && player.getHandSize() == 0) || player.getHandSize() == 0)
        return;

    bool cardsCollected = false; // Keeps track whether any cards were collected
    int totalWeight = 0; //Total amount of cards in players hand
    int playingType = 0; //Birdtype that is played
    for (int i = 0; i < kindsOfBirds; i++) {
        totalWeight += player.getHand()[i];
    }

    // Choose a random number between 1 and totalWeight
    std::uniform_int_distribution<int> dis(1, totalWeight);
    int randomCard = dis(table.getRNG());

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
    int row = distribution1(table.getRNG());
    int side = distribution2(table.getRNG());
    table.addCards(playingType,row,side,player.getHand()[playingType]);
    if (resolveTable(player,row,playingType, side, player.getHand()[playingType])) cardsCollected = true;
    player.deleteType(playingType);
    //draw 2 cards if no cards collected 50% of the time
    if (!cardsCollected && std::rand() % 2 == 0) {
        if (!table.drawPileEmpty()) player.drawCard(table.drawCard());
        if (!table.drawPileEmpty()) player.drawCard(table.drawCard());
    }

    //Play a family
    //Calculate the number of possibilities (families that can be played and the option of doing nothing)
    int possibilities = 1;
    for (int i = 0; i < kindsOfBirds; i++) {
        if (player.getHand()[i] >= smallFam[i]) possibilities++;
    }
    int currentPossibility = 0;
    std::uniform_int_distribution<int> fam(0, possibilities); // Random option
    int option = fam(table.getRNG());
    if (option != possibilities) {
        for (int i = 0; i < kindsOfBirds; i++) {
            if (player.getHand()[i] >= smallFam[i]){
                currentPossibility++;
            }
            if (currentPossibility == option){
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

void Game::playMCCards(Player &player, int k, int i, int j, int numberBirds) {
    bool cardsCollected = false;
    if(!table.drawPileEmpty()){
        table.addCards(k,i,j,numberBirds);
        if (resolveTable(player,i,k,j, numberBirds)) cardsCollected = true;
        player.deleteType(k);

        //draw 2 cards if no cards collected and hand is not empty
        if (!cardsCollected && player.getHandSize() != 0) {
            if (!table.drawPileEmpty()) player.drawCard(table.drawCard());
            if (!table.drawPileEmpty()) player.drawCard(table.drawCard());
        }
    }
}

void Game::playMCFamily (Player &player, int m) {
    if (m < kindsOfBirds){
        if (player.getHand()[m] >= bigFam[m]){
            player.collectBird(m);
            player.collectBird(m);
            for (int j=0; j < player.getHand()[m]-2; j++){
                table.addCardToDiscard(m);
            }
            player.deleteType(m);
            return;
        } else if (player.getHand()[m] >= smallFam[m]){
            player.collectBird(m);
            for (int j=0; j < player.getHand()[m]-1; j++){
                table.addCardToDiscard(m);
            }
            player.deleteType(m);
            return;
        }
    }
}

std::pair<int, int> Game::playMCGame () {
    bool gameEnded = false;
    bool skipFirstTurn = false;
    int numberWins = 0;
    int gameEnds = 0; // Keeps track of whether a collections is complete
    int nRepeats = 1000; // How many games are played per possible move
    std::vector<int> cRow0, cRow1, cRow2, cRow3;
    std::array<int, kindsOfBirds> cHand0, cHand1, cCollection0, cCollection1;
    std::array<int, kindsOfBirds> cDiscardPile, cDrawPile;
    int maxWins = -1, maxI = 0, maxJ = 0, maxK = 0, maxM = 0;
    while (true) {
        for (Player& player : players) { 
            while (true) {
                // Reset variables
                maxWins = -1;
                maxI = 0; 
                maxJ = 0;
                maxK = 0;
                maxM = 0;
                if (table.drawPileEmpty()) 
                    return std::make_pair(0, endGame()); // End the game if the drawpile is empty
                if (table.getDrawSize() <= 15 && 
                    (players[0].getHandSize() == 0 || players[1].getHandSize() == 0)) 
                    return std::make_pair(0, endGame());
                //Start the time for the current player
                auto start = std::chrono::high_resolution_clock::now();

                //Store the current state
                cRow0 = table.getRow(0);
                cRow1 = table.getRow(1);
                cRow2 = table.getRow(2);
                cRow3 = table.getRow(3);
                cHand0 = players[0].getHand();
                cHand1 = players[1].getHand();
                cCollection0 = players[0].getCollection();
                cCollection1 = players[1].getCollection();
                cDiscardPile = table.getDiscardPile();
                cDrawPile = table.getDrawPile();

                //Play out random games for every possible move
                for (int i = 0; i < numberOfRows; i++){
                    for (int j = 0; j < numberSides; j++) {
                        for (int k = 0; k < kindsOfBirds; k++){
                            if (player.getHand()[k] != 0){
                                // For every possible family that can be layed down
                                for (int m = 0; m <= kindsOfBirds; m++) {
                                    for (int l = 0; l < nRepeats; l++){
                                        if (m == kindsOfBirds) { // No family is played
                                            numberWins = 0;
                                            if (player.getIndex() == 1) skipFirstTurn = true;
                                            playMCCards(player, k,i,j,player.getHand()[k]);
                                            if (checkForEmptyHand() && player.getHandSize() != 0) playRandomTurn(player);
                                            // play out randomly and store the number of wins
                                            while (true) {
                                                for (Player& plr : players) { 
                                                    if (skipFirstTurn) skipFirstTurn = false;
                                                    else {
                                                        if (table.drawPileEmpty() || (table.getDrawSize() <= 15 
                                                            && (players[0].getHandSize() == 0 || players[1].getHandSize() == 0))) {
                                                            gameEnded = true;
                                                        }
                                                        if (!gameEnded) {
                                                            playRandomTurn(plr); // Play the turn of a current player
                                                            if (checkForEmptyHand() && plr.getHandSize() != 0) playRandomTurn(plr);
                                                            if(checkForWin(plr)!= 0) {
                                                                gameEnded = true;
                                                                if (player.getIndex() == plr.getIndex()) numberWins++;
                                                            }
                                                        }
                                                    }
                                                    if (gameEnded) break;
                                                }
                                                if (gameEnded) break;
                                            }
                                        } else if (player.getHand()[m] >= smallFam[m]){
                                            numberWins = 0;
                                            if (player.getIndex() == 1) skipFirstTurn = true;
                                            playMCCards(player, k,i,j,player.getHand()[k]);
                                            playMCFamily(player, m);
                                            if (checkForEmptyHand() && player.getHandSize() != 0) playRandomTurn(player);
                                            // play out randomly and store the number of wins
                                            while (true) {
                                                for (Player& plr : players) { 
                                                    if (skipFirstTurn) skipFirstTurn = false;
                                                    else {
                                                        if (table.drawPileEmpty() || (table.getDrawSize() <= 15 
                                                            && (players[0].getHandSize() == 0 || players[1].getHandSize() == 0))) {
                                                            gameEnded = true;
                                                        }
                                                        if (!gameEnded) {
                                                            playRandomTurn(plr); // Play the turn of a current player
                                                            if (checkForEmptyHand() && plr.getHandSize() != 0) playRandomTurn(plr);
                                                            if(checkForWin(plr)!= 0) {
                                                                gameEnded = true;
                                                                if (player.getIndex() == plr.getIndex()) numberWins++;
                                                            }
                                                        }
                                                    }
                                                    if (gameEnded) break;
                                                }
                                                if (gameEnded) break;
                                            }
                                        }

                                        // Put the game back in the previous state
                                        table.setRow(cRow0, 0);
                                        table.setRow(cRow1, 1);
                                        table.setRow(cRow2, 2);
                                        table.setRow(cRow3, 3);
                                        players[0].setHand(cHand0);
                                        players[1].setHand(cHand1);
                                        players[0].setCollection(cCollection0);
                                        players[1].setCollection(cCollection1);
                                        table.setDiscardPile(cDiscardPile);
                                        table.setDrawPile(cDrawPile);

                                        // Reset variables
                                        gameEnded = false;
                                    }
                                    // If a new maximum of wins is found change the variables
                                    if (numberWins > maxWins) {
                                        maxWins = numberWins;
                                        maxI = i; 
                                        maxJ = j;
                                        maxK = k;
                                        maxM = m;
                                    }
                                }
                            }
                        }
                    }
                }
                //Play the best move
                turn++;
                playMCCards(player, maxK,maxI,maxJ,player.getHand()[maxK]);
                playMCFamily(player, maxM);

                //Stop the time for the current player
                auto end = std::chrono::high_resolution_clock::now();
                //Add the time of the turn to the right counter
                if (player.getIndex() == 1) totalTimeP1 += end - start;
                else totalTimeP2 += end-start;

                if (!(checkForEmptyHand() && player.getHandSize() != 0)) break; // If the players hand is empty he gets another turn
            }

            gameEnds = checkForWin(player); // End the game if the collection is complete
            if (gameEnds!=0) return std::make_pair(gameEnds,player.getIndex());
        }
    }
}

std::pair<int, int> Game::playRandomGame () {
    while (true) {
        for (Player& plr : players) { 
            turn++;
            if (table.drawPileEmpty() || (table.getDrawSize() <= 15 
                && (players[0].getHandSize() == 0 || players[1].getHandSize() == 0))) {
                return std::make_pair(0,endGame());
            }
            auto start = std::chrono::high_resolution_clock::now();
            playRandomTurn(plr); // Play the turn of a current player
            auto end = std::chrono::high_resolution_clock::now();
            if (plr.getIndex() == 1) totalTimeP1 += end - start;
            else totalTimeP2 += end-start;
            if (checkForEmptyHand() && plr.getHandSize() != 0) playRandomTurn(plr);
            if(checkForWin(plr)!= 0) {
                return std::make_pair(checkForWin(plr),plr.getIndex());
            }
        }
    }
}

/* --- GET/SET FUNCTIONS --- */
int Game::getTurn(){
    return turn;
}

std::chrono::duration<double> Game::getTimeP1(){
    return totalTimeP1;
}

std::chrono::duration<double> Game::getTimeP2(){
    return totalTimeP2;
}

void Game::setPlayer1 (float k, float l, float m, float n, float o, float p) {
    players[0].setVariables(k,l,m,n,o,p);
}

void Game::setPlayer2 (float k, float l, float m, float n, float o, float p) {
    players[1].setVariables(k,l,m,n,o,p);
}

void Game::setCollectionP1 (std::array<int, kindsOfBirds>  newCollection) {
    std::array<int,kindsOfBirds> oldCollection = players[0].getCollection();
    for (int i = 0; i < kindsOfBirds; i++){
        table.discardPile[i]+=oldCollection[i];
        table.reshuffleFromDiscardPile();
        table.drawPile[i]-=newCollection[i];
    }
    players[0].setCollection(newCollection);
}

void Game::setCollectionP2 (std::array<int, kindsOfBirds>  newCollection) {
    std::array<int,kindsOfBirds> oldCollection = players[1].getCollection();
    for (int i = 0; i < kindsOfBirds; i++){
        table.discardPile[i]+=oldCollection[i];
        table.reshuffleFromDiscardPile();
        table.drawPile[i]-=newCollection[i];
    }
    players[1].setCollection(newCollection);
}


void Game::setHandP1 (std::array<int, kindsOfBirds>  newHand) {
    std::array<int,kindsOfBirds> oldHand = players[0].getHand();
    for (int i = 0; i < kindsOfBirds; i++){
        table.discardPile[i]+=oldHand[i];
        table.reshuffleFromDiscardPile();
        table.drawPile[i]-=newHand[i];
    }
    players[0].setHand(newHand);
}

void Game::setHandP2 (std::array<int, kindsOfBirds>  newHand) {
    std::array<int,kindsOfBirds> oldHand = players[1].getHand();
    for (int i = 0; i < kindsOfBirds; i++){
        table.discardPile[i]+=oldHand[i];
        table.reshuffleFromDiscardPile();
        table.drawPile[i]-=newHand[i];
    }
    players[1].setHand(newHand);
}